#include "LocationContext.hpp"

LocationContext::LocationContext(): AContext()
{
    std::string contexts[] = {"server", "location"};
    AContextCreator* contextsCreators[] = {NULL, NULL};
    MapUtils<std::string, AContextCreator *>::FillMapFromArray(this->_allowedSubContexts, contexts, contextsCreators, sizeof(contexts) / sizeof(std::string));
    std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "index", "autoindex", "limit_except", "cgi"};
    ADirectiveCreator * directivesCreators[] = {NULL, NULL, new ErrorPageDirectiveCreator(), new ClientMaxBodySizeDirectiveCreator(), new RootDirectiveCreator(), new IndexDirectiveCreator(), new AutoIndexDirectiveCreator(), new LimitExceptDirectiveCreator(), NULL};
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

void LocationContext::ParseContext(std::string &content)
{
    std::string::iterator it = content.begin();
    std::string word;
    bool locationHasCloseCurlyBraces = false;
    bool locationHasSlash = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '/');
    if (!locationHasSlash)
        throw SyntaxErrorException("Location context must have a slash `/` for uri");

    std::string uri = StringUtils::ExtractWord(it, content);
    this->SetUri("/" + uri);

    bool locationHasOpenCurlyBraces = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '{');
    if (!locationHasOpenCurlyBraces)
        throw SyntaxErrorException("Location context must have an open curly braces `{`");

    for (; it != content.end(); it++)
    {
        StringUtils::AdvaceOnWhiteSpace(it, content);
        if (*it == '{')
            throw SyntaxErrorException("Duplicated {");
        if (*it == '}')
        {
            it++;
            locationHasCloseCurlyBraces = true;
            break;
        }
        word = StringUtils::ExtractWord(it, content);
        Logger::Debug("LocationContext::ParseContext word = ", INFO, word);
        AContext::HandleContextCreation(content, word, "location");
        AContext::HandleDirectiveCreation(it, content, word, "location");
    }
    if (!locationHasCloseCurlyBraces)
        throw SyntaxErrorException("Location context must have a close curly braces `}`");
    content.erase(content.begin(), it);
}