#pragma once

#include "AContext.hpp"

class LocationContext : public AContext
{
  public:
    LocationContext(std::vector<std::string> allowedDirectives, std::vector<std::string> allowedSubContexts, AContext* parentContext);
    LocationContext(LocationContext const &other);
    ~LocationContext();
    bool ParseContext(std::string &content);

};