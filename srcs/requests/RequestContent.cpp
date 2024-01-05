#include "RequestContent.hpp"


RequestContent::RequestContent(): headersFullyRead(false), hasMultiPartFormData(false), hasChunkedBody(false), hasErrorInRequest(false)
{

}

RequestContent::RequestContent(ServerConfig *serverConfig): serverConfig(serverConfig), headersFullyRead(false), hasMultiPartFormData(false), hasChunkedBody(false), hasErrorInRequest(false)
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
    this->headersFullyRead = other.headersFullyRead;
    this->hasMultiPartFormData = other.hasMultiPartFormData;
    this->boundary = other.boundary;
    this->hasChunkedBody = other.hasChunkedBody;
    this->hasErrorInRequest = other.hasErrorInRequest;
  }
  return *this;
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
  this->headersFullyRead = false;
}

void RequestContent::clear()
{
  this->clearHeaders();
  this->body.clear();
  this->boundary.clear();
  this->hasMultiPartFormData = false;
  this->hasChunkedBody = false;
  this->hasErrorInRequest = false;
}

ServerConfig *RequestContent::getServerConfig()
{
  return this->serverConfig;
}

bool RequestContent::parseHeader(std::string header)
{
  return this->headers.parseHeader(header);
}

bool RequestContent::parseBody(std::string line, ssize_t contentLengthNbr)
{
  return this->body.parseBody(line, contentLengthNbr);
}

bool RequestContent::hasParsedAllRequest()
{
  bool hasBody = this->getHeader("Content-Length") != "";
  bool requestWithBodyFullyRead = this->body.getBodyFullyRead() && hasBody && this->headersFullyRead;
  bool requestWithoutBodyFullyRead = !hasBody && this->headersFullyRead;
  return requestWithBodyFullyRead || requestWithoutBodyFullyRead;
}

void RequestContent::setHeadersFullyRead(bool headersFullyRead)
{
  this->headersFullyRead = headersFullyRead;
}

void RequestContent::setBodyFullyRead(bool bodyFullyRead)
{
  this->body.setBodyFullyRead(bodyFullyRead);
}

std::string RequestContent::getBody() const
{
  return this->body.getBody();
}

bool RequestContent::isMultiPartFormData()
{
  if (this->hasMultiPartFormData)
    return true;
  std::string contentType = this->headers.getHeader("Content-Type");
  this->hasMultiPartFormData = contentType.find("multipart/form-data") != std::string::npos;
  if (hasMultiPartFormData == false)
    return false;
  size_t boundaryPos = contentType.find(BOUNDARY);
  if (boundaryPos == std::string::npos && this->hasMultiPartFormData)
    throw std::runtime_error("No boundary found in Content-Type");
  this->boundary = contentType.substr(boundaryPos + std::string(BOUNDARY).size());
  return this->hasMultiPartFormData;
}

MultiPartData RequestContent::getMultiPartData() const
{
  return this->body.getMultiPartData();
}

bool RequestContent::parseMultiPartBody(std::string line, ssize_t contentLengthNbr)
{
  return this->body.parseMultiPartBody(line, contentLengthNbr, this->boundary);
}

bool RequestContent::getHasMultiPartFormData() const
{
  return this->hasMultiPartFormData;
}

std::string RequestContent::getBoundary() const
{
  return this->boundary;
}

Body &RequestContent::getBodyObject()
{
  return this->body;
}

bool RequestContent::isChunkedBody()
{
  if (this->hasChunkedBody)
    return true;
  this->hasChunkedBody = this->headers.getHeader("Transfer-Encoding") == "chunked";
  return this->hasChunkedBody;
}

bool RequestContent::parseChunkedBody(std::string line)
{
  return this->body.parseChunkedBody(line);
}

bool RequestContent::getHasErrorInRequest() const
{
  return this->hasErrorInRequest;
}

void RequestContent::setHasErrorInRequest(bool hasErrorInRequest)
{
  this->hasErrorInRequest = hasErrorInRequest;
}