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
    *this = other;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
    if (this != &other)
    {
        
    }
    return *this;
}

void RequestHandler::sendJsonResponse(std::string json, StatusCode statusCode) {
    this->sendResponse(statusCode.code, statusCode.description);
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
            std::vector<std::string> files = this->_directoryHandler->getFilesInDirectory("./data");
            std::string value;
            for (it = files.begin(); it != files.end(); it++) {
                if (*it != "." && *it != "..") {
                    std::string label = (*it).substr(0, it->find_last_of("_"));
                    std::vector<std::string> labelSplited = StringUtils::Split(label, "_");
                    if (labelSplited.size() < 2)
                        continue;
                    label = labelSplited[0] + " " + labelSplited[1];
                    label = renderTemplate("<div><span>{data}</span>", label);
                    std::string btnDelete = renderTemplate(
                        "<button onclick=\"deleteRecord('{data}')\" id=\"deleteLink\" class=\"ml-5 text-red-600\">Delete</button></div>", *it);
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
        {   
            Logger::Log(ERROR, "Invalid body");
            return ;
        }
        data[body[0]] = body[1];
        dynamic_file_name += body[1] + "_";
    }
    std::string filename = "data/" + dynamic_file_name + this->generateRandomString(30) + ".json";
    JsonSerializer::save(JsonSerializer::serialize(data), filename);
}

void RequestHandler::saveMultiPartData()
{
    std::vector<MultiPartData> multiPartData = this->getMultiPartData();
    for (std::vector<MultiPartData>::iterator it = multiPartData.begin(); it != multiPartData.end(); it++)
    {
        if (it->isFileData())
        {
            std::string filename = "wwwroot/upload_files/" + it->fileName;
            std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
            file << it->data;
            file.close();
        }
        else
            Logger::Log(INFO, "Body received: " + it->data);
    }
    // RequestContent *requestContent = this->getCurrentRequestContent();
    // MultiPartData multiPartData = requestContent->getMultiPartData();
    // std::string filename = "wwwroot/upload_files/" + multiPartData.fileName;
    // std::string data = multiPartData.data;
    // std::ofstream file(filename.c_str(), std::ios::out | std::ios::binary);
    // file << data;
    // file.close();
}

void RequestHandler::savePostData()
{
    if (this->getCurrentRequestContent()->getBody().empty())
        return ;
    if (!this->getCurrentRequestContent()->hasParsedAllRequest())
        return ;
    if (this->getCurrentRequestContent()->getHasMultiPartFormData())
    {
        this->saveMultiPartData();
        return;
    }
    this->saveDefaultData();
}

void RequestHandler::doPOST() {
   
    this->savePostData();
    this->doGET();
}

void RequestHandler::doDELETE() {
    std::string path = this->GetPath();
    std::string filename = path.substr(path.find_last_of("/") + 1);
    if (filename.empty())
        return this->sendJsonResponse("{\"message\": \"Invalid filename\"}", HTTPStatus::BAD_REQUEST);
    std::string fullPath = "./data/" + filename;
    bool fileExists = this->_directoryHandler->isValidFile(fullPath);
    if (!fileExists)
        return this->sendJsonResponse("{\"message\": \"File not found\"}", HTTPStatus::NOT_FOUND);
    std::remove(fullPath.c_str());
    this->sendJsonResponse("{\"message\": \"File deleted\"}");
}

void RequestHandler::clearRequestContent(int clientSocket)
{
    if (MapUtils<int, RequestContent>::SafeFindMap(this->clientSocketRequestContentMap, clientSocket))
        this->clientSocketRequestContentMap[clientSocket].clear();
}