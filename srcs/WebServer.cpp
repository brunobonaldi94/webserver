#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include "WebServer.hpp"

// Handler for when a message is received from the client
void WebServer::onMessageReceived(int clientSocket, const char* msg) 
{
	if (this->requestHandler.parseRequest(msg) == true)
		this->requestHandler.doGET();
	this->sendToClient(
		clientSocket, 
		this->requestHandler.headersBufferToString().c_str(), 
		this->requestHandler.headersBufferToString().size());
	this->requestHandler.clearHeadersBuffers();
}

// Handler for client disconnections
void WebServer::onClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
	ATcpListener::onClientDisconnected(clientSocket, socketIndex, nbytes);
	std::cout << "Client disconnected: " << clientSocket << std::endl;
}