#ifndef CGIREQUESTHANDLER_HPP
#define CGIREQUESTHANDLER_HPP

#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <vector>
#include "StringUtils.hpp"
#include "RequestContent.hpp"
#include "ServerConfig.hpp"

class CGIRequestHandler
{
    public:
		CGIRequestHandler(RequestContent *RequestContent);
		~CGIRequestHandler();
		CGIRequestHandler(const CGIRequestHandler& other);
		CGIRequestHandler& operator=(const CGIRequestHandler& other);

		void execute();
		std::string response();

    private:
		std::string content;
		std::map<std::string, std::string> env;
		RequestContent *_requestContent;
		ServerConfig *_serverConfig;
		void setEnv();
		bool isCGI();
		char** stringVectorToArray(const std::vector<std::string>& inputStrings);
};

#endif
