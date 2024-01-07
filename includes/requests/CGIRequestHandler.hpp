#ifndef CGIREQUESTHANDLER_HPP
#define CGIREQUESTHANDLER_HPP

#include <map>
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

    private:
		std::map<std::string, std::string> env;
		RequestContent *_requestContent;
		ServerConfig *_serverConfig;
		void setEnv();
		bool isCGI();
};

#endif
