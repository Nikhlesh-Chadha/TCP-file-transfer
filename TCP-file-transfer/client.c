#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#define SERVER_IP "127.0.0.1" // Server IP address
#define SERVER_PORT 9002
#define BUFSIZE 4096
int main() {
int client_socket;
struct sockaddr_in server_address;
// Create TCP client socket
client_socket = socket(AF_INET, SOCK_STREAM, 0);
if (client_socket == -1) {
perror("Socket creation failed");
exit(1);
}
// Configure server address
server_address.sin_family = AF_INET;
server_address.sin_port = htons(SERVER_PORT);
server_address.sin_addr.s_addr = inet_addr(SERVER_IP);
// Connect to the server
if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
perror("Connection failed");
exit(1);
}
char buffer[BUFSIZE];
int bytesRead;
// Receive the file size from the server
size_t file_size;
recv(client_socket, &file_size, sizeof(file_size), 0);
// Create or open a file for writing
FILE* file = fopen("/home/rakesh/Desktop/Projects/client.txt", "wb");
if (file == NULL) {
perror("File opening failed");
exit(1);
}
// Receive and write the file content
size_t remaining_bytes = file_size;
while (remaining_bytes > 0) {
bytesRead = recv(client_socket, buffer, BUFSIZE, 0);
if (bytesRead <= 0) {
break;
}
fwrite(buffer, 1, bytesRead, file);
remaining_bytes -= bytesRead;
}
// Close the socket and file
fclose(file);
close(client_socket);
printf("File transfer complete.\n");
// Display the contents of the received file
file = fopen("/home/rakesh/Desktop/Projects/client.txt", "rb");
if (file == NULL) {
perror("File opening failed");
exit(1);
}
printf("Contents of received file:\n");
while (bytesRead = fread(buffer, 1, BUFSIZE, file)) {
fwrite(buffer, 1, bytesRead, stdout);
}
fclose(file);
return 0;
}
