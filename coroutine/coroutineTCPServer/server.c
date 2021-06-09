#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "coroutine.h"

int tcp_init()
{
	int lst_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (lst_fd == -1) 
	{
		perror("socket error\n");
		exit(1);
	}
	int op = 1;
	setsockopt(lst_fd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("192.168.73.130");
	int ret = bind(lst_fd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("bind error\n");
		exit(1);
	}

	listen(lst_fd, SOMAXCONN);

	return lst_fd;
}

void set_nonblock(int fd)
{
	int flgs = fcntl(fd, F_GETFL, 0);
	flgs |= O_NONBLOCK;
	fcntl(fd, F_SETFL, flgs);
}

void accept_conn(int lst_fd, schedule_t* s, int co_ids[], void* (*call_back)(schedule_t* s, void* args))
{
	while (1)
	{
		int cli_fd = accept(lst_fd, NULL, NULL);
		//如果有新的客户端到来，就创建一个新的协程来管理这个连接
		if (cli_fd > 0)
		{
			set_nonblock(cli_fd);

			int args[] = {lst_fd, cli_fd};
			int id = coroutine_create(s, call_back, args);
			
			if (id == COR_SIZE)
			{
				printf("too many connections.\n");
				return;
			}

			co_ids[id] = 1;;
			coroutine_running(s, id);
		}
		//当前没有客户端连接，则切换至协程的上下文继续运行
		else
		{
			int i;
			for (i = 0; i < COR_SIZE; ++i)
			{
				int cid = co_ids[i];
				if (cid == -1)
					continue;
				coroutine_resume(s, i);
			}
		}
	}
}

void* handle(schedule_t* s, void* args)
{
	int* arr = (int*)args;
	int cli_fd = arr[1];

	char buf[1024] = { 0 };
	while (1)
	{
		memset(buf, 0x00, sizeof(buf));
		int ret = recv(cli_fd, buf, 1024, 0);
		if (ret == -1)
			//如果此时没有数据，则不再等待，直接切回主流程
			coroutine_yield(s);
		else if (ret == 0)
		{
			//通信结束
			break;
		}
		else
		{
			printf("client:%s", buf);
			if (strncasecmp(buf, "exit", 4) == 0)
			{
				break;
			}
			memset(buf, 0x00, sizeof(buf));
			printf("send:");
			fgets(buf, 1024, stdin);
			send(cli_fd, buf, ret, 0);
		}
	}
}

int main()
{
	int lst_fd = tcp_init();
	set_nonblock(lst_fd);

	schedule_t* s = schedule_create();
	static int co_idx[COR_SIZE];
	int i;
	for (i = 0; i < COR_SIZE; ++i)
	{
		co_idx[i] = -1;
	}

	accept_conn(lst_fd, s, co_idx, handle);

	schedule_destroy(s);

	return 0;
}
