#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[30000] = {0};

    // Create Socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0){
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // Setup Address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Bind
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if(listen(server_fd, 3) < 0){
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server running on port 8080... \n";

    // Accept Connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    // Read Request
    read(new_socket, buffer, 30000);
    cout << buffer << endl;

    // Send Response
    const char* response =
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n\n"
        "<html><body><h1>Hello from C++ Server!</h1></body></html>";

    send(new_socket, response , strlen(response), 0);

    // Close Sockets
    close(new_socket);
    close(server_fd);

    return 0;
}
