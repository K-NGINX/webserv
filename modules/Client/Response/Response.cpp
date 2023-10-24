#include "Response.hpp"

const std::string& Response::getStatusMessage() const {
    if (status_code_ == "200")
        return "OK";
    else if (status_code_ == "301")
        return "Moved Permanetly";
    else if (status_code_ == "400")
        return "Bad Request";
    else if (status_code_ == "404")
        return "Not Found";
    else if (status_code_ == "405")
        return "Method Not Allowed";
    else if (status_code_ == "413")
        return "Content Too Large";
    else if (status_code_ == "500")
        return "Internal Server Error";
}
