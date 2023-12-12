#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include <dirent.h>
#include "BaseHTTPRequestHandler.hpp"

class RequestHandler : public BaseHTTPRequestHandler
{
    public:
		RequestHandler(ADirectoryHandler *directoryHandler);
		~RequestHandler();
		RequestHandler(const RequestHandler& other);
		RequestHandler& operator=(const RequestHandler& other);

		void doGET();
		void doPOST();
		void doDELETE();
		void clearRequestContent(int clientSocket);
		void sendJsonResponse(std::string json);

    private:
		std::string renderTemplate(const std::string& templateStr, const std::string& value);
};

#endif
