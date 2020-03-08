#pragma once

#include "client_pch.h"
#include "../project_paramters.h"

class CClient {

public:
	enum SignalSet {
		kStart = 111, kStop = 222, kPause = 333,
		kContinue = 444, kUnknow = 555
	};

	
public:
	CClient(uint id = 1, const string &ip = default_client_ip, 
		const string &port = default_client_port);
	static CClient& get_instance();//singleton class entrance

	void set_id(uint id);
	uint get_id();
	void set_ip_address(const string ip);

	void connect_to_ip_address();

	void subscribe_specific_signal();

	SignalSet listen_from_server();
	bool is_irrelevant(const SignalSet &signal) const;
	void execute_control_command(SignalSet control_signal);

	string get_ip_address();

	void send_heartbeat(int max_num = REPEAT_FOREVER);

	void simulation_wrap(int task_num);
	int simulation(int input);

private:
	bool is_not_reach(int max_num, int &count);

private:
	uint id_;
	zmq::context_t context;
	zmq::socket_t heartbeat_sender;
	zmq::socket_t task_requester;
	zmq::socket_t result_sender;
	zmq::socket_t command_receiver;

	string ip_;
	string port_;


public:
	int start_flag;
	int pause_flag;
	int stop_flag;
};

typedef CClient::SignalSet Command;

