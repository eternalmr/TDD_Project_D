#include "pch.h"
#include "../Server/CClientRecord.h"
#include "../project_paramters.h"

class ClientRecordTest : public testing::Test
{
public:
	ClientRecordTest() : client(1) {}

public:
	ClientRecord client;
};

TEST_F(ClientRecordTest, ClientIsTimeoutIfHeartbeatExceedLimit) {
	int64_t this_moment = s_clock();
	client.set_heartbeat(this_moment - MAX_HEARTBEAT_TIMEOUT - 1); // MAX_HEARTBEAT_TIMEOUT = 10000
	EXPECT_TRUE(client.is_timeout());

	client.set_heartbeat(this_moment - MAX_HEARTBEAT_TIMEOUT + 1); // MAX_HEARTBEAT_TIMEOUT = 10000
	EXPECT_FALSE(client.is_timeout());
}

TEST_F(ClientRecordTest, SetHeartbeatToThisMoment) {
	int64_t this_moment = s_clock();
	client.set_heartbeat(this_moment);
	EXPECT_EQ(client.get_heartbeat(), this_moment);
}

TEST_F(ClientRecordTest, SetATaskToClient) {
	Task new_task(1);
	client.set_task(&new_task);
	EXPECT_EQ(client.get_task(), &new_task);
}

TEST_F(ClientRecordTest, SetClientStatusToFree) {
	client.set_free();
	EXPECT_TRUE(client.is_free());
}

TEST_F(ClientRecordTest, SetClientStatusToBreakdown) {
	client.set_breakdown();
	EXPECT_TRUE(client.is_breakdown());
}

TEST_F(ClientRecordTest, SetClientStatusToInComputing) {
	client.set_in_computing();
	EXPECT_TRUE(client.is_in_computing());
}

TEST_F(ClientRecordTest, ResetClientIdAfterInitialization) {
	uint new_id = 5u;
	client.set_id(new_id);
	EXPECT_EQ(client.get_id(), new_id);
}

TEST_F(ClientRecordTest, TestInitialStatus) {
	EXPECT_TRUE(client.is_free());
	EXPECT_EQ(client.get_task(), nullptr);
}

TEST_F(ClientRecordTest, TestGetClientId) {
	EXPECT_EQ(client.get_id(), 1);
}