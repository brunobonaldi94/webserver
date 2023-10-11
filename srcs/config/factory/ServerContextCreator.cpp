#include "ServerContextCreator.hpp"

ServerContextCreator::ServerContextCreator(): AContextCreator() {}

ServerContextCreator::ServerContextCreator(ServerContextCreator const & other): AContextCreator(other) 
{
    *this = other;
}

ServerContextCreator::~ServerContextCreator() {}

ServerContextCreator& ServerContextCreator::operator=(ServerContextCreator const & other)
{
    (void)other;
    return (*this);
}

AContext *ServerContextCreator::CreateContext() const
{
    std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "index", "autoindex"};
    std::vector<std::string> allowedDirectives(directives, directives + sizeof(directives) / sizeof(std::string));
    std::string subContexts[] = {"location"};
    std::vector<std::string> allowedSubContexts(subContexts, subContexts + sizeof(subContexts) / sizeof(std::string));
    return (new ServerContext(allowedDirectives, allowedSubContexts));
}