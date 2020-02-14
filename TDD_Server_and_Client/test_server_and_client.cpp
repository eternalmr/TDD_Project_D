#include "pch.h"
#include "CServer.h"
#include "CClient.h"

class ServerAndClientTest : public testing::Test {
public:
	CServer server;
};

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
	CClient client1(1);
	CClient client2(2);

	std::thread tServer(&CServer::receive_heartbeat_test_only, &server);
	std::thread tClient1(&CClient::send_heartbeat_test_only, &client1);
	std::thread tClient2(&CClient::send_heartbeat_test_only, &client2);

	tServer.join();
	tClient1.join();
	tClient2.join();
}

