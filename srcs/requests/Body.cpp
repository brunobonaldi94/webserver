#include "Body.hpp"

MultiPartData::MultiPartData(): boundaryFound(false), dataStart(false), dataEnd(false), bodyParsed(false), headersParsed(false)
{}

void MultiPartData::setContentDisposition(std::string contentDisposition)
{
  size_t formDataLen =  std::string(FORM_DATA).size();
  std::string startsWithFormData = contentDisposition.substr(0, formDataLen);
  if (startsWithFormData != FORM_DATA)
    throw std::runtime_error("Invalid Content-Disposition header");
  this->contentDisposition = contentDisposition;
  std::vector<std::string> splittedContentDisposition = StringUtils::Split(this->contentDisposition.substr(formDataLen + 2), ";");
  for (size_t i = 0; i < splittedContentDisposition.size(); i++)
  {
    std::vector<std::string> splittedContentDispositionKeyValue = StringUtils::SplitAtFirstDelimiter(StringUtils::TrimLeft(splittedContentDisposition[i]), "=");
    if (splittedContentDispositionKeyValue.size() < 2)
      throw std::runtime_error("Invalid Content-Disposition header");
    std::string key = splittedContentDispositionKeyValue[0];
    std::string value = splittedContentDispositionKeyValue[1];
    if (key == "name")
      this->name = value;
    else if (key == "filename")
      this->fileName = value;
  }
}

bool MultiPartData::parseHeaders(std::string line)
{
  Headers headers;
  bool headersParsed = false;

  headersParsed = !this->contentDisposition.empty() && !this->contentType.empty();
  if (headersParsed)
    return true;
  headers.parseHeader(line);
  std::string headerContentDisposition = headers.getHeader("Content-Disposition");
  if (headerContentDisposition != "")
  {
    this->setContentDisposition(headerContentDisposition);
    return true;
  }
  if (headers.getHeader("Content-Type") != "")
  {
    this->contentType = headers.getHeader("Content-Type");
    return true;
  }
  return !this->contentDisposition.empty() && !this->contentType.empty();;
}

bool MultiPartData::parseBody(std::string line, ssize_t contentLengthNbr, std::string boundary)
{
   size_t contentLength = static_cast<size_t>(contentLengthNbr);
    if (contentLengthNbr <= 0)
          return false;
    if (contentLength == this->data.size())
    {
      this->bodyParsed = true;
      this->data.erase(this->data.size() - 1);
      return true;
    }
    if (line == "--" + boundary + "--")
    {
      this->dataEnd = true;
      this->bodyParsed = true;
      this->data.erase(this->data.size() - 1);
      return true;
    }
    if (static_cast<size_t>(contentLengthNbr) < this->data.size() + line.size())
    {
      line = line.substr(0, contentLengthNbr - this->data.size());  
      StringUtils::AddToString(this->data, line, false);
      this->bodyParsed = true;
      return true;
    }
    StringUtils::AddToString(this->data, line);
    return true;
}

Body::Body(): bodyFullyRead(false)
{}

Body::~Body()
{}

Body::Body(const Body &other)
{
  *this = other;
}

Body &Body::operator=(const Body &other)
{
  if (this != &other)
  {
    this->body = other.body;
  }
  return *this;
}

std::string Body::getBody() const
{
  return this->body;
}

void Body::setBody(std::string body)
{
  this->body = body;
}

void Body::clearBody()
{
  this->body.clear();
  this->multiPartData = MultiPartData();
}

void Body::clear()
{
  this->clearBody();
}

void Body::setBodyFullyRead(bool bodyFullyRead)
{
  this->bodyFullyRead = bodyFullyRead;
}

bool Body::parseBody(std::string line, ssize_t contentLengthNbr)
{
  size_t contentLength = static_cast<size_t>(contentLengthNbr);
	if (contentLengthNbr <= 0)
				return false;
  if (contentLength == this->getBody().size())
  {
    this->bodyFullyRead = true;
    this->body.erase(this->body.size() - 1);
    return true;
  }
	if (static_cast<size_t>(contentLengthNbr) < this->getBody().size() + line.size())
	{
		line = line.substr(0, contentLengthNbr - this->getBody().size());
		StringUtils::AddToString(this->body, line, false);
    this->bodyFullyRead = true;
		return true;
	}
	StringUtils::AddToString(this->body, line);
  return true;
}

bool Body::parseBody(std::string line, ssize_t contentLength, std::string boundary)
{
  if (line.empty() && !this->multiPartData.boundaryFound)
    return true;
  if (!this->multiPartData.boundaryFound)
  {
    if (line.find("--" + boundary) != std::string::npos)
    {
      this->multiPartData.boundaryFound = true;
      return true;
    }
  }
  if (!this->multiPartData.boundaryFound)
    return false;
  if (line.empty())
  {
    this->multiPartData.dataStart = true;
    if (this->multiPartData.dataStart && !this->multiPartData.headersParsed)
      throw std::runtime_error("Invalid multipart/form-data");
    return true;
  }
  this->multiPartData.headersParsed = this->multiPartData.parseHeaders(line);
  if (this->multiPartData.dataStart && this->multiPartData.headersParsed)
    this->multiPartData.parseBody(line, contentLength, boundary);
  if (this->multiPartData.bodyParsed)
  {
    this->bodyFullyRead = true;
    this->body = this->multiPartData.data;
    return true;
  }
  return true;
}

bool Body::getBodyFullyRead() const
{
  return this->bodyFullyRead;
}