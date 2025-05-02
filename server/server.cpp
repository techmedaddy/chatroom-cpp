// server/server.cpp
#include "server.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <unistd.h>       // for close()
#include <arpa/inet.h>    // for inet_ntoa()
#include <netinet/in.h>   // for sockaddr_in
#include <sys/socket.h>   // for socket functions

#define PORT 8080
#define BUFFER_SIZE 1024

std::vector<int> clients;      // List of connected clients
std::mutex clients_mutex;      // Protects access to clients list

// Function to broadcast a message to all connected clients
void broadcastMessage(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);  // Lock during update

    for (int client_socket : clients) {
        if (client_socket != sender_socket) {   // Don't send back to sender
            send(client_socket, message.c_str(), message.length(), 0);
        }
    }
}

// Function to handle communication with a client
void handleClient(int client_socket) {
    char buffer[BUFFER_SIZE];

    while (true) {
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            // Client disconnected or error
            std::cout << "Client disconnected: " << client_socket << std::endl;
            
            // Remove from client list
            {
                std::lock_guard<std::mutex> lock(clients_mutex);
                clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            }

            close(client_socket);
            break;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the string

        std::string message = "Client " + std::to_string(client_socket) + ": " + buffer;
        std::cout << message << std::endl;

        broadcastMessage(message, client_socket); // Send to others
    }
}

int main() {
    int server_socket;
    sockaddr_in server_address;

    // Step 1: Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Step 2: Bind socket to IP/Port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_address.sin_port = htons(PORT);       // Host-to-network-short

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Bind failed." << std::endl;
        close(server_socket);
        return 1;
    }

    // Step 3: Listen for connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Listen failed." << std::endl;
        close(server_socket);
        return 1;
    }

    std::cout << "Server started on port " << PORT << ". Waiting for connections..." << std::endl;

    // Step 4: Accept clients in a loop
    while (true) {
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);

        if (client_socket < 0) {
            std::cerr << "Error accepting client connection." << std::endl;
            continue; // Continue to accept other clients
        }

        // Print client information
        std::cout << "New client connected: " << inet_ntoa(client_address.sin_addr) 
                  << ":" << ntohs(client_address.sin_port) << std::endl;

        // Add client to the list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        // Handle client in a separate thread
        std::thread client_thread(handleClient, client_socket);
        client_thread.detach(); // Detach the thread to allow independent execution
    }

    // Close the server socket (unreachable in this loop, but good practice)
    close(server_socket);
    return 0;
}