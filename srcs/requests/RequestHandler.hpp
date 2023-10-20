#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>

class BaseHTTPRequestHandler {
    public:
    
    protected:
		void send_response(int statusCode, std::string message);
		void send_header(std::string key, std::string value);
		void send_error();

    private:
		std::ostringstream headers_buffer;

};

class RequestHandler : public BaseHTTPRequestHandler {
    public:
		void doGET();
		void doPOST();
		void doDELETE();

    private:

};

#endif
