#pragma once

#include "ADirective.hpp"
#include "StringUtils.hpp"
#include <vector>
#include <string>

#define SPACE " \t"
 
class ListenDirective : public ADirective
{
private:
    int         _port;
    std::string _host;
public:
    ListenDirective();
    ~ListenDirective();
    ListenDirective(ListenDirective const & other);
    ListenDirective& operator=(ListenDirective const & other);

    void SetPort(int port);
    void SetHost(std::string host);
    int GetPort() const;
    std::string GetHost() const;

    bool ValidatePort(std::string port) const;
    bool ValidateHost(std::string host) const;
    bool ParseDirective(std::string &line);
};

class ServerNameDirective : public ADirective
{
private:
    std::vector<std::string> _names;
public:
    ServerNameDirective();
    ~ServerNameDirective();
    ServerNameDirective(ServerNameDirective const & other);
    ServerNameDirective& operator=(ServerNameDirective const & other);

    void AddName(std::string name);
    std::vector<std::string> GetNames() const;
    bool ParseDirective(std::string &line);
};

class ErrorPageDirective : public ADirective
{
private:
    int _code;
    std::string _path;
public:
    ErrorPageDirective();
    ~ErrorPageDirective();
    ErrorPageDirective(ErrorPageDirective const & other);
    ErrorPageDirective& operator=(ErrorPageDirective const & other);

    void SetCode(int code);
    void SetPath(std::string path);

    int GetCode() const;
    std::string GetPath() const;

    bool ValidateCode(std::string code) const;
    bool ParseDirective(std::string &line);
};

class ClientMaxBodySizeDirective : public ADirective
{
private:
    std::string _size;
public:
    ClientMaxBodySizeDirective();
    ~ClientMaxBodySizeDirective();
    ClientMaxBodySizeDirective(ClientMaxBodySizeDirective const & other);
    ClientMaxBodySizeDirective& operator=(ClientMaxBodySizeDirective const & other);

    void SetSize(std::string size);

    std::string GetSize() const;

    bool ValidateSize(std::string size) const;
    bool ParseDirective(std::string &line);
};

class LimitExceptDirective : public ADirective
{
private:
    std::vector<std::string> _methods;
public:
    LimitExceptDirective();
    ~LimitExceptDirective();
    LimitExceptDirective(LimitExceptDirective const & other);
    LimitExceptDirective& operator=(LimitExceptDirective const & other);

    void AddMethod(std::string method);
    std::vector<std::string> GetMethods() const;

    bool ValidateMethods(std::string methods) const;

    bool ParseDirective(std::string &line);
};

class RootDirective : public ADirective
{
private:
    std::string _path;
public:
    RootDirective();
    ~RootDirective();
    RootDirective(RootDirective const & other);
    RootDirective& operator=(RootDirective const & other);

    void SetPath(std::string path);

    std::string GetPath() const;
    bool ParseDirective(std::string &line);
};

class AutoIndexDirective : public ADirective
{
private:
    bool _autoIndex;
public:
    AutoIndexDirective();
    ~AutoIndexDirective();
    AutoIndexDirective(AutoIndexDirective const & other);
    AutoIndexDirective& operator=(AutoIndexDirective const & other);

    void SetAutoIndex(bool autoIndex);

    bool GetAutoIndex() const;
    bool ParseDirective(std::string &line);
};

class IndexDirective : public ADirective
{
private:
    std::vector<std::string> _index;
public:
    IndexDirective();
    ~IndexDirective();
    IndexDirective(IndexDirective const & other);
    IndexDirective& operator=(IndexDirective const & other);

    void AddIndex(std::string index);
    std::vector<std::string> GetIndex() const;
    bool ParseDirective(std::string &line);
};