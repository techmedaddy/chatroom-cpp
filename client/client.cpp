// client/client.cpp
#include "client.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>       // for close()
#include <arpa/inet.h>    // for inet_pton()
#include <sys/socket.h>   // for socket functions
#include <netinet/in.h>   // for sockaddr_in

#define SERVER_IP "127.0.0.1"  // Server IP (localhost)
#define SERVER_PORT 8080       // Server Port
#define BUFFER_SIZE 1024

// Function to continuously receive messages from the server
void receiveMessages(int server_socket) {
    char buffer[BUFFER_SIZE];

    while (true) {
        ssize_t bytes_received = recv(server_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0) {
            std::cout << "Disconnected from server." << std::endl;
            close(server_socket);
            exit(0);
        }

        buffer[bytes_received] = '\0'; // Null-terminate the message
        std::cout << buffer << std::endl;
    }
}

int main() {
    int client_socket;
    sockaddr_in server_address;

    // Step 1: Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Step 2: Setup server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported." << std::endl;
        return 1;
    }

    // Step 3: Connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Connection Failed." << std::endl;
        return 1;
    }

    std::cout << "Connected to the chatroom!" << std::endl;
    std::cout << "Type your messages below:" << std::endl;

    // Step 4: Start a thread to receive messages
    std::thread receiver_thread(receiveMessages, client_socket);
    receiver_thread.detach();

    // Step 5: Main thread will send messages
    std::string message;
    while (true) {
        std::getline(std::cin, message);

        if (message.empty()) {
            continue;
        }

        send(client_socket, message.c_str(), message.length(), 0);
    }

    close(client_socket);
    return 0;
}
