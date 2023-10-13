#pragma once

#include "AContext.hpp"
#include "StringUtils.hpp"
#include "VectorUtils.hpp"
#include "MapUtils.hpp"
#include "DirectivesCreator.hpp"
#include "NotAllowedException.hpp"
#include "NotFoundException.hpp"
#include "SyntaxErrorException.hpp"

class LocationContext : public AContext
{

private:
    std::string uri;
public:
    LocationContext();
    LocationContext(LocationContext const & other);
    ~LocationContext();

    LocationContext& operator=(LocationContext const & other);

    void SetUri(std::string uri);
    std::string GetUri() const;

    bool ParseContext(std::string &content);
};