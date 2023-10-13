#include "LocationContext.hpp"

LocationContext::LocationContext(): AContext()
{
    std::string contexts[] = {"server", "location"};
    AContextCreator* contextsCreators[] = {NULL, NULL};
    MapUtils<std::string, AContextCreator *>::FillMapFromArray(this->_allowedSubContexts, contexts, contextsCreators, sizeof(contexts) / sizeof(std::string));
    std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "index", "autoindex", "limit_except"};
    ADirectiveCreator * directivesCreators[] = {NULL, NULL, new ErrorPageDirectiveCreator(), new ClientMaxBodySizeDirectiveCreator(), new RootDirectiveCreator(), new IndexDirectiveCreator(), new AutoIndexDirectiveCreator(), new LimitExceptDirectiveCreator()};
    MapUtils<std::string, ADirectiveCreator *>::FillMapFromArray(this->_allowedDirectives, directives, directivesCreators, sizeof(directives) / sizeof(std::string));
};

LocationContext::LocationContext(LocationContext const &other): AContext(other) {};

LocationContext::~LocationContext()
{
    MapUtils<std::string, ADirectiveCreator *>::ClearMap(this->_allowedDirectives);
    MapUtils<std::string, AContextCreator *>::ClearMap(this->_allowedSubContexts);
};

void LocationContext::SetUri(std::string uri)
{
    this->uri = uri;
}

std::string LocationContext::GetUri() const
{
    return (this->uri);
}

bool LocationContext::ParseContext(std::string &content)
{
    std::string::iterator it = content.begin();
    std::string word;
    bool locationHasSlash = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '/');
    if (!locationHasSlash)
        return (false);

    std::string uri = StringUtils::ExtractWord(it, content, this->_allowedChars);
    this->SetUri(uri);

    bool locationHasOpenCurlyBraces = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '{');
    if (!locationHasOpenCurlyBraces)
        return (false);

    for (; it != content.end(); it++)
    {
        StringUtils::AdvaceOnWhiteSpace(it, content);
        if (*it == '{')
        {
            throw SyntaxErrorException("Duplicated {");
            return (false);
        }
        word = StringUtils::ExtractWord(it, content, this->_allowedChars);
        PairContextCreator *contextCreator = MapUtils<std::string, AContextCreator *>::SafeFindMap(this->_allowedSubContexts, word);
        if (contextCreator && contextCreator->second == NULL)
            throw NotAllowedException("Context " + word + " not allowed in location context");
        PairDirCreator *directiveCreator = MapUtils<std::string, ADirectiveCreator *>::SafeFindMap(this->_allowedDirectives, word);
        if (directiveCreator && directiveCreator->second == NULL)
            throw NotAllowedException("Directive " + word + " not allowed in location context");
        if (directiveCreator != NULL)
        {
            std::string line = StringUtils::ExtractLine(it, content);
            ADirective *directive = directiveCreator->second->CreateDirective();
            directive->SetParentContext(this);
            this->AddDirective(directive);
            if (!directive->ParseDirective(line))
                throw SyntaxErrorException("Error: Directive " + word + " failed to parse");
            directive->PrintDirective();
        }
    }
    content.erase(content.begin(), it);
    return (false);
}