#pragma once

#include "pch.h"
#include <regex>
#include <map>
#include "CClientRecord.h"
#include "project_paramters.h"

typedef std::map<uint, ClientRecord> ClientMap;

class CServer {
public:
	CServer(const string &ip = default_server_ip, 
		const string &port = default_server_port);

	string get_ip_address();

	void receive_heartbeat(int max_num = REPEAT_FOREVER);
	std::tuple<int, string> decode_signal(string &raw_signal);
	std::vector<string> split_string(const string& in, const string& delim);
	void update_heartbeat_of_client(uint id);

	bool is_not_connect_to_client(uint id);
	void add_new_client(uint id);
	void add_new_task(Task task);

	void mark_breakdown_client();
	Task* get_undo_task();

private:
	bool CServer::is_not_reach(int max_num, int &count);

private:
	zmq::context_t context;
	zmq::socket_t heartbeat_receiver;
	string ip_;
	string port_;
	
public:
	ClientMap clients;
	std::vector<Task> tasks;
};

