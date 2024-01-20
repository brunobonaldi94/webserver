#include <string>
#include <istream>
#include <sstream>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>
#include <iterator>
#include "WebServer.hpp"

WebServer::WebServer(BaseHTTPRequestHandler *requestHandler, std::vector<ServerConfig *> serverConfigs) : ATcpListener(requestHandler, serverConfigs)
{
}

WebServer::~WebServer()
{
}

// Handler for when a message is received from the client
void WebServer::OnMessageReceived(ServerConfig *serverConfig, int clientSocket, std::string msg)
{
	try 
	{
		BaseHTTPRequestHandler::RequestMethodFunction method = this->requestHandler->parseRequestForClientSocket(msg, clientSocket, serverConfig);
		if (method != NULL)
			(this->requestHandler->*method)();
	} 
	catch (std::exception &e) 
	{
		Logger::Log(ERROR, e.what());
	}
}

void WebServer::SendReponseToClient(int clientSocket)
{
		if (this->requestHandler->getCurrentRequestContent() == NULL)
			return;
		if (this->requestHandler->hasParsedAllRequestContent() || this->requestHandler->getCurrentRequestContent()->getHasErrorInRequest())
			ATcpListener::SendToClient(
				clientSocket,
				this->requestHandler->headersBufferToString(),
				this->requestHandler->headersBufferToString().size()
			);
		this->requestHandler->shouldClearRequestContent(clientSocket);
}

// Handler for client disconnections
void WebServer::OnClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
	ATcpListener::OnClientDisconnected(clientSocket, socketIndex, nbytes);
	this->requestHandler->clearRequestContent(clientSocket);
	std::cout << "Client disconnected: " << clientSocket << std::endl;
}