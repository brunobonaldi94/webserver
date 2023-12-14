#pragma once

#include <iostream>
#include <map>
#include "Headers.hpp"
#include "Body.hpp"
#include "ServerConfig.hpp"

class RequestContent
{
	public:
    RequestContent();
		RequestContent(ServerConfig *serverConfig);
		~RequestContent();
		RequestContent(const RequestContent &other);
		RequestContent &operator=(const RequestContent &other);
		Headers getHeaders() const;
		void setHeaders(Headers headers);
    void setHeader(std::string key, std::string value);
    std::string getHeader(std::string key);
    void clearHeaders();
    void clear();
    ServerConfig *getServerConfig();
		bool parseHeader(std::string header);
		bool hasParsedAllRequest();
		void setHeadersFullyRead(bool headersFullyRead);
		void setBodyFullyRead(bool bodyFullyRead);
		bool parseBody(std::string line, ssize_t contentLengthNbr);
		std::string getBody() const;
		bool isMultiPartFormData();
		MultiPartData getMultiPartData() const;
		bool parseMultiPartBody(std::string line, ssize_t contentLengthNbr);

	private:
		Headers headers;
		Body body;
		std::string boundary;
    ServerConfig *serverConfig;
		bool headersFullyRead;
};

