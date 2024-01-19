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
#include <csignal>
#include <fcntl.h>
#include "Logger.hpp"
#include "requests/RequestHandler.hpp"
#include "StringUtils.hpp"
#include "ServerConfig.hpp"

class ATcpListener
{

public:

	ATcpListener(BaseHTTPRequestHandler *requestHandler, std::vector<ServerConfig *> serverConfigs);
	static ATcpListener* runningInstance;

	virtual ~ATcpListener();

	// Initialize the listener
	bool Init();

	// Run the listener
	bool Run();

	void Stop(int sig);

protected:

	// Handler for client connections
	virtual void HandleNewConnection(int clientSocket);

	// Handler for client connections
	virtual void HandleOnGoingConnection(int clientSocket, int socketIndex);

	// Handler for client disconnections

	virtual void OnClientDisconnected(int clientSocket, int socketIndex,ssize_t nbytes);

	// Handler for when a message is received from the client
	virtual void OnMessageReceived(ServerConfig *serverConfig, int clientSocket, std::string msg) = 0;

	// Send a message to a client
	void SendToClient(int clientSocket, std::string msg, int length) const;

	BaseHTTPRequestHandler *requestHandler;

private:

	int GetListenerSocket(ServerConfig * ServerConfig);
	struct addrinfo* GetAddressInfo(std::string ipAddress, std::string port);
	int BindSocket(struct addrinfo*  addrinfo);
	bool IsListeningSocket(int fd);
	void AddToPfds(int newfd);
	void RemoveFromPfds(int i);
	void AddToListenFds(int newfd, ServerConfig * serverConfig);
	void RemoveFromListenFds(int i);
	void AddToSocketFdToServerConfig(int newfd, ServerConfig * ServerConfig);
	void RemoveFromSocketFdToServerConfig(int i);
	void RemoveFromBufferMap(int i);
	void AddToBufferMap(int newfd);


	//int														m_socket;		// Internal FD for the listening socket
	std::vector<ServerConfig *> 	m_serverConfigs;
	char 													m_buffer[BUFFER_SIZE];	// Buffer for incoming data
	std::vector<struct pollfd>		pfds;			// Pointer to the pollfd array
	static const int 							MAXFDS = 100;	// Maximum number of file descriptors
	std::map<int, ServerConfig*> listenFds;
	std::map<int, ServerConfig*> m_socketFdToServerConfigs;
	bool 													m_isRunning;	// Flag indicating the listener is running
	std::map<int, std::string> 	 m_bufferMap;
	static void StaticStop(int signal);
};
