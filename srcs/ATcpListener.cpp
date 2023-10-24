#include "ATcpListener.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>


ATcpListener::ATcpListener(std::string ipAddress, std::string port) :
	m_ipAddress(ipAddress), m_port(port)
{
}

struct addrinfo * ATcpListener::GetAddressInfo(void)
{
		int rv;
		struct addrinfo hints, *servinfo;
		memset(&hints, 0, sizeof hints);
		hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags = AI_PASSIVE; // use my IP
		if ((rv = getaddrinfo(this->m_ipAddress.c_str(), this->m_port.c_str(), &hints, &servinfo)) != 0) 
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
				std::cerr << "Failed to bind socket" << std::endl;
				return -1;
		}
		return listener;
}

int ATcpListener::GetListenerSocket(void)
{
    int listener;
    struct addrinfo *ai;

    ai = this->GetAddressInfo();
    listener = this->BindSocket(ai);
		if (listener == -1)
				return -1;
    if (listen(listener, SOMAXCONN) == -1)
        return -1;
    return listener;
}

void ATcpListener::AddToPfds(int newfd)
{
		struct pollfd pfd;
		pfd.fd = newfd;
		pfd.events = POLLIN;
		this->pfds.push_back(pfd);
}

void ATcpListener::HandleNewConnection(int clientSocket)
{
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);
	int newfd = accept(clientSocket, (struct sockaddr *)&remoteaddr, &addrlen);
	if (newfd == -1) 
		std::cerr << "accept" << std::endl;
	else
		this->AddToPfds(newfd);
}

void ATcpListener::RemoveFromPfds(int i)
{
		this->pfds.erase(this->pfds.begin() + i);
}

int ATcpListener::Init()
{
	this->listenfd = this->GetListenerSocket();
	if (this->listenfd == -1)
	{
		std::cerr << "Failed to create listener socket" << std::endl;
		return -1;
	}
	this->AddToPfds(this->listenfd);
	Logger::Log(INFO, "Listening on port " + this->m_port);
	return 0;
}

int ATcpListener::Run()
{
	bool running = true;
	char buff[4096];
	while (running)
	{
	
		int poll_count = poll(&this->pfds[0], this->pfds.size(), -1);
		int socketCount = this->pfds.size();
		if (poll_count == -1)
		{
			std::cerr << "poll" << std::endl;
			return -1;
		}
		for (int i = 0; i < socketCount; i++)
		{
			if (this->pfds[i].revents & POLLIN)
			{
				if (this->pfds[i].fd == this->listenfd)
				{
					this->HandleNewConnection(this->pfds[i].fd);
				}
				else
				{
					 int sender_fd = this->pfds[i].fd;
					 ssize_t nbytes = recv(sender_fd, buff, sizeof buff, 0);
					 if (nbytes <= 0)
					 {
							this->OnClientDisconnected(sender_fd, i, nbytes);
					 }
					 else
					 {
						 this->OnMessageReceived(sender_fd, buff);
					 }

				}
			}
		}	
	}
	return 0;
}
void ATcpListener::SendToClient(int clientSocket, const char* msg, int length) const
{
	if (send(clientSocket, msg, length, 0) == -1)
		std::cerr << "send" << std::endl;
}

void ATcpListener::OnClientDisconnected(int clientSocket, int socketIndex,ssize_t nbytes)
{
 	  if (nbytes == 0)
			std::cerr << "socket " << clientSocket << " hung up" << std::endl; 
		else
			std::cerr << "recv" << std::endl;
		close(this->pfds[socketIndex].fd);
		this->RemoveFromPfds(socketIndex);
}