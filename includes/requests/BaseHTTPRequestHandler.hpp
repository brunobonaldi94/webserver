#ifndef BASE_HTT_PREQUES_THANDLER_HPP
#define BASE_HTT_PREQUES_THANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <vector>
#include <iterator> 
#include <stdlib.h>
#include <stdexcept>
#include "StringUtils.hpp"
#include "HTTPStatus.hpp"

class BaseHTTPRequestHandler {
    public:
		const std::string headersBufferToString() const;
		bool parseRequest(const char* request);
		void clearHeadersBuffers();
	
	protected:
		void sendResponse(int statusCode, std::string message);
		template <typename T>
		void sendHeader(std::string key, T value);
		void endHeaders();
		void sendError(const std::string& content, const StatusCode& status);
		void writeContent(const std::string content);
		void setRequestLines(const std::vector<std::string> requestLines);
		std::string getContent(const std::string path, bool& foundContent);
	  std::string GetPath() const;

    private:
		std::ostringstream headersBuffer;

		std::string requestMethod;
		std::string path;
		std::string requestVersion;
};

template <typename T>
void BaseHTTPRequestHandler::sendHeader(std::string key, T value) {
	this->headersBuffer <<  key << ": " << value << std::endl;
}
#endif
