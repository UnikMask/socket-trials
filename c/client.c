#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main() {
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_fd < 0) {
		perror("Failure to create client socket");
		goto error_generic;
	}

	struct sockaddr_in address = {.sin_family = AF_INET,
								  .sin_port = htons(PORT),
								  .sin_addr.s_addr = 127 + (1 << 24)};

	int err = connect(client_fd, (struct sockaddr *)&address, sizeof(address));
	if (err) {
		perror("Connection Failed");
		goto error_w_socket;
	}

	const char *hello = "Hello Mr. Server!\n";
	send(client_fd, hello, strlen(hello), 0);

	char buf[1024];
	int valread = read(client_fd, buf, 1024 - 1);
	printf("%s\n", buf);

	close(client_fd);
	exit(EXIT_SUCCESS);

error_w_socket:
	close(client_fd);
error_generic:
	exit(EXIT_FAILURE);
}
