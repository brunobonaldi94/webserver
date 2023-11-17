#include "ATcpListener.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

ATcpListener::ATcpListener(RequestHandler requestHandler, std::vector<AContext *> serverContexts) :
	requestHandler(requestHandler), m_serverContexts(serverContexts) 
{
}

struct addrinfo * ATcpListener::GetAddressInfo(std::string ipAddress, std::string port)
{
		int rv;
		struct addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE; // use my IP
		if ((rv = getaddrinfo(ipAddress.c_str(), port.c_str(), &hints, &servinfo)) != 0) 
		{
				std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
				return NULL;
		}
		return servinfo;
}

int ATcpListener::BindSocket(struct addrinfo *addrinfo)
{
	 struct addrinfo *p;
	 int yes = 1;
	 int listener; 
	 for(p = addrinfo; p != NULL; p = p->ai_next) 
		{
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) 
            continue;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
				{
            close(listener);
            continue;
        }
        break;
    }
		freeaddrinfo(addrinfo);
		if (p == NULL)
		{
				Logger::Log(ERROR, "Failed to bind socket");
				return -1;
		}
		return listener;
}

int ATcpListener::GetListenerSocket(AContext * serverContext)
{
    int listener;
    struct addrinfo *ai;
		ServerContext *serverContextCast = dynamic_cast<ServerContext *>(serverContext);
		if (serverContextCast == NULL)
		{
				Logger::Log(ERROR, "Failed to cast server context");
				return -1;
		}
		ListenDirective *listenDirective = dynamic_cast<ListenDirective *>(serverContextCast->GetDirectives()["listen"]);
    ai = this->GetAddressInfo(listenDirective->GetHost(), listenDirective->GetPort());
    listener = this->BindSocket(ai);
		if (listener == -1)
				return -1;
    if (listen(listener, SOMAXCONN) == -1)
        return -1;
		Logger::Log(INFO, "Listening on host - " + listenDirective->GetHost() + ":" + listenDirective->GetPort());
    return listener;
}

bool ATcpListener::IsListeningSocket(int fd)
{
	std::pair<const int, ServerContext*> *found = MapUtils<int, ServerContext*>::SafeFindMap(this->listenFds, fd);
	if (found == NULL)
		return false;
	return true;
}

void ATcpListener::AddToPfds(int newfd)
{
		struct pollfd pfd;
		pfd.fd = newfd;
		pfd.events = POLLIN;
		this->pfds.push_back(pfd);
}

void ATcpListener::AddToListenFds(int newfd, ServerContext *serverContext)
{
		this->listenFds[newfd] = serverContext;
}

void ATcpListener::RemoveFromListenFds(int i)
{
		MapUtils<int, ServerContext *>::SafeRemoveMap(this->listenFds, i);
}

void ATcpListener::AddToSocketFdToServerContext(int newfd, ServerContext *serverContext)
{
		this->m_socketFdToServerContext[newfd] = serverContext;
}

void ATcpListener::RemoveFromSocketFdToServerContext(int i)
{
		MapUtils<int, ServerContext *>::SafeRemoveMap(this->m_socketFdToServerContext, i);
}

void ATcpListener::HandleNewConnection(int clientSocket)
{
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);
	int newfd = accept(clientSocket, (struct sockaddr *)&remoteaddr, &addrlen);
	if (newfd == -1) 
		Logger::Log(ERROR, "accept");
	else
	{
		this->AddToPfds(newfd);
		this->AddToSocketFdToServerContext(newfd, this->listenFds[clientSocket]);
	}
}

void ATcpListener::RemoveFromPfds(int i)
{
		this->pfds.erase(this->pfds.begin() + i);
}

bool ATcpListener::Init()
{
	for (std::vector<AContext *>::iterator it = this->m_serverContexts.begin(); it != this->m_serverContexts.end(); it++)
	{
		int listenFd = this->GetListenerSocket(*it);
		if (listenFd == -1)
		{
			Logger::Log(ERROR, "Failed to create listener socket");
			return false;
		}
		this->AddToListenFds(listenFd, dynamic_cast<ServerContext *>(*it));
	}
	for (std::map<int, ServerContext *>::iterator it = this->listenFds.begin(); it != this->listenFds.end(); it++)
		this->AddToPfds(it->first);
	return true;
}

void ATcpListener::HandleOnGoingConnection(int clientSocket, int socketIndex)
{
	ssize_t nbytes = recv(clientSocket, this->m_buffer, sizeof this->m_buffer, 0);
	if (nbytes > 0)
	{
		this->OnMessageReceived(this->m_socketFdToServerContext[clientSocket], clientSocket, this->m_buffer);
		return ;
	}
	this->OnClientDisconnected(clientSocket, socketIndex, nbytes);
}

void ATcpListener::SendToClient(int clientSocket, const char* msg, int length) const
{
	if (send(clientSocket, msg, length, 0) == -1)
		Logger::Log(ERROR, "send");
}

void ATcpListener::OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
 	  if (nbytes == 0)
			Logger::Debug(
				"ATcpListener::OnClientDisconnected",
				INFO,
				"socket " + StringUtils::ConvertNumberToString(clientSocket) + " hung up");
		else
			Logger::Log(ERROR, "recv");
		close(this->pfds[socketIndex].fd);
		this->RemoveFromPfds(socketIndex);
		this->RemoveFromSocketFdToServerContext(clientSocket);
}

bool ATcpListener::Run()
{
	bool running = true;
	while (running)
	{
		int poll_count = poll(&this->pfds[0], this->pfds.size(), -1);
		int socketCount = this->pfds.size();
		if (poll_count == -1)
		{
			Logger::Log(ERROR, "poll error");
			return false;
		}
		for (int i = 0; i < socketCount; i++)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				if (this->IsListeningSocket(this->pfds[i].fd))
					this->HandleNewConnection(this->pfds[i].fd);
				else
					this->HandleOnGoingConnection(this->pfds[i].fd, i);
			}
		}	
	}
	return true;
}