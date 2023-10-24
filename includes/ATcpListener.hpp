#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "Logger.hpp"

class ATcpListener
{

public:

	ATcpListener(std::string ipAddress, std::string port);

	// Initialize the listener
	int Init();

	// Run the listener
	int Run();

protected:

	// Handler for client connections
	virtual void HandleNewConnection(int clientSocket);

	// Handler for client disconnections

	virtual void OnClientDisconnected(int clientSocket, int socketIndex,ssize_t nbytes);

	// Handler for when a message is received from the client
	virtual void OnMessageReceived(int clientSocket, const char* msg) const = 0;

	// Send a message to a client
	void SendToClient(int clientSocket, const char* msg, int length) const;

private:

	int GetListenerSocket(void);
	struct addrinfo* GetAddressInfo(void);
	int BindSocket(struct addrinfo*  addrinfo);
	void AddToPfds(int newfd);
	void RemoveFromPfds(int i);

	std::string										m_ipAddress;	// IP Address server will run on
	std::string										m_port;			// Port # for the web service
	//int														m_socket;		// Internal FD for the listening socket
	std::vector<struct pollfd>		pfds;			// Pointer to the pollfd array
	static const int 							MAXFDS = 100;	// Maximum number of file descriptors
	int														listenfd;											// File descriptor for the listening socket
	
};
