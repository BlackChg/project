#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("192.168.73.130");
	int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("connect error\n");
		exit;
	}


	while (1)
	{
		char buf[1024] = { 0 };
		printf("send:");
		if (fgets(buf, 1024, stdin) == NULL)
			break;
		send(fd, buf, strlen(buf), 0);
		memset(buf, 0x00, sizeof(buf));
		int ret = recv(fd, buf, 1024, 0);
		if (ret <= 0)
			break;
		printf("server: %s\n", buf);
	}
}
