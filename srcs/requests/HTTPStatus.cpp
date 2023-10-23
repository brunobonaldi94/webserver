#include "HTTPStatus.hpp"

HTTPStatus::HTTPStatus() : 
    OK(200, "OK", "Request fulfilled, document follows"), 
    NOT_FOUND(404, "Not Found", "Nothing matches the given URI") {}

std::ostream& operator<<(std::ostream& os, const StatusCode& status) {
    os << status.code << std::endl;
    os << status.description << std::endl;
    os << status.details << std::endl;

    return os;
}
