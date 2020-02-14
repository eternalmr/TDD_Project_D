#include "pch.h"
#include "CServer.h"

const string ip("127.0.0.1");
const string port("8888");

class ServerTest : public testing::Test
{
public:
	ServerTest(){ }

public:
	CServer default_server;
};

TEST_F(ServerTest, DecodeRawSignalWithSpecificFormat) {
	int client_id;
	string signal;
	string raw_signal = "Heartbeat_3";
	std::tie(client_id, signal) = default_server.decode_signal(raw_signal);
	EXPECT_EQ(client_id, 3);
	EXPECT_EQ(signal, "Heartbeat");
}

TEST_F(ServerTest, SplitStringUsingGivenDelimitter) {
	string str = "test_string/split function";
	string delimitter = "[_/ ]+";
	auto splitted_str = default_server.split_string(str, delimitter);
	EXPECT_EQ(splitted_str[0], "test");
	EXPECT_EQ(splitted_str[1], "string");
	EXPECT_EQ(splitted_str[2], "split");
	EXPECT_EQ(splitted_str[3], "function");
}

TEST_F(ServerTest, TestSetIPAndPort) {
	CServer server_with_ip_and_port_set(ip, port);
	EXPECT_EQ(server_with_ip_and_port_set.get_ip_address(), "tcp://127.0.0.1:8888");
}

TEST_F(ServerTest, TestSetIP) {
	CServer server_with_ip_set(ip,"5556");
	EXPECT_EQ(server_with_ip_set.get_ip_address(), "tcp://127.0.0.1:5556");
}

TEST_F(ServerTest, TestDefaultLocalIP) {
	EXPECT_EQ(default_server.get_ip_address(),"tcp://127.0.0.1:5555");
}



