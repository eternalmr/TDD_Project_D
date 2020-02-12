#include "pch.h"
#include "CClient.h"

const string ip("127.0.0.1");
const string port("8888");
//string ip_address = "tcp://" + ip + ":" + port;
class ClientTest : public testing::Test
{
public:
	ClientTest(): client_with_ip_set(ip),
				  client_with_ip_and_port_set(ip, port)
	{}

public:
	CClient default_client;
	CClient client_with_ip_set;
	CClient client_with_ip_and_port_set;
};


TEST_F(ClientTest, TestSetIPAndPort) {
	EXPECT_EQ(client_with_ip_and_port_set.get_ip_address(), "tcp://127.0.0.1:8888");
}

TEST_F(ClientTest, TestSetIP) {
	EXPECT_EQ(client_with_ip_set.get_ip_address(), "tcp://127.0.0.1:5555");
}

TEST_F(ClientTest, TestDefaultLocalIP) {
	EXPECT_EQ(default_client.get_ip_address(),"tcp://127.0.0.1:5555");
}



