#include "doctest.h"
#include "RequestHandler.hpp"


TEST_CASE("parseRequestChuckedBody")
{
    ADirectoryHandler *directoryHandler = new DirectoryHandler();
    RequestHandler requestHandler(directoryHandler);
    std::string request = "POST / HTTP/1.1\r\n" \
        "Host: localhost:8080\r\n" \
        "User-Agent: curl/7.64.1\r\n" \
        "Accept: */*\r\n" \
        "Transfer-Encoding: chunked\r\n" \
        "Content-Type: application/x-www-form-urlencoded\r\n" \
        "\r\n" \
        "5\r\n" \
        "hello\r\n" \
        "6\r\n" \
        " world\r\n" \
        "0\r\n" \
        "\r\n";
    ServerContext *serverContext = new ServerContext();
    ServerConfig *serverConfig = new ServerConfig(serverContext);
    requestHandler.parseRequestForClientSocket(request, 3, serverConfig);
    std::string body = requestHandler.getCurrentRequestContent()->getBody();
    delete serverConfig;
    delete serverContext;
    CHECK(body == "hello world");
}

TEST_CASE("parseRegularBody")
{
    ADirectoryHandler *directoryHandler = new DirectoryHandler();
    RequestHandler requestHandler(directoryHandler);
    std::string request = "POST / HTTP/1.1\r\n" \
        "Host: localhost:8080\r\n" \
        "User-Agent: curl/7.64.1\r\n" \
        "Accept: */*\r\n" \
        "Content-Length: 11\r\n" \
        "Content-Type: application/x-www-form-urlencoded\r\n" \
        "\r\n" \
        "hello world";
    ServerContext *serverContext = new ServerContext();
    ServerConfig *serverConfig = new ServerConfig(serverContext);
    requestHandler.parseRequestForClientSocket(request, 4, serverConfig);
    std::string body = requestHandler.getCurrentRequestContent()->getBody();
    delete serverConfig;
    delete serverContext;
    CHECK(body == "hello world");
}

TEST_CASE("parseRequestMultipartData")
{
    ADirectoryHandler *directoryHandler = new DirectoryHandler();
    RequestHandler requestHandler(directoryHandler);
std::string bodyRequest = "\r\n--------------------------d1b6c5f7e0e5a8a8\r\n" \
    "Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n" \
    "Content-Type: text/plain\r\n" \
    "\r\n" \
    "hello world\r\n" \
    "--------------------------d1b6c5f7e0e5a8a8--";

std::string request = "POST / HTTP/1.1\r\n" \
    "Host: localhost:8080\r\n" \
    "User-Agent: curl/7.64.1\r\n" \
    "Accept: */*\r\n" \
    "Content-Length: " + StringUtils::ConvertNumberToString(bodyRequest.length()) + "\r\n" \
    "Content-Type: multipart/form-data; boundary=------------------------d1b6c5f7e0e5a8a8\r\n" \
    "\r\n" + bodyRequest;
    ServerContext *serverContext = new ServerContext();
    ServerConfig *serverConfig = new ServerConfig(serverContext);
    requestHandler.parseRequestForClientSocket(request, 5, serverConfig);
    std::string body = requestHandler.getCurrentRequestContent()->getBody();
    delete serverConfig;
    delete serverContext;
    CHECK(body == "hello world");
}