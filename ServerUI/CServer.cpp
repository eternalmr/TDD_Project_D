#include "pch.h"
#include "CServer.h"
#include "CClientRecord.h"
#include "CLogShow.h"

#define NO_TASK 0

string heartbeat_ipaddress = "tcp://" + default_server_ip + ":" + heartbeat_port;
string command_ipaddress = "tcp://" + default_server_ip + ":" + command_port;
string task_ipaddress = "tcp://" + default_server_ip + ":" + task_port;
string result_ipaddress = "tcp://" + default_server_ip + ":" + result_port;

CServer::CServer(const string &ip, const string &port) :
	context(1),
	heartbeat_receiver(context, ZMQ_PULL),
	task_assigner(context, ZMQ_REP),
	command_sender(context, ZMQ_PUB),
	result_collector(context, ZMQ_PULL),
	ip_(ip), port_(port), clients({}),
	exit_server(false),start_simulation(false)
{
	//bind_sockets_to_ip();
}

CServer::~CServer()
{
	zmq_ctx_term(&context);

	heartbeat_receiver.close();
	task_assigner.close();
	command_sender.close();
	result_collector.close();

	for (auto t: all_tasks)
	{
		delete t;
	}
}

CServer& CServer::get_instance()
{
	static CServer server;
	return server;
}

void CServer::bind_sockets_to_ip()
{
	heartbeat_receiver.bind(get_ip_address(ip_, std::to_string(heartbeat_port)));
	command_sender.bind(get_ip_address(ip_, std::to_string(control_port)));
	task_assigner.bind(get_ip_address(ip_, std::to_string(task_port)));
	result_collector.bind(get_ip_address(ip_, std::to_string(result_port)));
}

void CServer::unbind_sockets_to_ip()
{
	try {
		heartbeat_receiver.unbind(heartbeat_ipaddress);
	}
	catch (zmq::error_t &e) {
		OutputDebugString(CA2T(e.what()));
	}

	try {
		command_sender.unbind(command_ipaddress);
	}
	catch (zmq::error_t &e) {
		OutputDebugString(CA2T(e.what()));
	}

	try {
		task_assigner.unbind(task_ipaddress); 
	}
	catch (zmq::error_t &e) {
		OutputDebugString(CA2T(e.what()));
	}

	try {
		result_collector.unbind(result_ipaddress);
	}
	catch (zmq::error_t &e) {
		OutputDebugString(CA2T(e.what()));
	}
}

string CServer::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

string CServer::get_ip_address(string ip, string port)
{
	return "tcp://" + ip + ":" + port;
}

void CServer::set_ip(string ip)
{
	ip_ = ip;
}

void CServer::add_new_client(uint id)
{
	clients.insert(ClientMap::value_type(id, ClientRecord(id)));
}

void CServer::add_new_task(uint i)
{
	Task *pTask = new Task(i);
	all_tasks.push_back(pTask);
	undo_tasks.push_back(pTask);
}

void CServer::add_tasks(int num)
{
	for (int i = 1; i <= num; i++) {
		add_new_task(i);
	}
}

//========================= heartbeat related functions ==================================
void CServer::receive_heartbeat()
{
	uint client_id, task_id;
	uint simulation_progress;
	string raw_signal;

	while (!exit_server) {
		try {
			raw_signal = my_recv(heartbeat_receiver);
		}
		catch (zmq::error_t &e) {
			OutputDebugString(CA2T(e.what()));
			continue;
		}
		catch (...) {
			OutputDebugString(TEXT("Unknown exception occur"));
			continue;
		}

		std::tie(client_id, task_id, simulation_progress) = decode_signal_new(raw_signal);

		if (is_not_connect_to_client(client_id)){
			add_new_client(client_id);
		}
		update_client_info(client_id, task_id, simulation_progress);
	}
	OutputDebugString(TEXT("心跳线程已退出\r\n"));
}

