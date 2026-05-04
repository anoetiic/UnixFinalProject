#include <iostream> // Allows input/output (cout)
#include <fstream> // Allows file reading (ifstream)
#include <sstream> // Allows string streams
#include <string> // Allows use of string type
#include <unistd.h> // Provides close(), read(), write()
#include <netinet/in.h> // Provides internet structures (sockaddr_in)

using namespace std; // Avoids needing std:: everywhere

int main() { // Main function where program starts

    int server_fd, new_socket; // server_fd = server socket, new_socket = client connection
    struct sockaddr_in address; // Structure to store server address info
    int addrlen = sizeof(address); // Size of address structure

    int port = 9090; // Port number server will listen on

    server_fd = socket(AF_INET, SOCK_STREAM, 0); // Create TCP socket

    address.sin_family = AF_INET; // Use IPv4
    address.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
    address.sin_port = htons(port); // Convert port to network byte order

    bind(server_fd, (struct sockaddr*)&address, sizeof(address)); // Bind socket to IP and port

    listen(server_fd, 5); // Start listening (max 5 queued connections)

    cout << "Server running on port " << port << endl; // Print server status

    while (true) { // Infinite loop to keep server running

        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen); // Accept incoming connection

        char buffer[30000] = {0}; // Create buffer to store request
        read(new_socket, buffer, 30000); // Read HTTP request from client

        ifstream file("bingo.html"); // Open HTML file

        stringstream ss; // Create string stream
        ss << file.rdbuf(); // Read entire file into stream
        string html = ss.str(); // Convert stream to string

        string response = // Build HTTP response
            "HTTP/1.1 200 OK\n" // Status line
            "Content-Type: text/html\n" // Tell browser it's HTML
            "Content-Length: " + to_string(html.size()) + "\n\n" + // Length of content
            html; // Attach HTML content

        send(new_socket, response.c_str(), response.size(), 0); // Send response to browser

        close(new_socket); // Close client connection
    }

    return 0; // End program (never reached)
}
