#pragma once

#include "AContextCreator.hpp"
#include "LocationContext.hpp"

class LocationContextCreator : public AContextCreator
{
public:
    LocationContextCreator();
    LocationContextCreator(LocationContextCreator const & other);
    ~LocationContextCreator();

    LocationContextCreator& operator=(LocationContextCreator const & other);

    AContext *CreateContext() const;
};