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
#include <string.h>

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
		RequestContent *_requestContent;
		std::map<std::string, std::string> env;
		char **envp;
		void setEnv();
		bool isCGI();
		char** stringVectorToArray(const std::vector<std::string>& inputStrings);
		char**  createEnvp(); 
};

#endif
