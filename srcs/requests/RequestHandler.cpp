#include "RequestHandler.hpp"
#include "JsonSerializer.hpp"

RequestHandler::RequestHandler(ADirectoryHandler *directoryHandler)
: BaseHTTPRequestHandler(directoryHandler)
{

}

RequestHandler::~RequestHandler()
{
}

RequestHandler::RequestHandler(const RequestHandler& other): BaseHTTPRequestHandler(other._directoryHandler)
{
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

    content = this->getContent(path);
    if (path == "/get") {
        std::vector<std::string>::iterator it;
        try {
            std::vector<std::string> files = this->_directoryHandler->getFilesInDirectory("../webserver/data");
            std::string value;
            for (it = files.begin(); it != files.end(); it++) {
                if (*it != "." && *it != "..") {
                    std::string label = (*it).substr(0, it->find_last_of("_"));
                    std::vector<std::string> labelSplited = StringUtils::Split(label, "_");
                    label = labelSplited[0] + " " + labelSplited[1];
                    label = renderTemplate("<h4><a href=\"{data}\"></a>{data}", label);
                    std::string btnDelete = renderTemplate(
                        "<a href=\"/get\" onclick=\"deleteRecord('{data}')\" id=\"deleteLink\" class=\"ml-5 text-red-600\">Delete</a></h4>", *it);
                    value += (label + btnDelete);
                }
            }
            content = this->getContent(path);
            content = this->renderTemplate(content, value);
        }
        catch (std::exception& e) {
            return this->sendJsonResponse("{\"message\": \"" + std::string(e.what()) + "\"}");
        }
    }
    if (path == "/api/files")
        return this->sendJsonResponse("{\"files\": [\"file1\", \"file2\"]}");
    if (this->getContentNotFound())
        return this->sendNotFoundError();
    this->sendResponse(HTTPStatus::OK.code, HTTPStatus::OK.description);
    this->writeDefaultResponse(content);
}

void RequestHandler::saveDefaultData() {
    std::vector<std::string> bodySplit = StringUtils::Split(this->getCurrentRequestContent()->getBody(), "&");
    std::map<std::string, std::string> data;
    std::string dynamic_file_name;
    for (std::vector<std::string>::iterator it = bodySplit.begin(); it != bodySplit.end(); it++)
    {
        std::vector<std::string> body = StringUtils::SplitAtFirstDelimiter(*it, "=");
        if (body.size() != 2)
            throw std::runtime_error("Invalid body");
        data[body[0]] = body[1];
        dynamic_file_name += body[1] + "_";
    }
    std::string filename = "data/" + dynamic_file_name + this->generateRandomString(30) + ".json";
    JsonSerializer::save(JsonSerializer::serialize(data), filename);
}

void RequestHandler::saveMultiPartData()
{
    RequestContent *requestContent = this->getCurrentRequestContent();
    MultiPartData multiPartData = requestContent->getMultiPartData();
    std::string filename = "data/" + multiPartData.fileName;
    std::string data = multiPartData.data;
    std::ofstream file(filename.c_str());
    file << data;
    file.close();
}

void RequestHandler::savePostData()
{
    if (this->getCurrentRequestContent()->getBody().empty())
        return ;
    if (this->getCurrentRequestContent()->isMultiPartFormData())
    {
        this->saveMultiPartData();
        return;
    }
    this->saveDefaultData();
}

void RequestHandler::doPOST() {
   
    this->savePostData();
    this->getCurrentRequestContent()->clear();
    this->doGET();
}

void RequestHandler::doDELETE() {
    std::string path = this->GetPath();
    std::string filename = path.substr(path.find_last_of("/") + 1);
    std::remove(("../webserver/data/" + filename).c_str());
}

void RequestHandler::clearRequestContent(int clientSocket)
{
    if (MapUtils<int, RequestContent>::SafeFindMap(this->clientSocketRequestContentMap, clientSocket))
        this->clientSocketRequestContentMap[clientSocket].clear();
}