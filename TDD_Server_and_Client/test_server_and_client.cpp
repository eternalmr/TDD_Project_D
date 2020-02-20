#include "pch.h"
#include "../Server/CServer.h"
#include "../Client/CClient.h"
#include <future>

class ServerAndClientTest : public testing::Test {
public:
	CServer server;
public:
	void SetUp() override {
		server.bind_sockets_to_ip();
	}

	void TearDown() override {
		//server.unbind_sockets_to_ip();
	}
};

class ServerAndClientTestWithParams : public testing::TestWithParam<string> {
public:
	CServer server;
};

INSTANTIATE_TEST_CASE_P(VOIP, ServerAndClientTestWithParams,
	testing::Values("start"));

//, "pause", "stop", "continue"

//TEST_P(ServerAndClientTestWithParams, DISABLED_ExecuteAllControlCommands) {
//	auto command = GetParam();
//
//	CClient client1(1);
//	CClient client2(2);
//
//	std::future<CClient::SignalSet> fu1 =
//		std::async(std::launch::deferred, &CClient::listen_from_server, &client1);
//
//	std::future<CClient::SignalSet> fu2 =
//		std::async(std::launch::deferred, &CClient::listen_from_server, &client2);
//
//	Sleep(2000);
//	server.send_command_to_all_client(command);
//
//	//for (int i = 0; i < 3; i++) {
//	//	Sleep(2000);
//	//	server.send_command_to_all_client(command);
//	//}
//	
//	EXPECT_EQ(fu1.get(), CClient::kStart);
//	EXPECT_EQ(fu2.get(), CClient::kStart);
//}



TEST_F(ServerAndClientTest, PauseAllClient) {
	CClient client1(1);
	CClient client2(2);

	std::future<CClient::SignalSet> fu1 =
		std::async(std::launch::deferred, &CClient::listen_from_server, &client1);

	std::future<CClient::SignalSet> fu2 =
		std::async(std::launch::async, &CClient::listen_from_server, &client2);

	Sleep(100);
	server.send_command_to_all_client("pause");

	EXPECT_EQ(fu1.get(), CClient::kPause);
	EXPECT_EQ(fu2.get(), CClient::kPause);
}

TEST_F(ServerAndClientTest, PauseOneClient) {
	CClient client1(1);
	CClient client2(2);

	std::future<CClient::SignalSet> fu1 =
		std::async(std::launch::async, &CClient::listen_from_server, &client1);

	std::future<CClient::SignalSet> fu2 =
		std::async(std::launch::async, &CClient::listen_from_server, &client2);

	Sleep(100);
	server.send_command_to_client(1,"pause");

	EXPECT_EQ(fu1.get(), CClient::kPause);
	EXPECT_NE(fu2.get(), CClient::kPause);
}

TEST_F(ServerAndClientTest, StartAllClient) {
	CClient client1(1);
	CClient client2(2);

	std::future<CClient::SignalSet> fu1 =
		std::async(std::launch::async, &CClient::listen_from_server, &client1);

	std::future<CClient::SignalSet> fu2 =
		std::async(std::launch::async, &CClient::listen_from_server, &client2);

	Sleep(100);
	server.send_command_to_all_client("start");
	
	EXPECT_EQ(fu1.get(), CClient::kStart);
	EXPECT_EQ(fu2.get(), CClient::kStart);
}

TEST_F(ServerAndClientTest, StartASpecificClient) {
	CClient client1(1);
	CClient client2(2);

	std::future<CClient::SignalSet> fu1 =
		std::async(std::launch::async, &CClient::listen_from_server, &client1);

	std::future<CClient::SignalSet> fu2 =
		std::async(std::launch::async, &CClient::listen_from_server, &client2);

	Sleep(100);
	server.send_command_to_client(1, "start");

	EXPECT_EQ(fu1.get(), CClient::kStart);
	EXPECT_NE(fu2.get(), CClient::kStart);
}

TEST_F(ServerAndClientTest, ServerAssignTaskToClientAndGetResultBack) {
	const int task_num = 1;
	const int client_num = 3;
	CClient client[client_num];
	std::thread client_thread[client_num];

	for (int i = 0; i<task_num*client_num;i++) {
		server.add_new_task(Task(i + 1));
	}

	for (int i = 0; i < client_num; i++) {
		server.add_new_client(i+1);
		client[i] =  CClient(i + 1);
		client[i].start_flag = 1;
	}

	std::thread task_thread(&CServer::assign_tasks, &server);
	for (int i = 0; i < client_num; i++) {
		client_thread[i] = std::thread(&CClient::simulation_wrap, &client[i], task_num);
	}
	std::thread result_thread(&CServer::collect_result, &server, task_num*client_num);

	task_thread.join();
	for (int i = 0; i < client_num; i++) {
		client_thread[i].join();
	}
	result_thread.join();
}

TEST_F(ServerAndClientTest, MarkBreakdownClientAndResetItsTask) {
	auto this_moment = s_clock();
	Task new_task(1);
	server.add_new_client(2);
	server.clients[2].set_heartbeat(this_moment - MAX_HEARTBEAT_TIMEOUT - 100);
	server.clients[2].set_task(&new_task);

	new_task.set_in_computing();

	ASSERT_FALSE(server.clients[2].is_breakdown());
	ASSERT_TRUE(new_task.is_in_computing());

	server.mark_breakdown_client();

	EXPECT_TRUE(server.clients[2].is_breakdown());
	EXPECT_TRUE(new_task.is_not_start());
}

TEST_F(ServerAndClientTest, NoClientIsConnectedToServer) {
	EXPECT_TRUE(server.is_not_connect_to_client(1));
}

TEST_F(ServerAndClientTest, AddNewClientToServer) {
	server.add_new_client(2);
	EXPECT_FALSE(server.is_not_connect_to_client(2));
}

TEST_F(ServerAndClientTest, TestHeartbeat) {
	int max_num = 3;
	const int client_num = 5;
	CClient* client[client_num];
	std::thread *client_thread[client_num];

	std::thread server_thread(&CServer::receive_heartbeat, &server, client_num*max_num);
	
	for (int i = 0; i < client_num; i++) {
		client[i] = new CClient(i + 1);
		client_thread[i] = new std::thread(&CClient::send_heartbeat, client[i], max_num);
	}

	for (int i = 0; i < client_num; i++) {
		client_thread[i]->join();
	}
	server_thread.join();
	//TODO : delete pointers
}

