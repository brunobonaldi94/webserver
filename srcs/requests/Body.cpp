#include "Body.hpp"

MultiPartData::MultiPartData(): bodyParsed(false), headersParsed(false)
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
      this->name = StringUtils::Trim(value, "\"");
    else if (key == "filename")
      this->fileName = StringUtils::Trim(value, "\"");
  }
}

bool MultiPartData::parseHeaders(std::vector<std::string> &headers)
{
  for (size_t i = 0; i < headers.size(); i++)
  {
    Headers header;
    std::string currentHeader = headers[i];
    header.parseHeader(currentHeader);
    std::string headerContentDisposition = header.getHeader("Content-Disposition");
    if (!headerContentDisposition.empty())
    {
      this->setContentDisposition(headerContentDisposition);
      continue;
    }
    else if (header.getHeader("Content-Type") != "")
    {
      this->contentType = header.getHeader("Content-Type");
      continue;
    }
  }
  return !this->contentDisposition.empty() && !this->contentType.empty();;
}

bool MultiPartData::parseBody(std::string line, ssize_t contentLengthNbr, std::string boundary)
{
  size_t contentLength = static_cast<size_t>(contentLengthNbr);
  std::string boundaryWithTwoDashes = "--" + boundary + "--";
  size_t boundaryPos = line.find(boundaryWithTwoDashes);
  if (boundaryPos != std::string::npos)
  {
    size_t substPos = boundaryPos - std::string(CRLF).size() - std::string("\n").size();
    line = line.substr(0, substPos);
    this->bodyParsed = true;
  }
  else
    this->bodyParsed = false;
  if (contentLengthNbr <= 0)
    throw std::runtime_error("Invalid Content-Length header");
  if (line.size() > contentLength)
  {
    line = line.substr(0, contentLengthNbr);  
    StringUtils::AddToString(this->data, line, false);
    this->bodyParsed = true;
    return this->bodyParsed;
  }
  StringUtils::AddToString(this->data, line);
  return this->bodyParsed;
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

bool Body::parseBody(std::string bodyLines, ssize_t contentLengthNbr)
{
	if (contentLengthNbr <= 0)
				return false;
  size_t contentLength = static_cast<size_t>(contentLengthNbr);
	if (bodyLines.size() > contentLength)
	{
		bodyLines = bodyLines.substr(0, contentLengthNbr);
		StringUtils::AddToString(this->body, bodyLines, false);
    this->bodyFullyRead = true;
		return true;
	}
  StringUtils::AddToString(this->body, bodyLines);
  this->bodyFullyRead = true;
  return true;
}

bool Body::parseMultiPartBody(std::string multiPartBody, ssize_t contentLength, std::string boundary)
{
  std::string line;
  std::vector<std::string> multiPartHeaders;
  std::string boundaryWithTwoDashes = "--" + boundary;
  size_t boundaryPos = multiPartBody.find_first_of(boundaryWithTwoDashes);
  bool bodyStart = false;
  if (boundaryPos == std::string::npos)
    return false;
  multiPartBody = multiPartBody.substr(boundaryPos + boundaryWithTwoDashes.size() + std::string(CRLF).size());
  std::istringstream iss(multiPartBody);
  while (std::getline(iss, line))
	{
		if (!line.empty() && line[line.size() - 1] == CR)
		{
			if (line.size() == 1)
				bodyStart = true;
			line.erase(line.size() - 1);
		}
		if (bodyStart)
			break;
		else
			multiPartHeaders.push_back(line);
	}
  this->multiPartData.headersParsed = this->multiPartData.parseHeaders(multiPartHeaders);
  if (!this->multiPartData.headersParsed)
    return false;
	std::string multiPartBodyLines((std::istreambuf_iterator<char>(iss)), std::istreambuf_iterator<char>());
  this->multiPartData.bodyParsed = this->multiPartData.parseBody(multiPartBodyLines, contentLength, boundary);
  this->bodyFullyRead = this->multiPartData.bodyParsed;
  this->body = this->multiPartData.data;
  return true;
}

bool Body::getBodyFullyRead() const
{
  return this->bodyFullyRead;
}

MultiPartData Body::getMultiPartData() const
{
  return this->multiPartData;
}
