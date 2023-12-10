#include "RequestContent.hpp"


RequestContent::RequestContent(){}

RequestContent::RequestContent(ServerConfig *serverConfig): serverConfig(serverConfig)
{

}

RequestContent::~RequestContent()
{

}

RequestContent::RequestContent(const RequestContent &other)
{
  *this = other;
}

RequestContent &RequestContent::operator=(const RequestContent &other)
{
  if (this != &other)
  {
    this->headers = other.headers;
    this->body = other.body;
    this->serverConfig = other.serverConfig;
  }
  return *this;
}

std::string RequestContent::getBody() const
{
  return this->body;
}

void RequestContent::setBody(std::string body)
{
  this->body = body;
}

Headers RequestContent::getHeaders() const
{
  return this->headers;
}

void RequestContent::setHeaders(Headers headers)
{
  this->headers = headers;
}

void RequestContent::setHeader(std::string key, std::string value)
{
  this->headers.setHeader(key, value);
}

std::string RequestContent::getHeader(std::string key)
{
  return this->headers.getHeader(key);
}

void RequestContent::clearHeaders()
{
  this->headers.clearHeaders();
}

void RequestContent::clearBody()
{
  this->body.clear();
}

void RequestContent::clear()
{
  this->clearHeaders();
  this->clearBody();
}

ServerConfig *RequestContent::getServerConfig()
{
  return this->serverConfig;
}

void RequestContent::addToBody(std::string body)
{
  this->body += body;
}

bool RequestContent::parseHeader(std::string header)
{
  std::vector<std::string> keyValue = StringUtils::SplitAtFirstDelimiter(header, ":");
	if (keyValue.size() < 2)
		return false;
	std::string key = keyValue[0];
	std::string value = keyValue[1];
	this->setHeader(key, value);
	return true;
}

bool RequestContent::parseBody(std::string line, ssize_t contentLengthNbr)
{
  size_t contentLength = static_cast<size_t>(contentLengthNbr);
	if (contentLengthNbr <= 0)
				return false;
  if (contentLength == this->getBody().size())
    return true;
	if (static_cast<size_t>(contentLengthNbr) < this->getBody().size() + line.size())
	{
		line = line.substr(0, contentLengthNbr - this->getBody().size());
		this->addToBody(line);
		return true;
	}
	this->addToBody(line);
  return true;
}