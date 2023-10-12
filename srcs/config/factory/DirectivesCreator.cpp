#include "DirectivesCreator.hpp"

ListenDirectiveCreator::ListenDirectiveCreator()
{
}

ListenDirectiveCreator::~ListenDirectiveCreator()
{
}

ADirective *ListenDirectiveCreator::CreateDirective() const
{
    return new ListenDirective();
}

ServerNameDirectiveCreator::ServerNameDirectiveCreator()
{
}

ServerNameDirectiveCreator::~ServerNameDirectiveCreator()
{
}

ADirective *ServerNameDirectiveCreator::CreateDirective() const
{
    return new ServerNameDirective();
} 

RootDirectiveCreator::RootDirectiveCreator()
{
}

RootDirectiveCreator::~RootDirectiveCreator()
{
}

ADirective *RootDirectiveCreator::CreateDirective() const
{
    return new RootDirective();
}

AutoIndexDirectiveCreator::AutoIndexDirectiveCreator()
{
}

AutoIndexDirectiveCreator::~AutoIndexDirectiveCreator()
{
}

ADirective *AutoIndexDirectiveCreator::CreateDirective() const
{
    return new AutoIndexDirective();
}

IndexDirectiveCreator::IndexDirectiveCreator()
{
}

IndexDirectiveCreator::~IndexDirectiveCreator()
{
}

ADirective *IndexDirectiveCreator::CreateDirective() const
{
    return new IndexDirective();
}

LimitExceptDirectiveCreator::LimitExceptDirectiveCreator()
{
}

LimitExceptDirectiveCreator::~LimitExceptDirectiveCreator()
{
}

ADirective *LimitExceptDirectiveCreator::CreateDirective() const
{
    return new LimitExceptDirective();
}

ErrorPageDirectiveCreator::ErrorPageDirectiveCreator()
{
}

ErrorPageDirectiveCreator::~ErrorPageDirectiveCreator()
{
}

ADirective *ErrorPageDirectiveCreator::CreateDirective() const
{
    return new ErrorPageDirective();
}

ClientMaxBodySizeDirectiveCreator::ClientMaxBodySizeDirectiveCreator()
{
}

ClientMaxBodySizeDirectiveCreator::~ClientMaxBodySizeDirectiveCreator()
{
}

ADirective *ClientMaxBodySizeDirectiveCreator::CreateDirective() const
{
    return new ClientMaxBodySizeDirective();
}