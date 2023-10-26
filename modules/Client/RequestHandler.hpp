#pragma once

class Client;

class RequestHandler {
public:
    static void handleRequest(Client& client);
    static void handleError(Client& client, const std::string& status_code);
    static void handleRedirection(Client& client);
    static void handleCgi(Client& client);
    static void handleGet(Client& client);
    static void handlePost(Client& client);
    static void handleDelete(Client& client);
};
