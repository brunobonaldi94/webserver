#ifndef HTTP_STATUS_HPP
#define HTTP_STATUS_HPP

#include <iostream>
#include "stdlib.h"

struct StatusCode {
    const int code;
    const std::string description;
    const std::string details;

    StatusCode(int cd, std::string desc, std::string dt) : 
        code(cd), description(desc), details(dt) {
    }
    friend std::ostream& operator<<(std::ostream& os, const StatusCode& status);
};

class HTTPStatus {
    public:
        static StatusCode OK;
        static StatusCode NOT_FOUND;
        static StatusCode BAD_REQUEST;
        static StatusCode HTTP_VERSION_NOT_SUPPORTED;
        static StatusCode METHOD_NOT_ALLOWED;
        static StatusCode CONTENT_TOO_LARGE;
        static StatusCode FORBIDDEN;
        static StatusCode MOVED_PERMANENTLY;
        static StatusCode FOUND;
        static StatusCode SEE_OTHER;
        static StatusCode TEMPORARY_REDIRECT;
        static StatusCode PERMANENT_REDIRECT;
        static StatusCode INTERNAL_SERVER_ERROR;
        static StatusCode NOT_IMPLEMENTED;
        static StatusCode NO_CONTENT;
        static bool validateStatusCode(std::string code);
        static bool validateStatusCode(std::string code, int *httpStatusCodes, int size);
};

#endif