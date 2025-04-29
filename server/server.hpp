// server/server.hpp

#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

// Constants
const int PORT = 8080;
const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

// Function declarations
int setupServerSocket();
void acceptClients(int server_socket);
void handleClient(int client_socket);
void broadcastMessage(const std::string& message, int sender_socket);

#endif // SERVER_HPP
