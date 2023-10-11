#include "ADirective.hpp"

class ListenDirective : public ADirective
{
private:
    int _port;
    std::string _host;
public:
    ListenDirective();
    ~ListenDirective();

    void SetPort(int port);
    void SetHost(std::string host);

    int GetPort() const;
    std::string GetHost() const;
    bool ParseDirective(std::string line) const;
};