bool CServer::is_not_reach(int max_num, int &count)
{
	return max_num == REPEAT_FOREVER ? true : count++ < max_num;
}

std::tuple<int, string> CServer::decode_signal(string &raw_signal)
{
	auto signal = split_string(raw_signal, "_");
	return std::make_tuple(std::stoi(signal[1]), signal[0]);//output is [client_id, signal]
}

std::tuple<int, int, int> CServer::decode_signal_new(string &raw_signal)
{
	auto signal = split_string(raw_signal, "_");
	return std::make_tuple(std::stoi(signal[0]), std::stoi(signal[1]), 
						   std::stoi(signal[2]));//[client_id, task_id, simulation_progress]
}

std::tuple<int, int> CServer::decode_result(string &raw_signal)
{
	auto signal = split_string(raw_signal, "_");
	return std::make_tuple(std::stoi(signal[0]), std::stoi(signal[1]));//[task_id, result]
}

//用delim指定的正则表达式将字符串in分割，返回分割后的字符串数组
//delim 分割字符串的格式为："[_/ ]+"
std::vector<string> CServer::split_string(const string& in, const string& delim)
{
	std::regex re{ delim };
	return std::vector<string> {
		std::sregex_token_iterator(in.begin(), in.end(), re, -1),
		std::sregex_token_iterator()
	};
}

void CServer::update_heartbeat_of_client(uint id)
{
	clients[id].set_heartbeat(s_clock());//set this moment as client's new heartbeat
}

void CServer::update_client_info(uint client_id, uint task_id, uint progress)
{
	clients[client_id].set_heartbeat(s_clock());//set this moment as client's new heartbeat
	
	if (task_id == NO_TASK)
		return;
	all_tasks[task_id-1]->set_simulation_progress(progress); //TODO: 可能将tasks也改为map类型的容器
}

bool CServer::is_not_connect_to_client(uint id)
{
	return clients.find(id) == clients.end() ? true : false;
}

//======================= assign task related functions ==========================
void CServer::mark_breakdown_client() //TODO:添加测试
{
	CString str;
	ClientRecord *pClient;

	for (auto &client : clients) {
		pClient = &(client.second);
		if (!pClient->is_timeout() || pClient->is_breakdown())
			continue;

		pClient->set_breakdown();
		str.Format(TEXT("Client[%d] is breakdown!\r\n"), pClient->get_id());
		AddLog(str, TLP_ERROR);

		reset_task_to_not_start(pClient->get_task());
	}
}

void CServer::reset_task_to_not_start(Task* pTask)
{
	if (pTask == nullptr || pTask->is_finished()) 
		return;

	pTask->reset_to_not_start();
	undo_tasks.push_front(pTask);

	CString str;
	str.Format(TEXT("Reset task[%d] status to not start!\r\n"), pTask->get_id());
	AddLog(str, TLP_ERROR);
}

void CServer::send_command_to_client(uint id, string command)
{
	s_send(command_sender, command + "_" + std::to_string(id));
}

void CServer::send_command_to_all_client(string command)
{
	s_send(command_sender, command);
}

void CServer::distribute_tasks()
{
	Task* undo_task_pointer;
	uint id;

	while (!exit_server) {//TODO : simulation is finished
		// update tasks and clients status
		while (!start_simulation) {
			std::this_thread::yield();
		}

		mark_breakdown_client(); //TODO : 根据单一责任原理，这个函数应该移出这里

		try {
			id = get_free_client();
		}
		catch (zmq::error_t &e) {
			OutputDebugString(CA2T(e.what()));
			continue;
		}

		undo_task_pointer = get_undo_task();
		assign_task_to_client(id, undo_task_pointer);
	}// end of while
	OutputDebugString(TEXT("任务分配线程已退出。"));
}


Task* CServer::get_undo_task()
{
	if (undo_tasks.empty())
		return nullptr;

	Task *ptask = undo_tasks.front();
	undo_tasks.pop_front();
	return ptask;
}

