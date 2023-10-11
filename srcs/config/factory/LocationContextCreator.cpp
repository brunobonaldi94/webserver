#include "LocationContextCreator.hpp"

LocationContextCreator::LocationContextCreator() {}

LocationContextCreator::~LocationContextCreator() {}

AContext *LocationContextCreator::CreateContext() const
{
    std::string directives[] = {"error_page", "root", "index", "autoindex", "limit_except"};
    std::vector<std::string> allowedDirectives(directives, directives + sizeof(directives) / sizeof(std::string));
    std::string subContexts[] = {"location", "server"};
    std::vector<std::string> allowedSubContexts(subContexts, subContexts + sizeof(subContexts) / sizeof(std::string));
    return (new LocationContext(allowedDirectives, allowedSubContexts, NULL));
}
