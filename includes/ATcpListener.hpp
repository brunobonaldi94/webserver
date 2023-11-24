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
#include "requests/RequestHandler.hpp"
#include "StringUtils.hpp"
#include "ServerContext.hpp"

class ATcpListener
{

public:

	ATcpListener(RequestHandler requestHandler, std::vector<AContext *> serverContexts);

	// Initialize the listener
	bool Init();

	// Run the listener
	bool Run();

protected:

	// Handler for client connections
	virtual void HandleNewConnection(int clientSocket);

	// Handler for client connections
	virtual void HandleOnGoingConnection(int clientSocket, int socketIndex);

	// Handler for client disconnections

	virtual void OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes);

	// Handler for when a message is received from the client
	virtual void OnMessageReceived(ServerContext *serverContext, int clientSocket, int socketIndex, size_t nbytes, const char* msg) = 0;

	// Send a message to a client
	void SendToClient(int clientSocket, const char* msg, int length) const;

	RequestHandler requestHandler;

private:

	int GetListenerSocket(AContext * serverContext);
	struct addrinfo* GetAddressInfo(std::string ipAddress, std::string port);
	int BindSocket(struct addrinfo*  addrinfo);
	bool IsListeningSocket(int fd);
	void AddToPfds(int newfd);
	void RemoveFromPfds(int i);
	void AddToListenFds(int newfd, ServerContext * serverContext);
	void RemoveFromListenFds(int i);
	void AddToSocketFdToServerContext(int newfd, ServerContext * serverContext);
	void RemoveFromSocketFdToServerContext(int i);


	//int														m_socket;		// Internal FD for the listening socket
	std::vector<AContext *> 			m_serverContexts;
	char 													m_buffer[4096];	// Buffer for incoming data
	std::vector<struct pollfd>		pfds;			// Pointer to the pollfd array
	static const int 							MAXFDS = 100;	// Maximum number of file descriptors
	std::map<int, ServerContext*> listenFds;
	std::map<int, ServerContext*> m_socketFdToServerContext;
	
};
