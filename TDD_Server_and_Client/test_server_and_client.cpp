#include "pch.h"
#include "CServer.h"
#include "CClient.h"
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


TEST_F(ServerAndClientTest, AssignTaskToClient) {
	CClient client(1);

	server.add_new_client(1);
	server.add_new_task(Task(1));

	client.start_flag = 1;

	std::thread task_thread(&CServer::assign_tasks, &server);
	std::thread sim_thread(&CClient::simulation_wrap, &client, 1);

	task_thread.join();
	sim_thread.join();
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

