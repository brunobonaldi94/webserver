#include "ATcpListener.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

ATcpListener::ATcpListener(BaseHTTPRequestHandler *requestHandler, std::vector<ServerConfig *> serverConfigs) :
	requestHandler(requestHandler), m_serverConfigs(serverConfigs), m_isRunning(true)
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

				if (fcntl(listener, F_SETFL, O_NONBLOCK) < 0) {
            close(listener);
            continue;
        }

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

int ATcpListener::GetListenerSocket(ServerConfig * serverConfig)
{
    int listener;
    struct addrinfo *ai;
    ai = this->GetAddressInfo(serverConfig->GetHost(), serverConfig->GetPort());
    listener = this->BindSocket(ai);
		if (listener == -1)
				return -1;
    if (listen(listener, SOMAXCONN) == -1)
        return -1;
		Logger::Log(INFO, "Listening on host - http://" + serverConfig->GetHost() + ":" + serverConfig->GetPort());
    return listener;
}

bool ATcpListener::IsListeningSocket(int fd)
{
	std::pair<const int, ServerConfig*> *found = MapUtils<int, ServerConfig*>::SafeFindMap(this->listenFds, fd);
	if (found == NULL)
		return false;
	return true;
}

void ATcpListener::AddToPfds(int newfd)
{
		struct pollfd pfd;
		pfd.fd = newfd;
		pfd.events = POLLIN | POLLOUT;
		pfd.revents = 0x000;
		this->pfds.push_back(pfd);
}

void ATcpListener::AddToListenFds(int newfd, ServerConfig *serverConfig)
{
		this->listenFds[newfd] = serverConfig;
}

void ATcpListener::RemoveFromListenFds(int i)
{
		MapUtils<int, ServerConfig *>::SafeRemoveMap(this->listenFds, i);
}

void ATcpListener::AddToSocketFdToServerConfig(int newfd, ServerConfig *serverConfig)
{
		this->m_socketFdToServerConfigs[newfd] = serverConfig;
}

void ATcpListener::RemoveFromSocketFdToServerConfig(int i)
{
		MapUtils<int, ServerConfig *>::SafeRemoveMap(this->m_socketFdToServerConfigs, i);
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
		this->AddToSocketFdToServerConfig(newfd, this->listenFds[clientSocket]);
	}
}

void ATcpListener::RemoveFromPfds(int i)
{
		 if (i >= 0 && i < static_cast<int>(this->pfds.size())) 
        this->pfds.erase(this->pfds.begin() + i);
}

bool ATcpListener::Init()
{
	for (std::vector<ServerConfig *>::iterator it = this->m_serverConfigs.begin(); it != this->m_serverConfigs.end(); it++)
	{
		int listenFd = this->GetListenerSocket(*it);
		if (listenFd == -1)
		{
			Logger::Log(ERROR, "Failed to create listener socket");
			return false;
		}
		this->AddToListenFds(listenFd, *it);
	}
	for (std::map<int, ServerConfig *>::iterator it = this->listenFds.begin(); it != this->listenFds.end(); it++)
		this->AddToPfds(it->first);
	return true;
}

void ATcpListener::HandleOnGoingConnection(int clientSocket, int socketIndex)
{
	memset(this->m_buffer, 0, sizeof this->m_buffer);
	ssize_t nbytes = recv(clientSocket, this->m_buffer, sizeof this->m_buffer - 1, 0);
	if (nbytes > 0)
	{
		this->m_buffer[nbytes] = '\0';
		std::string msg = std::string(this->m_buffer, nbytes);
		this->OnMessageReceived(this->m_socketFdToServerConfigs[clientSocket], clientSocket, msg);
		return ;
	}
	this->OnClientDisconnected(clientSocket, socketIndex, nbytes);
}

bool ATcpListener::SendToClient(int clientSocket, std::string msg, int length)
{
	if (send(clientSocket, msg.c_str(), length, 0) == -1)
	{
		Logger::Log(ERROR, "send");
		return false;
	}
	return true;
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
		this->RemoveFromSocketFdToServerConfig(clientSocket);
}

bool ATcpListener::Run()
{
	this->runningInstance = this;
	std::signal(SIGINT, ATcpListener::StaticStop);
	std::signal(SIGQUIT, ATcpListener::StaticStop);
	while (this->m_isRunning)
	{
		int poll_count = poll(&this->pfds[0], this->pfds.size(), 0);
		if (poll_count == -1)
		{
			Logger::Log(ERROR, "poll error");
			return false;
		}
		for (int i = 0; i < static_cast<int>(this->pfds.size()); i++)
		{
			short revents = this->pfds[i].revents;
			int clientSocket = this->pfds[i].fd;
			if (revents & POLLIN)
			{
				if (this->IsListeningSocket(clientSocket))
					this->HandleNewConnection(clientSocket);
				else
					this->HandleOnGoingConnection(clientSocket, i);
			}
			if (revents & POLLOUT)
			{
				this->SendReponseToClient(clientSocket, i);
			}
			if (revents & POLLHUP || revents & POLLERR || revents & POLLNVAL || revents & POLLRDHUP)
				this->OnClientDisconnected(clientSocket, i, 0);
		}		
	}
	return true;
}

ATcpListener::~ATcpListener()
{
	VectorUtils<ServerConfig *>::clearVector(this->m_serverConfigs);
	delete this->requestHandler;
}

void ATcpListener::Stop(int sig)
{
	if (sig == SIGINT || sig == SIGQUIT)
	{
		this->m_isRunning = false;
		Logger::Log(INFO, "Stopping server");
	}
}

void ATcpListener::StaticStop(int signal)
{
	if (runningInstance)
	{
  	runningInstance->Stop(signal);
	}
}

ATcpListener* ATcpListener::runningInstance = NULL;