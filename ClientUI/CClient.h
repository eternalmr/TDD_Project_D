#pragma once

#include "pch.h"
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "../project_paramters.h"

class CClient {

public:
	enum SignalSet {
		kStart = 111, kStop = 222, kPause = 333,
		kContinue = 444, kUnknown = 555, kNewTask
	};
	
private:
	CClient(uint id = 1, const string &ip = default_client_ip, 
		const string &port = default_client_port);

	~CClient();

public:
	static CClient& get_instance();//singleton class entrance

	void set_id(uint id);
	uint get_id();
	void set_ip(const string ip);

	void connect_sockets_to_ip();
	void disconnect_sockets_to_ip();
	void subscribe_specific_signal();

	SignalSet listen_from_server();
	bool is_irrelevant(const SignalSet &signal) const;
	void execute_control_command(SignalSet control_signal);

	string get_ip_address(string port);

	void send_heartbeat();
	void receive_control_command();
	void receive_tasks();
	void wait_simulation_finish();
	void start_threads();

	void put_task_into_queue(int new_task_id);

	int get_new_task_from_server();

	void wrap_simulation_process();

	void try_to_save_result(int result);

	void set_task_status_to_finished();

	void save_result_to_database(int result);
	void reset_current_task();
	void clear_temp_simulation_data();
	uint get_task_from_queue();

	int start_simulation(int input);
	double get_cpu_status();
	double get_memoery_status();

	uint get_simulation_progress();
	void set_simulation_progress(uint percent);

	uint get_task_id();

	void exit();

private:
	//bool is_not_reach(int max_num, int &count); 
	//bool simulation_is_not_finished(int task_num, int &count);
	bool simulation_is_not_finish();
	unsigned long long FileTimeSub(FILETIME ftEndTime, FILETIME ftStartTime);

	uint simulation_progress;

private:
	uint id_;
	zmq::context_t context;
	zmq::socket_t heartbeat_sender;
	zmq::socket_t task_requester;
	zmq::socket_t result_sender;
	zmq::socket_t command_receiver;

	std::queue<int> task_queue;
	uint current_task_id;

	string ip_;
	string port_;

	FILETIME IdleTime, KernelTime, UserTime;

	std::mutex queue_mtx, sim_mtx;
	std::condition_variable new_task_notifier;
	std::condition_variable sim_finished_notifier;
	std::atomic<bool> task_finished;

public:
	bool start_task;
	bool pause_task;
	bool stop_task;
	bool exit_client;
	bool server_has_no_pending_tasks;
	bool not_receive_new_tasks;

	std::thread heartbeat_thread;
	std::thread task_thread;
	std::thread simulation_thread;
	std::thread control_thread;

	int heartbeat_port;
	int control_port;
	int task_port;
	int result_port;
};

typedef CClient::SignalSet Command;

