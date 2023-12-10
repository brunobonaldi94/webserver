#pragma once

#include <iostream>
#include <map>
#include "Headers.hpp"
#include "ServerConfig.hpp"

class RequestContent
{
	public:
    RequestContent();
		RequestContent(ServerConfig *serverConfig);
		~RequestContent();
		RequestContent(const RequestContent &other);
		RequestContent &operator=(const RequestContent &other);
		std::string getBody() const;
		void setBody(std::string body);
		Headers getHeaders() const;
		void setHeaders(Headers headers);
    void setHeader(std::string key, std::string value);
    std::string getHeader(std::string key);
    void clearHeaders();
    void clearBody();
    void clear();
    ServerConfig *getServerConfig();
		void addToBody(std::string body);
		bool parseHeader(std::string header);
		bool parseBody(std::string line, ssize_t contentLength);

	private:
		Headers headers;
		std::string body;
    ServerConfig *serverConfig;
};

