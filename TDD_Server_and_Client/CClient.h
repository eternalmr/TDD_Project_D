#pragma once

#include "pch.h"
#include "project_paramters.h"

class CClient {
public:
	CClient();
	CClient(const string &ip);
	CClient(const string &ip, const string &port);
	CClient(uint id);

	string get_ip_address();

	void send_heartbeat();
	void send_heartbeat_test_only();

private:
	uint id_;
	zmq::context_t context;
	zmq::socket_t heartbeat_sender;
	string ip_;
	string port_;
};



