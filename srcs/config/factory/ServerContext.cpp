#include "ServerContext.hpp"

ServerContext::ServerContext(): AContext(NULL)
{
    this->SetParentContext(NULL);
    std::string allowedChars[] = {"{", "}", ";"};
    VectorUtils<std::string>::FillVectorFromArray(this->_allowedChars, allowedChars, sizeof(allowedChars) / sizeof(std::string));
    
    std::string subContexts[] = {"location"};
    AContextCreator * subContextsCreators[] = {new LocationContextCreator()};
    MapUtils<std::string, AContextCreator *>::FillMapFromArray(this->_allowedSubContexts, subContexts, subContextsCreators, sizeof(subContexts) / sizeof(std::string));
    
    std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "index", "autoindex"};
    ADirectiveCreator * directivesCreators[] = {new ListenDirectiveCreator(), new ServerNameDirectiveCreator(), new ErrorPageDirectiveCreator(), new ClientMaxBodySizeDirectiveCreator(), new RootDirectiveCreator(), new IndexDirectiveCreator(), new AutoIndexDirectiveCreator()};
    MapUtils<std::string, ADirectiveCreator *>::FillMapFromArray(this->_allowedDirectives, directives, directivesCreators, sizeof(directives) / sizeof(std::string));
}

ServerContext::ServerContext(ServerContext const &other) : AContext(other)
{
}

ServerContext::~ServerContext()
{
    MapUtils<std::string, AContextCreator *>::ClearMap(this->_allowedSubContexts);
    MapUtils<std::string, ADirectiveCreator *>::ClearMap(this->_allowedDirectives);
}

ServerContext &ServerContext::operator=(ServerContext const &other)
{
    AContext::operator=(other);
    return (*this);
}

bool ServerContext::ParseContext(std::string &content)
{
    std::string word;
    std::string serverContextName = "server";
    std::string::iterator it = content.begin();
    size_t pos = content.find_first_of("{");

    if (pos == std::string::npos)
        return (false);

    for (; it != content.end(); it++)
    {
        StringUtils::AdvaceOnWhiteSpace(it, content);
        word = StringUtils::ExtractWord(it, content, this->_allowedChars);
        Logger::debug(word, INFO,  "ServerContext::ParseContext word = ");
        if (word == serverContextName)
        {
            content = serverContextName + " " + content;
            return (true);
        }
        PairContextCreator *contextCreator = MapUtils<std::string, AContextCreator *>::SafeFindMap(this->_allowedSubContexts, word);
        if (contextCreator != NULL)
        {
            AContext *subContext = contextCreator->second->CreateContext();
            subContext->SetParentContext(this);
            this->AddSubContext(subContext);
            if (!subContext->ParseContext(content))
                throw std::runtime_error("Error: Context " + word + " failed to parse");
        }
        PairDirCreator *directiveCreator = MapUtils<std::string, ADirectiveCreator *>::SafeFindMap(this->_allowedDirectives, word);
        if (directiveCreator != NULL)
        {
            std::string line = StringUtils::ExtractLine(it, content);
            ADirective *directive = directiveCreator->second->CreateDirective();
            directive->SetParentContext(this);
            this->AddDirective(directive);
            if (!directive->ParseDirective(line))
                throw std::runtime_error("Error: Directive " + word + " failed to parse");
            directive->PrintDirective();
        }
        word.clear();
    } 
    content.erase(content.begin(), it);
    return (false);
}