#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>

#define LOCAL_PATH "/var/clinet"
#define SERVER_PATH "/var/server"

static void soecket_tx_buffer_debug(int socket_fd)
{
        int maxsndbuf = 0, outq = 0;
        int len = 0;

        if (getsockopt(socket_fd, SOL_SOCKET, SO_SNDBUF, &maxsndbuf, &len) < 0)
                return;

        if (ioctl(socket_fd, TIOCOUTQ, &outq) < 0)
                return;

        printf("sock=%d maxsndbuf=%d outq=%d\n", socket_fd, maxsndbuf, outq);
}

static void soecket_rx_buffer_debug(int socket_fd)
{
        int maxrcvbuf = 0, inq = 0;
        int len = 0;

        if (getsockopt(socket_fd, SOL_SOCKET, SO_RCVBUF, &maxrcvbuf, &len) < 0)
                return;

        if (ioctl(socket_fd, TIOCINQ, &inq) < 0)
                return;

        printf("sock=%d maxrcvbuf=%d intq=%d\n", socket_fd, maxrcvbuf, inq);
}

static int open_client_socket(int *fd)
{
	struct sockaddr_un client;

	if ((*fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		printf("create socket fail\n");
		return -1;
	}

	memset(&client, 0 , sizeof(client));
	client.sun_family = AF_UNIX;
	// do not create really file
	client.sun_path[0] = '\0';
	strncpy(&client.sun_path[1], LOCAL_PATH, sizeof(client.sun_path) - 1);

	if (bind(*fd, (struct sockaddr *) &client, sizeof(client)) == -1) {
		printf("bind socket fail\n");
		return -1;
	}

	return 0;
}

static int open_server_socket(int *fd)
{
	struct sockaddr_un client;

	if ((*fd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		printf("create socket fail\n");
		return -1;
	}

	memset(&client, 0 , sizeof(client));
	client.sun_family = AF_UNIX;
	// do not create really file
	client.sun_path[0] = '\0';
	strncpy(&client.sun_path[1], SERVER_PATH, sizeof(client.sun_path) - 1);

	if (bind(*fd, (struct sockaddr *) &client, sizeof(client)) == -1) {
		printf("bind socket fail\n");
		return -1;
	}

	return 0;
}

static int reinit_client_socket(int *fd)
{
	printf("reinit socket\n");
	close(*fd);
	open_client_socket(fd);
}

static void server_receive(int fd)
{
	socklen_t fromlen = 0;
	struct sockaddr_un from;
	char buf [1024];
	ssize_t received = 0;

	received = recvfrom(fd, buf, sizeof(buf), MSG_DONTWAIT, (struct sockaddr *) &from, &fromlen);

	if (received == -1) {
		printf("server_receive Error: no = %d, %s\n", errno, strerror(errno));
	} else if (received > 0) {
		printf("server_receive %ld data,\n", received);
	}
}

int main(void)
{
	int client_fd = 0, server_fd = 0;
	ssize_t sent = 0;
	struct sockaddr_un server;
	char test_data [1024 * 2] = {0};

	open_client_socket(&client_fd);
	open_server_socket(&server_fd);

	memset(&server, 0, sizeof(server));
	server.sun_family = AF_UNIX;
	// do not create really file
	server.sun_path[0] = '\0';
	strncpy(&server.sun_path[1], SERVER_PATH, sizeof(server.sun_path) - 1);

	memset(&test_data, 1, sizeof(test_data));
	for(;;) {
		soecket_tx_buffer_debug(client_fd);
		soecket_rx_buffer_debug(server_fd);
		
		if ((sent = sendto(client_fd, test_data, strlen(test_data), MSG_DONTWAIT, (struct sockaddr *) &server, sizeof(server))) == -1) {
			printf("Error: no = %d, %s\n", errno, strerror(errno));
		} else {
			printf("clent sent %ld data\n", sent);
		}

		soecket_tx_buffer_debug(client_fd);
		soecket_rx_buffer_debug(server_fd);
		sleep(10);

		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			reinit_client_socket(&client_fd);
			errno = 0;
		}

		server_receive(server_fd);
	}

	close(client_fd);

	return 0;
}