uint CServer::get_free_client()
{
	string reply;

	try {
		reply = my_recv(task_assigner);//reply client id
	}
	catch (zmq::error_t &e) {
		throw e;
	}

	uint id = stoi(reply);

	if (is_not_connect_to_client(id)) { //new client
		add_new_client(id);
	}
	else if (clients[id].is_in_computing()){ // already in clients pool
		clients[id].set_idle();
	}
	else if (clients[id].is_breakdown()) {
		clients[id].reset_idle();
	}

	return id;
}

void CServer::assign_task_to_client(uint id, Task* p_task)
{
	if (p_task == nullptr) {
		s_send(task_assigner, std::to_string(NO_TASK));
		return;
	}

	clients[id].set_task(p_task);
	int workload = p_task->get_id();
	s_send(task_assigner, std::to_string(workload));
	p_task->set_in_computing();
	clients[id].set_in_computing();

	CString str;
	str.Format(TEXT("Task[%d] is assigned to client[%d]\r\n"),
		p_task->get_id(), id);
	AddLog(str, TLP_NORMAL);
}

void CServer::collect_result()
{
	int task_id, result;
	string raw_result;
	CString str;

	while (!exit_server) {
		try {
			raw_result = my_recv(result_collector);
		}
		catch (zmq::error_t &e) {
			OutputDebugString(CA2T(e.what()));
			continue;
		}
		catch (...) {
			OutputDebugString(TEXT("Unknown exception occur"));
			continue;
		}
		
		std::tie(task_id, result) = decode_result(raw_result);
		all_tasks[task_id - 1]->set_finished();

		str.Format(TEXT("Task[%d] is accomplished, result is %d\r\n"), task_id, result);
		AddLog(str, TLP_NORMAL);
	}
	OutputDebugString(TEXT("结果收集线程已退出。"));
}


void CServer::start_threads()
{
	task_thread   = std::thread(&CServer::distribute_tasks, this);
	result_thread = std::thread(&CServer::collect_result, this);

	task_thread.detach();
	result_thread.detach();
}

void CServer::get_task_num_info(int &nTotal, int &nFinished, int &nIncomputing, int &nUndo)
{
	nTotal = Task::total_num;
	nFinished = Task::finished_num;
	nIncomputing = Task::in_computing_num;
	nUndo = nTotal - nFinished - nIncomputing;
}

void CServer::get_client_num_info(int &nTotal, int &nIncomputing, int &nIdle, int &nBreakdown)
{
	nTotal = ClientRecord::total_num;
	nIncomputing = ClientRecord::in_computing_num;
	nBreakdown = ClientRecord::breakdown_num;
	nIdle = nTotal - nIncomputing - nBreakdown;
}

void CServer::exit()
{
	exit_server = true;

	heartbeat_receiver.setsockopt(ZMQ_LINGER, 0);
	task_assigner.setsockopt(ZMQ_LINGER, 0);
	command_sender.setsockopt(ZMQ_LINGER, 0);
	result_collector.setsockopt(ZMQ_LINGER, 0);

	heartbeat_receiver.setsockopt(ZMQ_RCVTIMEO, 0);
	task_assigner.setsockopt(ZMQ_RCVTIMEO, 0);
	command_sender.setsockopt(ZMQ_RCVTIMEO, 0);
	result_collector.setsockopt(ZMQ_RCVTIMEO, 0);
	
	//unbind_sockets_to_ip();
}

std::string CServer::my_recv(zmq::socket_t & socket) 
{
	zmq::message_t message;

	try {
		socket.recv(&message);
	}
	catch (zmq::error_t &e) {
		throw e;
	}
	catch (...) {
		OutputDebugString(TEXT("Unknown exception occur."));
	}

	return std::string(static_cast<char*>(message.data()), message.size());
}

void CServer::reclaim_in_computing_task()
{

}
