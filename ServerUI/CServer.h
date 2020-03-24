#pragma once
#pragma warning(disable:4996)

#include <regex>
#include <map>
#include <mutex>
#include <atomic>
#include <deque>
#include "../project_paramters.h"

class Task;
class ClientRecord;
typedef std::map<uint, ClientRecord> ClientMap;

class CServer {
private:
	CServer(const string &ip = default_server_ip, 
		const string &port = default_server_port);
	~CServer();

public:
	static CServer& get_instance();

	void bind_sockets_to_ip();
	void unbind_sockets_to_ip();

	string get_ip_address();
	string get_ip_address(string ip, string port);

	void receive_heartbeat();
	std::tuple<int, string> decode_signal(string &raw_signal);
	std::tuple<int, int, int> decode_signal_new(string &raw_signal);
	std::tuple<int, int> decode_result(string &raw_signal);
	std::vector<string> split_string(const string& in, const string& delim);
	void update_heartbeat_of_client(uint id);

	void update_client_info(uint client_id, uint task_id, uint progress);
	bool is_not_connect_to_client(uint id);
	void add_new_client(uint id);
	void add_new_task(uint i);
	void add_tasks(int num);

	void distribute_tasks();

	void assign_task_to_client(uint id, Task* undo_task_pointer);

	uint get_free_client();

	void mark_breakdown_client();

	void reset_task_to_not_start(Task* pTask);

	Task* get_undo_task();
	void send_command_to_client(uint id, string command);
	void send_command_to_all_client(string command);

	void collect_result();

	void start_threads();

	void get_task_num_info(int &nTotal, int &nCompleted, int &nIncomputing, int &nUndo);
	void get_client_num_info(int &nTotal, int &nIncomputing, int &nFree, int &nBreakdown);
	void exit();

	std::string my_recv(zmq::socket_t &socket);

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

	std::atomic<int> total_task_num;
	std::atomic<int> completed_task_num;
	std::atomic<int> in_computing_task_num;
	std::atomic<int> undo_task_num;

	std::atomic<int> total_client_num;
	std::atomic<int> in_computing_client_num;
	std::atomic<int> free_client_num;
	std::atomic<int> breakdown_client_num;
	
	bool exit_flag;
public:
	ClientMap clients;
	std::vector<Task*> all_tasks;
	std::deque<Task*> undo_tasks;

	std::thread heartbeat_thread;
	std::thread sim_thread;
	std::thread task_thread;
	std::thread result_thread;

};

