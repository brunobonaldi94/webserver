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
	if (!configParser.ParseConfigFile())
		return -1;
	std::vector<AContext *> serverContexts = configParser.GetServerContexts();
	WebServer webServ(serverContexts);
	if (!webServ.Init())
		return -1;
	if (!webServ.Run())
		return -1;
	return 0;
}