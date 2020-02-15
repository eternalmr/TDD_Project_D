#pragma once

#include "pch.h"
#include "project_paramters.h"

class CClient {
public:
	CClient(uint id = 1, const string &ip = default_client_ip, 
		const string &port = default_client_port);

	string get_ip_address();

	void send_heartbeat(int max_num = REPEAT_FOREVER);

private:
	bool is_not_reach(int max_num, int &count);

private:
	uint id_;
	zmq::context_t context;
	zmq::socket_t heartbeat_sender;
	string ip_;
	string port_;
};



