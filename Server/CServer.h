#pragma once

#include "server_pch.h"
#include <regex>
#include <map>
#include "CClientRecord.h"
#include "../project_paramters.h"

typedef std::map<uint, ClientRecord> ClientMap;

class CServer {
public:
	CServer(const string &ip = default_server_ip, 
		const string &port = default_server_port);

	void bind_sockets_to_ip();
	void unbind_sockets_to_ip();

	string get_ip_address();

	void receive_heartbeat(int max_num = REPEAT_FOREVER);
	std::tuple<int, string> decode_signal(string &raw_signal);
	std::vector<string> split_string(const string& in, const string& delim);
	void update_heartbeat_of_client(uint id);

	bool is_not_connect_to_client(uint id);
	void add_new_client(uint id);
	void add_new_task(uint i);

	void assign_tasks();
	void mark_breakdown_client();
	Task* get_undo_task();

	void send_command_to_client(uint id, string command);
	void send_command_to_all_client(string command);

	void collect_result(uint max_num = REPEAT_FOREVER);

	void add_tasks(int num);
	void start_simulation();


private:
	bool is_not_reach(int max_num, int &count);

private:
	zmq::context_t context;
	zmq::socket_t heartbeat_receiver;
	zmq::socket_t task_assigner;
	zmq::socket_t command_sender;
	zmq::socket_t result_collector;

	string ip_;
	string port_;
	
public:
	ClientMap clients;
	std::vector<Task> tasks;
};

