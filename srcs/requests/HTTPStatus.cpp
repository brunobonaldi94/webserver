#include "HTTPStatus.hpp"


StatusCode HTTPStatus::OK(200, "OK", "Request was successful.");
StatusCode HTTPStatus::NOT_FOUND(404, "Not Found", "The requested resource was not found.");

std::ostream& operator<<(std::ostream& os, const StatusCode& status) {
    os << status.code << std::endl;
    os << status.description << std::endl;
    os << status.details << std::endl;

    return os;
}
