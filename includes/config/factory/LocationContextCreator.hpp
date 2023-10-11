#pragma once

#include "AContextCreator.hpp"

class LocationContextCreator : public AContextCreator
{
public:
    LocationContextCreator();
    LocationContextCreator(LocationContextCreator const & other);
    ~LocationContextCreator();

    LocationContextCreator& operator=(LocationContextCreator const & other);

    AContext *createContext(std::string const &name) const;
};