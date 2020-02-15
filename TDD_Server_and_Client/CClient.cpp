
#include "pch.h"
#include "CClient.h"

CClient::CClient(uint id, const string &ip, const string &port) :
	id_(id), context(1), heartbeat_sender(context, ZMQ_PUSH),
							ip_(ip), port_(port) 
{
	heartbeat_sender.connect(get_ip_address());
}

string CClient::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

void CClient::send_heartbeat(int max_num)
{
	int count = 0;
	std::string signal = "HEARTBEAT_" + std::to_string(id_);

	while (is_not_reach(max_num, count)) {
		s_send(heartbeat_sender, signal);
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
	}

	heartbeat_sender.close();
}

bool CClient::is_not_reach(int max_num, int &count)
{
	return max_num == REPEAT_FOREVER ? true : count++ < max_num;
}