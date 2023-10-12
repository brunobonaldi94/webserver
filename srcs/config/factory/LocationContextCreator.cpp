#include "LocationContextCreator.hpp"

LocationContextCreator::LocationContextCreator(): AContextCreator() {}

LocationContextCreator::LocationContextCreator(LocationContextCreator const & other): AContextCreator(other) 
{
    *this = other;
}

LocationContextCreator::~LocationContextCreator() {}

LocationContextCreator& LocationContextCreator::operator=(LocationContextCreator const & other)
{
    (void)other;
    return (*this);
}

AContext *LocationContextCreator::CreateContext() const
{
    return (new LocationContext());
}