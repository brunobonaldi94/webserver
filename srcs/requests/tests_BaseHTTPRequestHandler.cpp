#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "RequestHandler.hpp"

TEST_CASE("SENDER HEADER") {
    BaseHTTPRequestHandler base;
    base.sendHeader("Cache-Control", "no-cache, private");
	base.sendHeader("Content-type", "text/html");

    CHECK(base.headersBufferToString() == \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n");
    CHECK(base.headersBufferToString() != \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n\n");
    CHECK(base.headersBufferToString() != "Cache-Control: no-cache, private");
    CHECK(base.headersBufferToString() != "");

    //send new header
    base.sendHeader("Content-Length", "100");
    CHECK(base.headersBufferToString() == \
        "Cache-Control: no-cache, private\n" \
        "Content-type: text/html\n" \
        "Content-Length: 100\n");
}
