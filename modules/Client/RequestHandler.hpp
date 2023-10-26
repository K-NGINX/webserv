#pragma once

class Client;

void HandleRequest(Client& client);
void HandleError(Client& client, int status_code);
void HandleRedirection(Client& client);
void HandleCgi(Client& client);
void HandleGet(Client& client);
void HandlePost(Client& client);
void HandleDelete(Client& client);