#pragma once

#include "pch.h"
#include <regex>
#include <map>
#include "CClientRecord.h"
#include "project_paramters.h"

typedef std::map<uint, ClientRecord> ClientMap;

class CServer {
public:
	CServer();
	CServer(const string &ip);
	CServer(const string &ip, const string &port);

	string get_ip_address();

	void receive_heartbeat();
	void receive_heartbeat_test_only();
	std::tuple<int, string> decode_signal(string &raw_signal);
	std::vector<string> split_string(const string& in, const string& delim);
	void update_client_heartbeat(uint id);

	bool is_not_connect_to_client(uint id);
	void add_new_client(uint id);

private:
	zmq::context_t context;
	zmq::socket_t heartbeat_receiver;
	string ip_;
	string port_;
	ClientMap clients;
};

