#include "HTTPStatus.hpp"


StatusCode HTTPStatus::OK(200, "OK", "Request was successful.");
StatusCode HTTPStatus::NOT_FOUND(404, "Not Found", "The requested resource was not found.");
StatusCode HTTPStatus::BAD_REQUEST(400, "Bad Request", "Bad request syntax or unsupported method");
StatusCode HTTPStatus::HTTP_VERSION_NOT_SUPPORTED(505, "HTTP Version Not Supported", "Cannot fulfill request");
StatusCode HTTPStatus::METHOD_NOT_ALLOWED(405, "Method Not Allowed", "Method is not allowed for the requested URL");
StatusCode HTTPStatus::CONTENT_TOO_LARGE(413, "Request Entity Too Large", "The requested resource does not allow request data with GET requests, or the amount of data provided in the request exceeds the capacity limit.");
StatusCode HTTPStatus::FORBIDDEN(403, "Forbidden", "The server understood the request, but is refusing to fulfill it.");
StatusCode HTTPStatus::MOVED_PERMANENTLY(301, "Moved Permanently", "The requested resource has been assigned a new permanent URI and any future references to this resource SHOULD use one of the returned URIs.");
StatusCode HTTPStatus::FOUND(302, "Found", "The requested resource resides temporarily under a different URI.");
StatusCode HTTPStatus::SEE_OTHER(303, "See Other", "The response to the request can be found under a different URI and SHOULD be retrieved using a GET method on that resource.");
StatusCode HTTPStatus::TEMPORARY_REDIRECT(307, "Temporary Redirect", "The requested resource resides temporarily under a different URI.");
StatusCode HTTPStatus::PERMANENT_REDIRECT(308, "Permanent Redirect", "The requested resource has been assigned a new permanent URI and any future references to this resource SHOULD use one of the returned URIs.");
StatusCode HTTPStatus::INTERNAL_SERVER_ERROR(500, "Internal Server Error", "The server encountered an unexpected condition which prevented it from fulfilling the request.");

std::ostream& operator<<(std::ostream& os, const StatusCode& status) {
    os << status.code << std::endl;
    os << status.description << std::endl;
    os << status.details << std::endl;

    return os;
}

bool HTTPStatus::validateStatusCode(std::string code)
{
    for (size_t i = 0; i < code.size(); i++)
    {
        if (!isdigit(code[i]))
            return false;
    }
    int intCode = std::atoi(code.c_str());
    int httpStatusCodes[] = {100, 101, 102, 103, 200, 201, 202, 203, 204, 205,
                            206, 207, 208, 226, 300, 301, 302, 303, 304, 305,
                            306, 307, 308, 400, 401, 402, 403, 404, 405, 406,
                            407, 408, 409, 410, 411, 412, 413, 414, 415, 416,
                            417, 418, 421, 422, 423, 424, 425, 426, 428, 429,
                            431, 451, 500, 501, 502, 503, 504, 505, 506, 507,
                            508, 510, 511};
    for (size_t i = 0; i < sizeof(httpStatusCodes) / sizeof(int); i++)
    {
        if (intCode == httpStatusCodes[i])
            return true;
    }
    return false;
}

bool HTTPStatus::validateStatusCode(std::string code, int *httpStatusCodes, int size)
{
    for (size_t i = 0; i < code.size(); i++)
    {
        if (!isdigit(code[i]))
            return false;
    }
    int intCode = std::atoi(code.c_str());
    for (int i = 0; i < size; i++)
    {
        if (intCode == (httpStatusCodes)[i])
            return true;
    }
    return false;
}