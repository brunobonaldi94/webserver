#include "CGIRequestHandler.hpp"

CGIRequestHandler::CGIRequestHandler(RequestContent *RequestContent) : _requestContent(RequestContent), envp(NULL)
{
    this->setEnv();
    this->createEnvp();
}

CGIRequestHandler::~CGIRequestHandler()
{
    if (!this->envp)
        return;
    for(size_t index = 0; index < this->env.size(); index++)
        delete[] this->envp[index];
    delete[] this->envp;
}

CGIRequestHandler::CGIRequestHandler(const CGIRequestHandler& other)
{
    *this = other;
}

CGIRequestHandler& CGIRequestHandler::operator=(const CGIRequestHandler& other) {
    if (this != &other)
    {
        this->_requestContent = other._requestContent;
    }
    return *this;
}

char** CGIRequestHandler::stringVectorToArray(const std::vector<std::string>& inputStrings)
{
    char** outputArray = new char*[inputStrings.size() + 1];

    for (std::size_t i = 0; i < inputStrings.size(); ++i)
    {
        outputArray[i] = new char[inputStrings[i].size() + 1];
        std::strcpy(outputArray[i], inputStrings[i].c_str());
    }

    outputArray[inputStrings.size()] = 0;

    return outputArray;
}

void freeStringArray(char** arr)
{
    for (std::size_t i = 0; arr[i]; ++i)
    {
        delete[] arr[i];
    }

    delete[] arr;
}


void CGIRequestHandler::execute() {
    int pipe_fd[2];
    pid_t pid;

    std::string fileName = "cgi.py";
    std::string script_path = "../webserver/wwwroot/cgi-bin/" + fileName;

    if (access(script_path.c_str(), R_OK) != 0)
	{
		std::cerr << "CGI: invalid file" << std::endl;
		exit(EXIT_FAILURE);
	}

    pipe(pipe_fd);
    pid = fork();
    if (pid == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        const char* pathBinPython = "/usr/bin/python3";
        std::vector<std::string> argsString;
        argsString.push_back(" ");
        argsString.push_back(script_path);
        if (execve(pathBinPython, this->stringVectorToArray(argsString), NULL) -1) {
            std::cerr << "Erro ao executar o script CGI\n";
            exit(EXIT_FAILURE);
        }
    } 
    else if (pid > 0) {  
        close(pipe_fd[1]);  
        char buffer[4096];
        ssize_t bytes_read;
        while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            this->content.append(buffer, bytes_read);
        }
        waitpid(pid, NULL, 0);
        close(pipe_fd[0]);
    } 
    else {
        std::cerr << "Erro no fork\n";
    }
}

void CGIRequestHandler::setEnv()
{
    std::cout << "Setando o env!" << std::endl;
    ServerConfig *serverConfig = this->_requestContent->getServerConfig();
    std::vector<std::string> firstLineSplit = StringUtils::Split(this->_requestContent->getFirstRequestLine(), " ");
    std::string authorization = this->_requestContent->getHeader("Authorization");
    this->env["AUTH_TYPE"] = authorization.empty() ? authorization : StringUtils::Split(authorization, " ")[0];
    this->env["CONTENT_LENGTH"] = this->_requestContent->getHeader("Content-Length");
    this->env["CONTENT_TYPE"] = this->_requestContent->getHeader("Content-Type");
    this->env["GATEWAY_INTERFACE"] = "CGI/1.1";
    this->env["HTTP_HOST"] = this->_requestContent->getHeader("Host");
    this->env["SERVER_PORT"] = serverConfig->GetPort();
    this->env["SERVER_PROTOCOL"] = firstLineSplit[2];
    this->env["REQUEST_METHOD"] = firstLineSplit[0];
    this->env["PATH_INFO"] = firstLineSplit[1].replace(0, std::string(CGI_PATH).length(), "");
    this->env["PATH_TRANSLATED"] = "wwwroot" + this->env["PATH_INFO"];
    this->env["SCRIPT_NAME"] = firstLineSplit[1].substr(firstLineSplit[1].find_last_of("/") + 1);
    this->env["QUERY_STRING"] = this->_requestContent->getQueryString();
}

std::string CGIRequestHandler::response() { return this->content; }

char ** CGIRequestHandler::createEnvp()
{
    this->envp = new char *[this->env.size() + 1];
    size_t envp_index = 0;
    for (std::map<std::string,std::string>::iterator it = this->env.begin(); it != this->env.end(); it++)
    {
        std::string key = it->first;
        std::string value = it->second;
        std::string env = key + "=" + value;
        char *envp = new char[env.length() + 1];
        strcpy(envp, env.c_str());
        this->envp[envp_index] = envp;
        envp_index++;
    }
    this->envp[envp_index] = NULL;
    return this->envp;
}
