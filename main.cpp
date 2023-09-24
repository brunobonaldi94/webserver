#include "WebServer.hpp"

int main()
{
	std::string ipAddress("0.0.0.0");
	std::string port("8080");
	WebServer webServer(ipAddress, port);
	if (webServer.init() != 0)
		return -1;

	webServer.run();

	return 0;
}