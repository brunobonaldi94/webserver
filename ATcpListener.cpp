#include "ATcpListener.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>


ATcpListener::ATcpListener(std::string ipAddress, std::string port) :
	m_ipAddress(ipAddress), m_port(port)
{
}


int ATcpListener::getListenerSocket(void)
{
    int listener;     // Listening socket descriptor
    int yes = 1;        // For setsockopt() SO_REUSEADDR, below
    int rv;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, this->m_port.c_str(), &hints, &ai)) != 0) {
				std::cerr << "selectserver: " << gai_strerror(rv) << std::endl;
        return -1;
    }
    for(p = ai; p != NULL; p = p->ai_next) 
		{
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) 
            continue;
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
        break;
    }
    freeaddrinfo(ai); // All done with this
    if (p == NULL)
        return -1;
    if (listen(listener, 10) == -1)
        return -1;
    return listener;
}

void ATcpListener::addToPfds(int newfd)
{
		struct pollfd pfd;
		pfd.fd = newfd;
		pfd.events = POLLIN;
		this->pfds.push_back(pfd);
}

void ATcpListener::handleNewConnection(int clientSocket)
{
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;
	addrlen = sizeof(remoteaddr);
	int newfd = accept(clientSocket, (struct sockaddr *)&remoteaddr, &addrlen);
	if (newfd == -1) 
		std::cerr << "accept" << std::endl;
	else
		this->addToPfds(newfd);
}

void ATcpListener::removeFromPfds(int i)
{
		this->pfds.erase(this->pfds.begin() + i);
}

int ATcpListener::init()
{
	this->listenfd = this->getListenerSocket();
	if (this->listenfd == -1)
	{
		std::cerr << "Failed to create listener socket" << std::endl;
		return -1;
	}
	this->addToPfds(this->listenfd);
	return 0;
}

int ATcpListener::run()
{
	// this will be changed by the \quit command (see below, bonus not in video!)
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
					this->handleNewConnection(this->pfds[i].fd);
				}
				else
				{
					 int sender_fd = this->pfds[i].fd;
					 ssize_t nbytes = recv(sender_fd, buff, sizeof buff, 0);
					 if (nbytes <= 0)
					 {
							this->onClientDisconnected(sender_fd, i, nbytes);
					 }
					 else
					 {
						 this->onMessageReceived(sender_fd, buff);
					 }

				}
			}
		}	
	}
	return 0;
}
void ATcpListener::sendToClient(int clientSocket, const char* msg, int length) const
{
	if (send(clientSocket, msg, length, 0) == -1)
		std::cerr << "send" << std::endl;
}

void ATcpListener::onClientDisconnected(int clientSocket, int socketIndex,ssize_t nbytes)
{
 	  if (nbytes == 0)
			std::cerr << "socket " << clientSocket << " hung up" << std::endl; 
		else
			std::cerr << "recv" << std::endl;
		close(this->pfds[socketIndex].fd);
		this->removeFromPfds(socketIndex);
}