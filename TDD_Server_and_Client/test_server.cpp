#include "pch.h"
#include "CServer.h"

const string ip("127.0.0.1");
const string port("8888");
//string ip_address = "tcp://" + ip + ":" + port;
class ServerTest : public testing::Test
{
public:
	ServerTest(): server_with_ip_set(ip),
				  server_with_ip_and_port_set(ip, port)
	{}

public:
	CServer default_server;
	CServer server_with_ip_set;
	CServer server_with_ip_and_port_set;
};

TEST_F(ServerTest, TestSetIPAndPort) {
	EXPECT_EQ(server_with_ip_and_port_set.get_ip_address(), "tcp://127.0.0.1:8888");
}

TEST_F(ServerTest, TestSetIP) {
	EXPECT_EQ(server_with_ip_set.get_ip_address(), "tcp://127.0.0.1:5555");
}

TEST_F(ServerTest, TestDefaultLocalIP) {
	EXPECT_EQ(default_server.get_ip_address(),"tcp://127.0.0.1:5555");
}



