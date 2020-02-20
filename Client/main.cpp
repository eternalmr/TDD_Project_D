// Client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "client_pch.h"
#include "CClient.h"


int main(int argc, char* argv[])
{
	// get client id from argv
	uint32_t id = std::atoi(argv[1]);
	//uint32_t id = 1;
	cout << "client_id: " << id << endl;

	CClient client(id);
	Command command;

	std::thread simulation_thread(&CClient::simulation_wrap, &client, 0);
	std::thread heartbeat_thread(&CClient::send_heartbeat, &client, 0);

	while (true) {
		command = client.listen_from_server();
		if (client.is_irrelevant(command)) continue;
		client.execute_control_command(command);
		if(client.stop_flag) break;
	}

	simulation_thread.join();
	heartbeat_thread.join();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
