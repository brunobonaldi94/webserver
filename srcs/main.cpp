#include "WebServer.hpp"
#include "ConfigParser.hpp"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: ./webserv <config_file>" << std::endl;
		return -1;
	}
	std::string fileName = argv[1];
	ConfigParser configParser(fileName);
	configParser.ParseConfigFile();

	std::string ipAddress("0.0.0.0");
	std::string port("8080");
	WebServer webServer(ipAddress, port);
	if (webServer.init() != 0)
		return -1;

	webServer.run();

	return 0;
}