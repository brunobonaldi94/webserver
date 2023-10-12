#pragma once

#include "AContext.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"
#include "MapUtils.hpp"
#include "DirectivesCreator.hpp"

class LocationContext : public AContext
{
public:
    LocationContext();
    LocationContext(LocationContext const & other);
    ~LocationContext();

    LocationContext& operator=(LocationContext const & other);
    bool ParseContext(std::string &content);
};