#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include "WebServer.hpp"

WebServer::WebServer(RequestHandler requestHandler, std::vector<ServerConfig *> serverConfigs) : ATcpListener(requestHandler, serverConfigs)
{
}

WebServer::~WebServer()
{
	this->requestHandler.~RequestHandler();
}

// Handler for when a message is received from the client
void WebServer::OnMessageReceived(ServerConfig *serverConfig, int clientSocket, const char* msg)
{
	BaseHTTPRequestHandler::RequestMethodFunction method = this->requestHandler.parseRequestForClientSocket(msg, clientSocket, serverConfig);
	if (method != NULL)
		(this->requestHandler.*method)();
	this->SendToClient(
		clientSocket, 
		this->requestHandler.headersBufferToString().c_str(), 
		this->requestHandler.headersBufferToString().size()
	);
	this->requestHandler.clearHeadersBuffers();
}

// Handler for client disconnections
void WebServer::OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
	ATcpListener::OnClientDisconnected(clientSocket, socketIndex, nbytes);
	this->requestHandler.clearRequestContent(clientSocket);
	std::cout << "Client disconnected: " << clientSocket << std::endl;
}