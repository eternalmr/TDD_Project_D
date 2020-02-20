#include "pch.h"
#include "../Client/CClient.h"

const string ip("127.0.0.1");
const string port("8888");
//string ip_address = "tcp://" + ip + ":" + port;
class ClientTest : public testing::Test
{
public:
	ClientTest(): client_with_ip_set(1,ip),
				  client_with_ip_and_port_set(1, ip, port)
	{}

public:
	CClient client;
	CClient client_with_ip_set;
	CClient client_with_ip_and_port_set;
};

TEST_F(ClientTest, ExecuteContinueCommand) {
	client.execute_control_command(CClient::kStart);
	client.execute_control_command(CClient::kPause);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 1);
	ASSERT_EQ(client.start_flag, 1);

	client.execute_control_command(CClient::kContinue);

	ASSERT_EQ(client.start_flag, 1);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.stop_flag, 0);
}

TEST_F(ClientTest, ExecuteStopCommandWhilePause) {

	client.execute_control_command(CClient::kStart);
	client.execute_control_command(CClient::kPause);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 1);
	ASSERT_EQ(client.start_flag, 1);

	client.execute_control_command(CClient::kStop);

	ASSERT_EQ(client.start_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.stop_flag, 1);
}

TEST_F(ClientTest, ExecuteStopCommand) {

	client.execute_control_command(CClient::kStart);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.start_flag, 1);

	client.execute_control_command(CClient::kStop);

	ASSERT_EQ(client.start_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.stop_flag, 1);
}

TEST_F(ClientTest, ExecutePauseCommand) {

	client.execute_control_command(CClient::kStart);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.start_flag, 1);

	client.execute_control_command(CClient::kPause);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 1);
	EXPECT_EQ(client.start_flag, 1);
}

TEST_F(ClientTest, ExecuteStartCommand) {
	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	ASSERT_EQ(client.start_flag, 0);

	client.execute_control_command(CClient::kStart);

	ASSERT_EQ(client.stop_flag, 0);
	ASSERT_EQ(client.pause_flag, 0);
	EXPECT_EQ(client.start_flag, 1);
}

TEST_F(ClientTest, IsIrrelevantCommandSignals) {
	client.start_flag = 1;
	EXPECT_TRUE(client.is_irrelevant(CClient::kStart));

	client.start_flag = 0;
	EXPECT_TRUE(client.is_irrelevant(CClient::kPause));
	EXPECT_TRUE(client.is_irrelevant(CClient::kStop));

	client.pause_flag = 0;
	EXPECT_TRUE(client.is_irrelevant(CClient::kContinue));
}

TEST_F(ClientTest, IsValidCommandSignals) {
	EXPECT_FALSE(client.is_irrelevant(CClient::kStart));

	client.start_flag = 1;
	EXPECT_FALSE(client.is_irrelevant(CClient::kPause));
	EXPECT_FALSE(client.is_irrelevant(CClient::kStop));

	client.pause_flag = 1;
	EXPECT_FALSE(client.is_irrelevant(CClient::kContinue));
}

TEST_F(ClientTest, TestSetIPAndPort) {
	EXPECT_EQ(client_with_ip_and_port_set.get_ip_address(), "tcp://127.0.0.1:8888");
}

TEST_F(ClientTest, TestSetIP) {
	EXPECT_EQ(client_with_ip_set.get_ip_address(), "tcp://127.0.0.1:5555");
}

TEST_F(ClientTest, TestDefaultLocalIP) {
	EXPECT_EQ(client.get_ip_address(),"tcp://127.0.0.1:5555");
}



