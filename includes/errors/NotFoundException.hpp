#pragma once

#include <iostream>
#include <exception>

class NotFoundException : public std::exception
{
private:
  std::string _objectName;
public:
  NotFoundException(const std::string& objectName);
  virtual ~NotFoundException() throw();
  virtual const char* what() const throw();
};
