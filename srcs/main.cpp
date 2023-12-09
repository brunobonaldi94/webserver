#include "WebServer.hpp"
#include "ConfigParser.hpp"

int main(int argc, char *argv[])
{
		try 
		{
				if (argc != 2)
			{
				std::cerr << "Usage: ./webserv <config_file>" << std::endl;
				return ERROR_CODE;
			}
			std::string fileName = argv[1];
			ConfigParser configParser(fileName);
			if (!configParser.ParseConfigFile())
				return ERROR_CODE;
			std::vector<ServerConfig *> serverConfigs = configParser.CreateServerConfigs();
			ADirectoryHandler *directoryHandler = new DirectoryHandler();
			RequestHandler requestHandler(directoryHandler);
			WebServer webServ(requestHandler, serverConfigs);
			if (!webServ.Init())
				return ERROR_CODE;
			if (!webServ.Run())
				return ERROR_CODE;
			return SUCCESS_CODE;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return ERROR_CODE;
	}
}