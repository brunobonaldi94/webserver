#ifndef BASE_HTT_PREQUES_THANDLER_HPP
#define BASE_HTT_PREQUES_THANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>

class BaseHTTPRequestHandler {
    public:
		void sendResponse(int statusCode, std::string message);
		void sendHeader(std::string key, std::string value);
		void sendError();
		const std::string headersBufferToString() const;

    private:
		std::ostringstream headersBuffer;

};

#endif
