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
	// Parse out the client's request string e.g. GET /index.html HTTP/1.1
	std::istringstream iss(msg);
	std::cout << msg << std::endl;
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	// Some defaults for output to the client (404 file not found 'page')
	std::string content = "<h1>404 Not Found</h1>";
	std::string htmlFile = "/index.html";
	int errorCode = 404;

	// If the GET request is valid, try and get the name
	if (parsed.size() >= 3 && parsed[0] == "GET")
	{
		htmlFile = parsed[1];

		// If the file is just a slash, use index.html. This should really
		// be if it _ends_ in a slash. I'll leave that for you :)
		if (htmlFile == "/")
		{
			htmlFile = "/index.html";
		}
	}

	// Open the document in the local file system
	std::string fullPath("wwwroot/" + htmlFile);
	char const *file = fullPath.c_str();
	std::ifstream f(file);

	// Check if it opened and if it did, grab the entire contents
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
		content = str;
		errorCode = 200;
	}

	f.close();
	errorCode = errorCode;

	// Write the document back to the client
	HTTPStatus status;
	this->requestHandler.sendResponse(status.OK.code, status.OK.description);
	this->requestHandler.sendHeader("Cache-Control", "no-cache, private");
	this->requestHandler.sendHeader("Content-Type", "text/html");
	this->requestHandler.sendHeader("Content-Length", content.size());
	this->requestHandler.endHeaders();
	this->requestHandler.writeContent(content);

	this->sendToClient(
		clientSocket, 
		this->requestHandler.headersBufferToString().c_str(), 
		this->requestHandler.headersBufferToString().size());
}

// Handler for client disconnections
void WebServer::onClientDisconnected(int clientSocket, int socketIndex, ssize_t nbytes)
{
	ATcpListener::onClientDisconnected(clientSocket, socketIndex, nbytes);
	std::cout << "Client disconnected: " << clientSocket << std::endl;
}