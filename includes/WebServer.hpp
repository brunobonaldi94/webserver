#pragma once

#include "ATcpListener.hpp"
#include "requests/RequestHandler.hpp"

class WebServer : public ATcpListener
{
	public:

		WebServer(std::string ipAddress, std::string port, RequestHandler r) :
			ATcpListener(ipAddress, port, r) {}

	protected:

		// Handler for client disconnections
		void onClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes);
		// Handler for when a message is received from the client
		void onMessageReceived(int clientSocket, const char* msg);
			
};