// client/client.hpp

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

// Constants
const std::string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;
const int BUFFER_SIZE = 1024;

// Function declarations
void receiveMessages(int server_socket);

#endif // CLIENT_HPP
