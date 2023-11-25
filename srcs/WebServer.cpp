#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include "WebServer.hpp"

WebServer::WebServer(RequestHandler requestHandler, std::vector<AContext *> serverContexts) : ATcpListener(requestHandler, serverContexts)
{
}

// Handler for when a message is received from the client
void WebServer::OnMessageReceived(ServerContext *serverContext, int clientSocket, int socketIndex, size_t nbytes, const char* msg)
{
	(void) nbytes;
	(void) socketIndex;
	ListenDirective *listenDirective = serverContext->GetListenDirective();
	Logger::Log(INFO, "Received from client: " + listenDirective->GetHost() + ":" + listenDirective->GetPort());
	if (this->requestHandler.parseRequest(msg) == true)
		this->requestHandler.doGET();
	
	this->SendToClient(
		clientSocket, 
		this->requestHandler.headersBufferToString().c_str(), 
		this->requestHandler.headersBufferToString().size());
	
	/* if (this->requestHandler.parseRequest(msg) == false)
		OnClientDisconnected(clientSocket, socketIndex, nbytes); */

	this->requestHandler.clearHeadersBuffers();
}

// Handler for client disconnections
void WebServer::OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
	ATcpListener::OnClientDisconnected(clientSocket, socketIndex, nbytes);
	std::cout << "Client disconnected: " << clientSocket << std::endl;
}