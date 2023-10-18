#pragma once

#include "ADirectiveCreator.hpp"
#include "Directives.hpp"

class ListenDirectiveCreator : public ADirectiveCreator
{
public:
    ListenDirectiveCreator();
    ListenDirectiveCreator(ListenDirectiveCreator const & other);
    ~ListenDirectiveCreator();

    ListenDirectiveCreator& operator=(ListenDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class ServerNameDirectiveCreator : public ADirectiveCreator
{
public:
    ServerNameDirectiveCreator();
    ServerNameDirectiveCreator(ServerNameDirectiveCreator const & other);
    ~ServerNameDirectiveCreator();

    ServerNameDirectiveCreator& operator=(ServerNameDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class RootDirectiveCreator : public ADirectiveCreator
{
public:
    RootDirectiveCreator();
    RootDirectiveCreator(RootDirectiveCreator const & other);
    ~RootDirectiveCreator();

    RootDirectiveCreator& operator=(RootDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class AutoIndexDirectiveCreator : public ADirectiveCreator
{
public:
    AutoIndexDirectiveCreator();
    AutoIndexDirectiveCreator(AutoIndexDirectiveCreator const & other);
    ~AutoIndexDirectiveCreator();

    AutoIndexDirectiveCreator& operator=(AutoIndexDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class IndexDirectiveCreator : public ADirectiveCreator
{
public:
    IndexDirectiveCreator();
    IndexDirectiveCreator(IndexDirectiveCreator const & other);
    ~IndexDirectiveCreator();

    IndexDirectiveCreator& operator=(IndexDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class ErrorPageDirectiveCreator : public ADirectiveCreator
{
public:
    ErrorPageDirectiveCreator();
    ErrorPageDirectiveCreator(ErrorPageDirectiveCreator const & other);
    ~ErrorPageDirectiveCreator();

    ErrorPageDirectiveCreator& operator=(ErrorPageDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class ClientMaxBodySizeDirectiveCreator : public ADirectiveCreator
{
public:
    ClientMaxBodySizeDirectiveCreator();
    ClientMaxBodySizeDirectiveCreator(ClientMaxBodySizeDirectiveCreator const & other);
    ~ClientMaxBodySizeDirectiveCreator();

    ClientMaxBodySizeDirectiveCreator& operator=(ClientMaxBodySizeDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class LimitExceptDirectiveCreator : public ADirectiveCreator
{
public:
    LimitExceptDirectiveCreator();
    LimitExceptDirectiveCreator(LimitExceptDirectiveCreator const & other);
    ~LimitExceptDirectiveCreator();

    LimitExceptDirectiveCreator& operator=(LimitExceptDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};

class CgiDirectiveCreator : public ADirectiveCreator
{
public:
    CgiDirectiveCreator();
    CgiDirectiveCreator(CgiDirectiveCreator const & other);
    ~CgiDirectiveCreator();

    CgiDirectiveCreator& operator=(CgiDirectiveCreator const & other);

    ADirective *CreateDirective() const;
};