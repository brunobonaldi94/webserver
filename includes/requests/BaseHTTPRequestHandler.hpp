#ifndef BASE_HTT_PREQUES_THANDLER_HPP
#define BASE_HTT_PREQUES_THANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include<map>
#include <iterator> 
#include <stdlib.h>
#include <stdexcept>
#include "StringUtils.hpp"
#include "HTTPStatus.hpp"
#include "ServerConfig.hpp"
#include "Headers.hpp"
#include "dirent.h"

class BaseHTTPRequestHandler {
    public:
		BaseHTTPRequestHandler();
		virtual ~BaseHTTPRequestHandler();

		typedef void (BaseHTTPRequestHandler::*RequestMethodFunction)(void);
		
		const std::string headersBufferToString() const;
		RequestMethodFunction getMethod(const std::string& method);
		RequestMethodFunction parseRequest(const char* request);
		void clearHeadersBuffers();
		ServerConfig *getServerConfig() const;
		void setServerConfig(ServerConfig *serverConfig);
	
		virtual void doGET() = 0;
		virtual void doPOST() = 0;
		virtual void doDELETE() = 0;

	protected:
		void sendResponse(int statusCode, std::string message);
		template <typename T>
		void sendHeader(std::string key, T value);
		void endHeaders();
		void sendError(const std::string& content, const StatusCode& status);
		void sendNotFoundError();
		void writeContent(const std::string content);
		void setRequestLines(const std::vector<std::string> requestLines);
		std::vector<std::string> getMethodsAllowed() const;
		std::string readContent(const std::string path);
		std::string getContent(const std::string path);
	  std::string GetPath() const;
		std::vector<std::string> getMethodsAllowedForApi() const;
		void parseHeaders(std::vector<std::string> headers);
		std::vector<std::string> SplitRequest(const char* request);
		bool checkBodyLimit();
		std::string createDirectoryListing(LocationConfig *location, std::string path);
		bool validateServerName();
		bool checkRedirect();
		bool isDirectoryListingAllowed(std::string path);
		bool isInDirectory(std::string path, std::string directory);
		bool contentNotFound;

    private:
		std::ostringstream headersBuffer;
		std::string body;
		std::string requestMethod;
		std::string path;
		std::string requestVersion;
		ServerConfig *serverConfig;
		bool allowDirectoryListing;
		std::string directoryListingPath;
		Headers headers;
};

template <typename T>
void BaseHTTPRequestHandler::sendHeader(std::string key, T value) {
	this->headersBuffer <<  key << ": " << value << CRLF;
}
#endif
