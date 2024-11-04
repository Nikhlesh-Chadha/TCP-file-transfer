#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#define PORT 9002
#define BUFSIZE 4096
int main() {
int server_socket, client_socket;
struct sockaddr_in server_address, client_address;
socklen_t client_len = sizeof(client_address);
// Create TCP server socket
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket == -1) {
perror("Socket creation failed");
exit(1);
}
// Configure server address
server_address.sin_family = AF_INET;
server_address.sin_port = htons(PORT);
server_address.sin_addr.s_addr = INADDR_ANY;
// Bind the socket to the specified IP address and port
if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
perror("Binding failed");
exit(1);
}
printf("Server listening on port %d...\n", PORT);
// Listen for incoming connections
if (listen(server_socket, 5) == -1) {
perror("Listening failed");
exit(1);
}
// Accept a client connection
client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_len);
char buffer[BUFSIZE];
int bytesRead;
// Specify the file to be transferred
char* file_path = "/home/rakesh/Desktop/Projects/server.txt";
FILE* file = fopen(file_path, "rb");
if (file == NULL) {
perror("File opening failed");
exit(1);
}
// Get the file size
struct stat st;
stat(file_path, &st);
size_t file_size = st.st_size;
// Send the file size to the client
send(client_socket, &file_size, sizeof(file_size), 0);
// Send the file content to the client
while ((bytesRead = fread(buffer, 1, BUFSIZE, file)) > 0) {
send(client_socket, buffer, bytesRead, 0);
}
// Close the sockets and file
fclose(file);
close(client_socket);
close(server_socket);
printf("File transfer complete.\n");
return 0;
}
