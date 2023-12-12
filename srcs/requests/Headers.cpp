#include "Headers.hpp"

Headers::Headers()
{
}
Headers::~Headers()
{
}
Headers::Headers(const Headers &other)
{
	*this = other;
}
Headers &Headers::operator=(const Headers &other)
{
	if (this != &other)
		this->headers = other.headers;
	return *this;
}

std::string Headers::getHeader(std::string key)
{
	std::string keyLower = StringUtils::LowerCase(key);
	std::pair<const std::string, std::string> *header = MapUtils<std::string, std::string>::SafeFindMap(this->headers, keyLower);
	if (header == NULL)
		return "";
	return this->headers[keyLower];
}

void Headers::setHeader(std::string key, std::string value)
{
	std::string keyLower = StringUtils::LowerCase(key);
	std::string valueTrim = StringUtils::Trim(value);
	this->headers[keyLower] = valueTrim;
}

std::map<std::string, std::string> Headers::getHeaders() const
{
	return this->headers;
}

void Headers::clearHeaders()
{
	this->headers.clear();
}

bool Headers::parseHeader(std::string header)
{
  std::vector<std::string> keyValue = StringUtils::SplitAtFirstDelimiter(header, ":");
	if (keyValue.size() < 2)
		return false;
	std::string key = keyValue[0];
	std::string value = keyValue[1];
	this->setHeader(key, value);
	return true;
}