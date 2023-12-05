#include "HTTPStatus.hpp"


StatusCode HTTPStatus::OK(200, "OK", "Request was successful.");
StatusCode HTTPStatus::NOT_FOUND(404, "Not Found", "The requested resource was not found.");
StatusCode HTTPStatus::BAD_REQUEST(400, "Bad Request", "Bad request syntax or unsupported method");
StatusCode HTTPStatus::HTTP_VERSION_NOT_SUPPORTED(505, "HTTP Version Not Supported", "Cannot fulfill request");
StatusCode HTTPStatus::METHOD_NOT_ALLOWED(405, "Method Not Allowed", "Method is not allowed for the requested URL");
StatusCode HTTPStatus::CONTENT_TOO_LARGE(413, "Request Entity Too Large", "The requested resource does not allow request data with GET requests, or the amount of data provided in the request exceeds the capacity limit.");
StatusCode HTTPStatus::FORBIDDEN(403, "Forbidden", "The server understood the request, but is refusing to fulfill it.");

std::ostream& operator<<(std::ostream& os, const StatusCode& status) {
    os << status.code << std::endl;
    os << status.description << std::endl;
    os << status.details << std::endl;

    return os;
}
