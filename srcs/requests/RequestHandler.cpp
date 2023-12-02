#include "RequestHandler.hpp"
#include "JsonSerializer.hpp"

RequestHandler::RequestHandler(){

}

RequestHandler::RequestHandler(const RequestHandler& other) {
    (void) other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
    if (this != &other) {
        
    }
    return *this;
}

void RequestHandler::sendJsonResponse(std::string json) {
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", "application/json");
	this->sendHeader("Content-Length", json.size());
	this->endHeaders();
	this->writeContent(json);
}

std::vector<std::string> RequestHandler::getFiles(const std::string& path) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir (path.c_str())) != NULL) {
        while ((ent = readdir (dir)) != NULL)
            files.push_back(ent->d_name);
        closedir (dir);
    } 
    else {
        perror ("");
        throw std::runtime_error("Error while reading directory");
    }
    return files;
}

std::string RequestHandler::renderTemplate(const std::string& templateStr, const std::string& value) {
    std::string placeholder = "{data}";
    std::string result = templateStr;
    size_t pos = result.find(placeholder);

    while (pos != std::string::npos) {
        result.replace(pos, placeholder.length(), value);
        pos = result.find(placeholder, pos + value.length());
    }
    return result;
}

void RequestHandler::doGET() {
    std::string content;
    std::string path = this->GetPath();

    if (path == "/")
        path = "/index.html";
    bool foundContent = false;
    content = this->getContent("wwwroot/" + path, foundContent);
    if (path == "/get") {
        std::vector<std::string>::iterator it;
        try {
            std::vector<std::string> files = this->getFiles("../webserver/data");
            std::string value;
            for (it = files.begin(); it != files.end(); it++) {
                if (*it != "." && *it != "..")
                    value += renderTemplate("<h4><a href=\"{data}\"</a>{data}</h4>", *it);
            }
            path = "/get.html";
            content = this->getContent("wwwroot/" + path, foundContent);
            content = this->renderTemplate(content, value);
        }
        catch (std::exception& e) {
            return this->sendJsonResponse("{\"message\": \"" + std::string(e.what()) + "\"}");
        }
    }
    if (path == "/api/files")
        return this->sendJsonResponse("{\"files\": [\"file1\", \"file2\"]}");
    if (foundContent == false)
        return this->sendError("<h1>Not Found</h1>", HTTPStatus::NOT_FOUND);
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
	this->sendHeader("Cache-Control", "no-cache, private");
	this->sendHeader("Content-Type", "text/html");
	this->sendHeader("Content-Length", content.size());
	this->endHeaders();
	this->writeContent(content);
}

void RequestHandler::doPOST() {
    if (this->body.empty())
        return ;
    std::vector<std::string> body = StringUtils::Split(this->body, "&");
    std::map<std::string, std::string> data;
    data["first_name"] = StringUtils::Split(body[0], "=")[1];
    data["last_name"] = StringUtils::Split(body[1], "=")[1];
    std::string filename = "data/" + data["first_name"] + "_" + data["last_name"] + "_" + this->generateRandomString(30) + ".json";
    JsonSerializer::save(JsonSerializer::serialize(data), filename);
    this->body = std::string("");
    this->doGET();
}

void RequestHandler::doDELETE() {
    std::string path = this->GetPath();
    if (path == "/api/files/file1" || path == "/api/files/file2")
        return this->sendJsonResponse("{\"message\": \"File deleted\"}");
}

