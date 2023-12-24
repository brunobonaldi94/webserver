#include "doctest.h"
#include "RequestHandler.hpp"

TEST_CASE("SENDER HEADER")
{
    ADirectoryHandler *directoryHandler = new DirectoryHandler();
    RequestHandler requestHandler(directoryHandler);
    requestHandler.sendHeader("Cache-Control", "no-cache, private");
	requestHandler.sendHeader("Content-type", "text/html");

    CHECK(requestHandler.headersBufferToString() == \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n");
    CHECK(requestHandler.headersBufferToString() != \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n\n");
    CHECK(requestHandler.headersBufferToString() != "Cache-Control: no-cache, private");
    CHECK(requestHandler.headersBufferToString() != "");

    //send new header
    requestHandler.sendHeader("Content-Length", "100");
    CHECK(requestHandler.headersBufferToString() == \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n" \
        "Content-Length: 100\n");
}

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
    requestHandler.setClientSockerRequestContentMap(3, serverConfig);
    requestHandler.parseRequest(request);
    std::string body = requestHandler.getCurrentRequestContent()->getBody();
    delete serverConfig;
    delete serverContext;
    CHECK(body == "hello world");
}
