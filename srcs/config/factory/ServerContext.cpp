#include "ServerContext.hpp"

ServerContext::ServerContext(): AContext(NULL, "server")
{
    this->SetParentContext(NULL);
    
    std::string subContexts[] = {"server", "location"};
    AContextCreator * subContextsCreators[] = {NULL, new LocationContextCreator()};
    MapUtils<std::string, AContextCreator *>::FillMapFromArray(this->_allowedSubContexts, subContexts, subContextsCreators, sizeof(subContexts) / sizeof(std::string));
    std::string directives[] = {"listen", "server_name", "error_page", "client_max_body_size", "root", "index", "autoindex", "cgi", "return"};
    ADirectiveCreator * directivesCreators[] = {new ListenDirectiveCreator(), new ServerNameDirectiveCreator(), new ErrorPageDirectiveCreator(), new ClientMaxBodySizeDirectiveCreator(), new RootDirectiveCreator(), new IndexDirectiveCreator(), new AutoIndexDirectiveCreator(), new CgiDirectiveCreator(), NULL};
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

void ServerContext::ParseContext(std::string &content)
{
    std::string word;
    bool locationHasCloseCurlyBraces = false;
    std::string::iterator it = content.begin();

    bool serverHasOpenCurlyBraces = StringUtils::CheckNextCharAfterWhiteSpace(it, content, '{');
    if (!serverHasOpenCurlyBraces)
        throw SyntaxErrorException("Server context must have an open curly braces `{`");

    for (; it != content.end(); it++)
    {
        if (*it == '{')
            throw SyntaxErrorException("Duplicated {");
        if (*it == '}')
        {
            it++;
            locationHasCloseCurlyBraces = true;
            break;
        }
        StringUtils::AdvaceOnWhiteSpace(it, content);
        StringUtils::AdvanceOnComment(it, content);
        word = StringUtils::ExtractWord(it, content);
        AContext::HandleContextCreation(content, word, "server");
        AContext::HandleDirectiveCreation(it, content, word, "server");
        word.clear();
    }
    if (!locationHasCloseCurlyBraces)
        throw SyntaxErrorException("Server context must have a close curly braces `}`");
    content.erase(content.begin(), it);
}

void ServerContext::PrintContext()
{
    AContext::PrintContext();
}

void ServerContext::FillDefaultValues()
{
    AContext::FillDefaultValuesDirectives();
    MapContexts subContexts = this->GetSubContexts();
    for (MapContexts::iterator it = subContexts.begin(); it != subContexts.end(); ++it)
    {
        std::vector<AContext *> contexts = it->second;
        for (std::vector<AContext *>::iterator it2 = contexts.begin(); it2 != contexts.end(); ++it2)
            (*it2)->FillDefaultValues();
    }
}
