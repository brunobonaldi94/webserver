#pragma once

#include "ADirectiveCreator.hpp"


class ListenDirectiveCreator : public ADirectiveCreator
{
public:
    ListenDirectiveCreator();
    ListenDirectiveCreator(ListenDirectiveCreator const & other);
    ~ListenDirectiveCreator();

    ListenDirectiveCreator& operator=(ListenDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class ServerNameDirectiveCreator : public ADirectiveCreator
{
public:
    ServerNameDirectiveCreator();
    ServerNameDirectiveCreator(ServerNameDirectiveCreator const & other);
    ~ServerNameDirectiveCreator();

    ServerNameDirectiveCreator& operator=(ServerNameDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class RootDirectiveCreator : public ADirectiveCreator
{
public:
    RootDirectiveCreator();
    RootDirectiveCreator(RootDirectiveCreator const & other);
    ~RootDirectiveCreator();

    RootDirectiveCreator& operator=(RootDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class AutoIndexDirectiveCreator : public ADirectiveCreator
{
public:
    AutoIndexDirectiveCreator();
    AutoIndexDirectiveCreator(AutoIndexDirectiveCreator const & other);
    ~AutoIndexDirectiveCreator();

    AutoIndexDirectiveCreator& operator=(AutoIndexDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class IndexDirectiveCreator : public ADirectiveCreator
{
public:
    IndexDirectiveCreator();
    IndexDirectiveCreator(IndexDirectiveCreator const & other);
    ~IndexDirectiveCreator();

    IndexDirectiveCreator& operator=(IndexDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class ErrorPageDirectiveCreator : public ADirectiveCreator
{
public:
    ErrorPageDirectiveCreator();
    ErrorPageDirectiveCreator(ErrorPageDirectiveCreator const & other);
    ~ErrorPageDirectiveCreator();

    ErrorPageDirectiveCreator& operator=(ErrorPageDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class ClientMaxBodySizeDirectiveCreator : public ADirectiveCreator
{
public:
    ClientMaxBodySizeDirectiveCreator();
    ClientMaxBodySizeDirectiveCreator(ClientMaxBodySizeDirectiveCreator const & other);
    ~ClientMaxBodySizeDirectiveCreator();

    ClientMaxBodySizeDirectiveCreator& operator=(ClientMaxBodySizeDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};

class LimitExceptDirectiveCreator : public ADirectiveCreator
{
public:
    LimitExceptDirectiveCreator();
    LimitExceptDirectiveCreator(LimitExceptDirectiveCreator const & other);
    ~LimitExceptDirectiveCreator();

    LimitExceptDirectiveCreator& operator=(LimitExceptDirectiveCreator const & other);

    ADirective *createDirective(std::string const &name) const;
};