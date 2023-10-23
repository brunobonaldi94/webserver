#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "HTTPStatus.hpp"

TEST_CASE("HTTPStatus") {
    HTTPStatus status;

    CHECK(status.OK.code == 200);
    CHECK(status.OK.description == "OK");
    CHECK(status.OK.code != 0);
    CHECK(status.OK.description != "");
    
    CHECK(status.NOT_FOUND.code == 404);
    CHECK(status.NOT_FOUND.description == "Not Found");
    CHECK(status.NOT_FOUND.code != 0);
    CHECK(status.NOT_FOUND.description != "0");
}

