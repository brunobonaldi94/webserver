#pragma once

#include "ATcpListener.hpp"
#include "requests/RequestHandler.hpp"
#include "requests/HTTPStatus.hpp"

class WebServer : public ATcpListener
{
	public:

	WebServer(RequestHandler requestHandler, std::vector<AContext *> serverContexts);


	protected:

	// Handler for client disconnections
	void OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes);
	// Handler for when a message is received from the client
	void OnMessageReceived(ServerContext *serverContext, int clientSocket, int socketIndex, size_t nbytes, const char* msg);
};