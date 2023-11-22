#pragma once

#include "ADirective.hpp"
#include "StringUtils.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include <vector>
#include <string>

class ListenDirective : public ADirective
{
private:
    std::string _port;
    std::string _host;
public:
    ListenDirective();
    ~ListenDirective();
    ListenDirective(ListenDirective const & other);
    ListenDirective& operator=(ListenDirective const & other);

    void SetPort(int port);
    void SetHost(std::string host);
    std::string GetPort() const;
    std::string GetHost() const;

    bool ValidatePort(std::string port) const;
    bool ValidateHost(std::string host) const;
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
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
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
};

class ErrorPageDirective : public ADirective
{
private:
    std::string _code;
    std::string _path;
public:
    ErrorPageDirective();
    ~ErrorPageDirective();
    ErrorPageDirective(ErrorPageDirective const & other);
    ErrorPageDirective& operator=(ErrorPageDirective const & other);

    void SetCode(int code);
    void SetPath(std::string path);

    std::string GetCode() const;
    std::string GetPath() const;

    bool ValidateCode(std::string code) const;
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
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
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
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

    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
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
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
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
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
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
    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
};

class CgiDirective : public ADirective
{
private:
    std::string _extension;
    std::string _binaryPath;
public:
    CgiDirective();
    ~CgiDirective();
    CgiDirective(CgiDirective const & other);
    CgiDirective& operator=(CgiDirective const & other);

    void SetExtension(std::string extension);
    void SetBinaryPath(std::string binaryPath);

    std::string GetExtension() const;
    std::string GetBinaryPath() const;
    bool ValidateExtension(std::string extension) const;

    void ParseDirective(std::string &line);
    void PrintDirective() const;
    void FillDefaultValues();
    bool SetDefaultFromParent();
};