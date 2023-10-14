#include "ServerContext.hpp"

ServerContext::ServerContext(): AContext(NULL)
{
    this->SetParentContext(NULL);
    std::string allowedChars[] = {"{", "}", ";"};
    VectorUtils<std::string>::FillVectorFromArray(this->_allowedChars, allowedChars, sizeof(allowedChars) / sizeof(std::string));
    
    std::string subContexts[] = {"server", "location"};
    AContextCreator * subContextsCreators[] = {NULL, new LocationContextCreator()};
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

    bool serverHasOpenCurlyBraces = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '{');
    if (!serverHasOpenCurlyBraces)
        return (false);

    for (; it != content.end(); it++)
    {
        if (*it == '{')
        {
            throw SyntaxErrorException("Duplicated {");
            return (false);
        }
        if (*it == '}')
        {
            it++;
            break;
        }
        StringUtils::AdvaceOnWhiteSpace(it, content);
        word = StringUtils::ExtractWord(it, content);
        Logger::debug(word, INFO,  "ServerContext::ParseContext word = ");
        AContext::HandleContextCreation(content, word);
        AContext::HandleDirectiveCreation(it, content, word);
        word.clear();
    }
    content.erase(content.begin(), it);
    return (true);
}