#include "MimeTypes.hpp"

MimeTypes::MimeTypes()
{
    m_mime["html"] = "text/html";
    m_mime["css"] = "text/css";
    m_mime["html"] = "text/html";
    m_mime["css"] = "text/css";
    m_mime["js"] = "application/javascript";
    m_mime["json"] = "application/json";
    m_mime["png"] = "image/png";
    m_mime["jpg"] = "image/jpeg";
    m_mime["gif"] = "image/gif";
    m_mime["svg"] = "image/svg+xml";
    m_mime["ico"] = "image/x-icon";
    m_mime["woff"] = "font/woff";
    m_mime["woff2"] = "font/woff2";
    m_mime["ttf"] = "font/ttf";
    m_mime["txt"] = "text/plain";
    m_mime["pdf"] = "application/pdf";
    m_mime["zip"] = "application/zip";
    m_mime["tar"] = "application/x-tar";
    m_mime["gz"] = "application/gzip";
    m_mime["mp3"] = "audio/mpeg";
    m_mime["mp4"] = "video/mp4";
    m_mime["xml"] = "application/xml";
    m_mime["csv"] = "text/csv";
    m_mime["doc"] = "application/msword";
    m_mime["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    m_mime["xls"] = "application/vnd.ms-excel";
    m_mime["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    m_mime["ppt"] = "application/vnd.ms-powerpoint";
    m_mime["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    m_mime["odt"] = "application/vnd.oasis.opendocument.text";
    m_mime["swf"] = "application/x-shockwave-flash";
    m_mime["ics"] = "text/calendar";
    m_mime["vcf"] = "text/x-vcard";
    m_mime["mkv"] = "video/x-matroska";
    m_mime["m3u8"] = "application/x-mpegURL";
    m_mime["ts"] = "video/MP2T";
    m_mime["mov"] = "video/quicktime";
    m_mime["webm"] = "video/webm";
    m_mime["avi"] = "video/x-msvideo";
    m_mime["wmv"] = "video/x-ms-wmv";
    m_mime["mpg"] = "video/mpeg";
    m_mime["mpeg"] = "video/mpeg";
    m_mime["m4v"] = "video/x-m4v";
    m_mime["flv"] = "video/x-flv";
    m_mime["mng"] = "video/x-mng";
    m_mime["webp"] = "image/webp";
}

std::string MimeTypes::ExtensionToType(const std::string& extension)
{
  std::pair<const std::string, std::string> *mime = MapUtils<std::string, std::string>::SafeFindMap(m_mime, extension);
  if (mime == NULL)
    return "text/plain";
  return mime->second;
}