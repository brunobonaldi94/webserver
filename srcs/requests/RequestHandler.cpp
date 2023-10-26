#include "RequestHandler.hpp"

RequestHandler::RequestHandler(){

}

RequestHandler::RequestHandler(const RequestHandler& other) {
    (void) other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
    if (this != &other) {
        // Realize a cópia dos membros relevantes, se necessário.
    }
    return *this;
}

void RequestHandler::doGET() {
	
}

void RequestHandler::doPOST() {

}

void RequestHandler::doDELETE() {

}

