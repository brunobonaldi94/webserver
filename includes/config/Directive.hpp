#pragma once

#include <iostream>
#include "Context.hpp"

class Context;

class Directive
{
private:
  Context*    _context;
  std::string _name;
  std::string _value;
  std::string _endChar;
  
public:
    Directive();
    Directive(Directive const & other);
    ~Directive();

    Directive& operator=(Directive const & other);

};