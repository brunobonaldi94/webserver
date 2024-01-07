#ifndef CGIREQUESTHANDLER_HPP
#define CGIREQUESTHANDLER_HPP

#include <map>
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

		std::string execute();

    private:
		RequestContent *_requestContent;
		std::map<std::string, std::string> env;
		char **envp;
		void setEnv();
		bool isCGI();
		char**  createEnvp(); 
};

#endif
