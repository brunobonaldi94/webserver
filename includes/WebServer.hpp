#pragma once

#include "ATcpListener.hpp"
#include "requests/RequestHandler.hpp"
#include "requests/HTTPStatus.hpp"

class WebServer : public ATcpListener
{
	public:

	WebServer(BaseHTTPRequestHandler *requestHandler, std::vector<ServerConfig *> serverConfigs);
	~WebServer();

	protected:

	// Handler for client disconnections
	void OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes);
	// Handler for when a message is received from the client
	void OnMessageReceived(ServerConfig *ServerConfig, int clientSocket, std::string msg);
};