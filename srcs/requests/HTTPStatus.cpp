#include "HTTPStatus.hpp"


StatusCode HTTPStatus::OK(200, "OK", "Request was successful.");
StatusCode HTTPStatus::NOT_FOUND(404, "Not Found", "The requested resource was not found.");
StatusCode HTTPStatus::BAD_REQUEST(400, "Bad Request", "Bad request syntax or unsupported method");

std::ostream& operator<<(std::ostream& os, const StatusCode& status) {
    os << status.code << std::endl;
    os << status.description << std::endl;
    os << status.details << std::endl;

    return os;
}
