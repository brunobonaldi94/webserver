#pragma once

#include <iostream>
#include <sys/types.h>
#include "defines.hpp"
#include "Headers.hpp"
#include "StringUtils.hpp"

struct MultiPartData
{
  MultiPartData();
  std::string name;
  std::string fileName;
  std::string contentDisposition;
  std::string contentType;
  std::string data;
  bool boundaryFound;
  bool dataStart;
  bool dataEnd;
  bool bodyParsed;
  bool headersParsed;
  bool parseHeaders(std::string line);
  bool parseBody(std::string line, ssize_t contentLengthNbr, std::string boundary);
  void setContentDisposition(std::string contentDisposition);
};

class Body
{
  public:
    Body();
    virtual ~Body();
    Body(const Body &other);
    Body &operator=(const Body &other);
    std::string getBody() const;
    void setBody(std::string body);
    void clearBody();
    void clear();
    void setBodyFullyRead(bool bodyFullyRead);
    bool getBodyFullyRead() const;
    virtual bool parseBody(std::string line, ssize_t contentLength);
    virtual bool parseMultiPartBody(std::string line, ssize_t contentLength, std::string boundary);
    MultiPartData getMultiPartData() const;
  private:
    std::string body;
    MultiPartData multiPartData;
    bool bodyFullyRead;
};
