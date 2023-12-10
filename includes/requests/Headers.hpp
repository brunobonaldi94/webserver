#pragma once

#include <iostream>
#include <map>
#include "StringUtils.hpp"
#include "MapUtils.hpp"

class Headers
{
	private:	
		std::map<std::string, std::string> headers;
	public:
		Headers();
		~Headers();
		Headers(const Headers &other);
		Headers &operator=(const Headers &other);
		std::string getHeader(std::string key);
		void setHeader(std::string key, std::string value);
    std::map<std::string, std::string> getHeaders() const;
		void clearHeaders();
		bool parseHeader(std::string header);
};

