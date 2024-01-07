#ifndef CGIREQUESTHANDLER_HPP
#define CGIREQUESTHANDLER_HPP

#include <map>
#include "BaseHTTPRequestHandler.hpp"

class CGIRequestHandler : public BaseHTTPRequestHandler
{
    public:
		CGIRequestHandler(ADirectoryHandler *directoryHandler);
		~CGIRequestHandler();
		CGIRequestHandler(const CGIRequestHandler& other);
		CGIRequestHandler& operator=(const CGIRequestHandler& other);

		void execute();
		void doGET();
		void doPOST();

    private:
		std::map<std::string, std::string> env;
		bool isCGI();
};

#endif
