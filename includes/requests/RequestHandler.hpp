#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "BaseHTTPRequestHandler.hpp"

class RequestHandler : public BaseHTTPRequestHandler {
    public:
		RequestHandler();
		RequestHandler(const RequestHandler& other);
		RequestHandler& operator=(const RequestHandler& other);
		void doGET();
		void doPOST();
		void doDELETE();

    private:
		//
};

#endif
