#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080
#define BACKLOG_SIZE 128

int main() {
	// Create socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("Socket creation failed!");
		goto error_generic;
	}

	// Set socket options
	int opt = 1;
	int err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
						 sizeof(opt));
	if (err) {
		perror("Set socket options failed");
		goto error_listening;
	}

	struct sockaddr_in address = {.sin_family = AF_INET,
								  .sin_port = htons(PORT),
								  .sin_addr.s_addr = INADDR_ANY};
	unsigned int addrlen = sizeof(address);
	err = bind(sockfd, (struct sockaddr *)&address, addrlen);
	if (err) {
		perror("Failure binding socket");
		goto error_listening;
	}

	err = listen(sockfd, BACKLOG_SIZE);
	if (err) {
		perror("Failure listening to socket");
		goto error_listening;
	}

	int new_socket = accept(sockfd, (struct sockaddr *)&address, &addrlen);
	if (new_socket < 0) {
		perror("Failure to accept a new client");
		goto error_listening;
	}

	char buffer[1024] = {};
	ssize_t valread = read(new_socket, buffer, 1024 - 1);
	printf("%s\n", buffer);
	const char *hello = "Hello brother!";
	err = send(new_socket, hello, strlen(hello), 0);
	if (err < 0) {
		perror("Issue sending message to client");
	}
	close(new_socket);
	close(sockfd);
	exit(EXIT_SUCCESS);
error_listening:
	close(sockfd);
error_generic:
	exit(EXIT_FAILURE);
}
