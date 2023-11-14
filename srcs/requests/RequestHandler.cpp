#include "RequestHandler.hpp"

void BaseHTTPRequestHandler::send_response(int statusCode, std::string message) {

}

void BaseHTTPRequestHandler::send_header(std::string key, std::string value) {
	this->headers_buffer <<  key << ": " << value << std::endl;
	std::cout << this->headers_buffer.str() << std::endl;
}

void BaseHTTPRequestHandler::send_error() {

}

void RequestHandler::doGET() {
	this->send_header("Cache-Control", "no-cache, private");
	this->send_header("Content-type", "text/html");
}

void RequestHandler::doPOST() {

}

void RequestHandler::doDELETE() {

}

