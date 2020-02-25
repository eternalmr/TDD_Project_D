// Server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "server_pch.h"
#include "CServer.h"


int main()
{
	CServer server;

	int task_num = 20;
	for (int i = 1; i <= task_num; i++) {
		server.add_new_task(i);
	} 


	std::thread      task_thread(&CServer::assign_tasks,	  &server);
	std::thread    result_thread(&CServer::collect_result,    &server, REPEAT_FOREVER);
	std::thread heartbeat_thread(&CServer::receive_heartbeat, &server, REPEAT_FOREVER);

	char command;
	while (true) {
		std::cout << "Please input your command: ";
		std::cin >> command;

		if (command == 's') {
			server.send_command_to_all_client("start");
		}
		else if (command == 'c') {
			server.send_command_to_all_client("continue");
		}
		else if (command == 'p') {
			server.send_command_to_all_client("pause");
		}
		else if (command == 'e') {
			server.send_command_to_all_client("stop");
			std::cout << "Simulation stop!" << std::endl;
			break;
		}
		else {
			std::cout << "Wrong command!" << std::endl;
			continue;
		}
	}

	task_thread.join();
	result_thread.join();
	heartbeat_thread.join();

	return 0;
}