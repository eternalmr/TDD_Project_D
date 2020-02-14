#include "pch.h"
#include "CServer.h"
#include "CClient.h"

class ServerAndClientTest : public testing::Test {
public:
	CServer server;
};

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





//TEST_F(ServerAndClientTest, AddClientsToServer) {
//
//}

