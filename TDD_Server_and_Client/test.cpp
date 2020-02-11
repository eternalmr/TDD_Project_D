#include "pch.h"
#include "CServer.h"


const string ip("192.168.10.248");
const string port("8888");
class TestServer : public testing::Test
{

public:
	TestServer(): server_with_ip_set(ip),
				  server_with_ip_and_port_set(ip, port)
	{}

public:
	CServer default_server;
	CServer server_with_ip_set;
	CServer server_with_ip_and_port_set;
};

TEST_F(TestServer, TestSetIPAndPort) {
	EXPECT_EQ(server_with_ip_and_port_set.get_ip_address(), "tcp://192.168.10.248:8888");
}

TEST_F(TestServer, TestSetIP) {
	EXPECT_EQ(server_with_ip_set.get_ip_address(), "tcp://192.168.10.248:5555");
}

TEST_F(TestServer, TestDefaultLocalIP) {
	EXPECT_EQ(default_server.get_ip_address(),"tcp://127.0.0.1:5555");
}



