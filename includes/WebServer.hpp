#pragma once

#include "ATcpListener.hpp"

class WebServer : public ATcpListener
{
public:

	WebServer(std::vector<AContext *> serverContexts) :
		ATcpListener(serverContexts) { }

protected:

	// Handler for client disconnections
	void OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes);
	// Handler for when a message is received from the client
	void OnMessageReceived(ServerContext *serverContext, int clientSocket, const char* msg) const;
};