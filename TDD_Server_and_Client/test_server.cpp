#include "pch.h"
#include "../Server/CServer.h"

const string ip("127.0.0.1");
const string port("8888");

class ServerTest : public testing::Test
{
public:
	ServerTest(){ }

public:
	CServer server;
};

TEST_F(ServerTest, OneClientIsConnectToServer) {
	EXPECT_TRUE(server.is_not_connect_to_client(0));
	EXPECT_TRUE(server.is_not_connect_to_client(1));
	server.add_new_client(1);
	EXPECT_TRUE(server.is_not_connect_to_client(0));
	EXPECT_FALSE(server.is_not_connect_to_client(1));
	EXPECT_EQ(server.clients.size(), 1);
}

TEST_F(ServerTest, NewServerIsNotConnectToClient) {
	EXPECT_TRUE(server.is_not_connect_to_client(0));
	EXPECT_TRUE(server.is_not_connect_to_client(1));
}

TEST_F(ServerTest, AddNewTaskIntoTaskPool) {
	uint num = 10;
	for (uint i = 1; i <= num; i++) {
		server.add_new_task(i);
	}
	EXPECT_EQ(num, server.tasks.size());
}

TEST_F(ServerTest, ReturnNullptrIfAllTasksIsCompleted) {
	for (uint i = 1; i <= 10; i++) {
		server.add_new_task(i);
		server.tasks[i - 1].set_in_computing();
	}
	EXPECT_EQ(server.get_undo_task(), nullptr);
}

TEST_F(ServerTest, GetAnUndoTask) {
	for (uint i = 1; i <= 10; i++) {
		server.add_new_task(i);
		server.tasks[i-1].set_in_computing();
	}
	server.tasks[6].set_not_start();
	EXPECT_EQ(server.get_undo_task(), &server.tasks[6]);
}

TEST_F(ServerTest, DecodeRawSignalWithSpecificFormat) {
	int client_id;
	string signal;
	string raw_signal = "Heartbeat_3";
	std::tie(client_id, signal) = server.decode_signal(raw_signal);
	EXPECT_EQ(client_id, 3);
	EXPECT_EQ(signal, "Heartbeat");
}

TEST_F(ServerTest, SplitStringUsingGivenDelimitter) {
	string str = "test_string/split function";
	string delimitter = "[_/ ]+";
	auto splitted_str = server.split_string(str, delimitter);
	EXPECT_EQ(splitted_str[0], "test");
	EXPECT_EQ(splitted_str[1], "string");
	EXPECT_EQ(splitted_str[2], "split");
	EXPECT_EQ(splitted_str[3], "function");
}

TEST_F(ServerTest, DISABLED_TestSetIPAndPort) {
	CServer server_with_ip_and_port_set(ip, port);
	EXPECT_EQ(server_with_ip_and_port_set.get_ip_address(), "tcp://127.0.0.1:8888");
}

TEST_F(ServerTest, DISABLED_TestSetIP) {
	CServer server_with_ip_set(ip,"5556");
	EXPECT_EQ(server_with_ip_set.get_ip_address(), "tcp://127.0.0.1:5556");
}

TEST_F(ServerTest, DISABLED_TestDefaultLocalIP) {
	EXPECT_EQ(server.get_ip_address(),"tcp://127.0.0.1:5555");
}



