#include "ADirective.hpp"

class ListenDirective : public ADirective
{
private:
    int _port;
    std::string _host;
public:
    ListenDirective();
    ~ListenDirective();

    void setPort(int port);
    void setHost(std::string host);

    int getPort() const;
    std::string getHost() const;
};