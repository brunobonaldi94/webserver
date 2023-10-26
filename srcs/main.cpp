#include "WebServer.hpp"
#include "ConfigParser.hpp"
#include "requests/BaseHTTPRequestHandler.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return -1;
	}
	std::string fileName = argv[1];
	ConfigParser configParser(fileName);
	if (!configParser.ParseConfigFile())
		return -1;

	std::string ipAddress("0.0.0.0");
	std::string port("8080");
	RequestHandler requestHandler;
	WebServer webServer(ipAddress, port, requestHandler);
	if (webServer.init() != 0)
		return -1;

	webServer.run();

	return 0;
}