// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "server_pch.h"
#include "CServer.h"


int main()
{
	CServer server;
	server.add_tasks(10);
	server.start_simulation();

	return 0;
}