#ifndef BASE_HTT_PREQUES_THANDLER_HPP
#define BASE_HTT_PREQUES_THANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include "HTTPStatus.hpp"

class BaseHTTPRequestHandler {
    public:
		void sendResponse(int statusCode, std::string message);
		template <typename T>
		void sendHeader(std::string key, T value);
		void endHeaders();
		void sendError();
		void writeContent(const std::string content);
		const std::string headersBufferToString() const;
	
	protected:

    private:
		std::ostringstream headersBuffer;
		size_t _size;

};

template <typename T>
void BaseHTTPRequestHandler::sendHeader(std::string key, T value) {
	this->headersBuffer <<  key << ": " << value << std::endl;
}
#endif
