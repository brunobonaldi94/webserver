#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <dirent.h>
#include "BaseHTTPRequestHandler.hpp"

class RequestHandler : public BaseHTTPRequestHandler {
    public:
		RequestHandler();
		RequestHandler(const RequestHandler& other);
		RequestHandler& operator=(const RequestHandler& other);

		void doGET();
		void doPOST();
		void doDELETE();

		void sendJsonResponse(std::string json);

    private:
		std::vector<std::string> getFiles(const std::string& path);
};

#endif
