#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "BaseHTTPRequestHandler.hpp"

class RequestHandler : public BaseHTTPRequestHandler {
    public:
		void doGET();
		void doPOST();
		void doDELETE();

    private:
		//
};

#endif
