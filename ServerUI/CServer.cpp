#include "pch.h"
#include "CServer.h"
#include "CClientRecord.h"
#include "CLogShow.h"

CServer::CServer(const string &ip, const string &port) :
	context(1),
	heartbeat_receiver(context, ZMQ_PULL),
	task_assigner(context, ZMQ_REP),
	command_sender(context, ZMQ_PUB),
	result_collector(context, ZMQ_PULL),
	ip_(ip), port_(port), clients({}),
	total_task_num(0), completed_task_num(0),
	in_computing_task_num(0), undo_task_num(total_task_num)
{
	bind_sockets_to_ip();
}

CServer& CServer::get_instance()
{
	static CServer server;
	return server;
}

void CServer::bind_sockets_to_ip()
{
	heartbeat_receiver.bind("tcp://127.0.0.1:1217");
	command_sender.bind("tcp://127.0.0.1:5555");
	task_assigner.bind("tcp://127.0.0.1:5560"); //TODO : 
	result_collector.bind("tcp://127.0.0.1:5558");
}

void CServer::unbind_sockets_to_ip()
{
	heartbeat_receiver.unbind(get_ip_address());
	command_sender.unbind("tcp://127.0.0.1:5556");
	task_assigner.unbind("tcp://127.0.0.1:5560"); //TODO : 
}

string CServer::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

void CServer::add_new_client(uint id)
{
	clients.insert(ClientMap::value_type(id, ClientRecord(id)));
}

void CServer::add_new_task(uint i)
{
	tasks.push_back(Task(i));
	total_task_num++;
}

void CServer::add_tasks(int num)
{
	for (int i = 1; i <= num; i++) {
		add_new_task(i);
	}
}
//========================= heartbeat related functions ==================================
void CServer::receive_heartbeat(int max_num)
{
	uint id;
	int count = 0;
	string heartbeat_signal;

	while (is_not_reach(max_num, count)) {
		auto raw_signal = s_recv(heartbeat_receiver);
		std::tie(id, heartbeat_signal) = decode_signal(raw_signal);

		if (is_not_connect_to_client(id)){
			add_new_client(id);
		}
		update_heartbeat_of_client(id);

		//std::cout << "Heartbeat of client[" << id << "] : "
		//	<< clients[id].get_heartbeat() << std::endl;

		//TODO : 加个线程的退出条件
	}
	heartbeat_receiver.close();
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

		if (pClient->get_task()) {
			pClient->get_task()->set_not_start();
			in_computing_task_num--;
			str.Format(TEXT("Reset task[%d] status to not start!\r\n"), 
				pClient->get_task()->get_id());
			AddLog(str, TLP_ERROR);
		}
	}
}



void CServer::send_command_to_client(uint id, string command)
{
	// publish command to client(id)
	s_send(command_sender, command + "_" + std::to_string(id));
}

void CServer::send_command_to_all_client(string command)
{
	// publish command to all clients
	s_send(command_sender, command);
}

void CServer::assign_tasks()
{
	// TODO : check tasks and clients status

	Task* undo_task_pointer;

	while (true) {//TODO : simulation is finished
		// update tasks and clients status
		mark_breakdown_client(); //TODO : 根据单一责任原理，这个函数应该移出这里

		undo_task_pointer = get_undo_task();
		if (!undo_task_pointer)
			break; // all task is completed and stored, than break

		uint id = get_free_client();

		assign_task_to(id, undo_task_pointer);

	}// end of while

	std::cout << "All tasks is finished!" << std::endl;
}

Task* CServer::get_undo_task()
{
	Task *ptask = nullptr;
	for (int i = 0; i < tasks.size(); i++) {//TODO : 可以优化不用每次从头开始查找
		if (tasks[i].is_not_start()) {
			ptask = &tasks[i];
			break;
		}
	}
	return ptask;
}

uint CServer::get_free_client()
{
	Task* ptask;
	CString str;

	string reply = s_recv(task_assigner);//reply client id
	uint id = stoi(reply);
	//str = CString(TEXT("Receive request from client[")) 
	//	+ CA2T(reply.c_str()) + CString(TEXT("]\r\n"));
	//AddLog(str, TLP_NORMAL);

	if (is_not_connect_to_client(id)) { //new client
		add_new_client(id);
	}
	else { // already in clients pool
		clients[id].set_free();
		ptask = clients[id].get_task();

		if (ptask != nullptr && (ptask->is_in_computing())) {
			ptask->set_finished();
			str.Format(TEXT("Task[%d] is accomplished by client[%d]\r\n"),
				ptask->get_id(), id);
			AddLog(str, TLP_NORMAL);
		}
	}		
	return id;
}

void CServer::assign_task_to(uint id, Task* p_task)
{
	clients[id].set_task(p_task);//update clients
	int workload = p_task->get_id();
	s_send(task_assigner, std::to_string(workload));
	p_task->set_in_computing();
	clients[id].set_in_computing();

	mtx.lock();
	in_computing_task_num++;
	mtx.unlock();

	CString str;
	str.Format(TEXT("Task[%d] is assigned to client[%d]\r\n"),
		p_task->get_id(), id);
	AddLog(str, TLP_NORMAL);
}

void CServer::collect_result(uint max_num)
{
	int count = 0;
	// Collect result from workers
	std::string result;
	while (is_not_reach(max_num, count)) {
		result = s_recv(result_collector);
		mtx.lock();
		in_computing_task_num--;
		completed_task_num++;
		mtx.unlock();
		std::cout << result << std::endl;
	}
}

void CServer::start_simulation()
{
	std::thread      task_thread(&CServer::assign_tasks, this);
	std::thread    result_thread(&CServer::collect_result, this, REPEAT_FOREVER);
	std::thread heartbeat_thread(&CServer::receive_heartbeat, this, REPEAT_FOREVER);

	char command;
	while (true) {
		std::cout << "Please input your command: ";
		std::cin >> command;

		if (command == 's') {
			send_command_to_all_client("start");
		}
		else if (command == 'c') {
			send_command_to_all_client("continue");
		}
		else if (command == 'p') {
			send_command_to_all_client("pause");
		}
		else if (command == 'e') {
			send_command_to_all_client("stop");
			std::cout << "Simulation stop!" << std::endl;
			break;
		}
		else {
			std::cout << "Wrong command!" << std::endl;
			continue;
		}
	}

	task_thread.join();
	result_thread.join();
	heartbeat_thread.join();
}

void CServer::start_threads()
{
	std::thread      task_thread(&CServer::assign_tasks, this);
	std::thread    result_thread(&CServer::collect_result, this, REPEAT_FOREVER);
	std::thread heartbeat_thread(&CServer::receive_heartbeat, this, REPEAT_FOREVER);

	if (task_thread.joinable()) task_thread.join();
	if (result_thread.joinable()) result_thread.join();
	if (heartbeat_thread.joinable()) heartbeat_thread.join();
}

void CServer::get_task_num_info(int &total, int &completed, int &incomputing, int &undo)
{
	total = total_task_num;
	completed = completed_task_num;
	incomputing = in_computing_task_num;
	undo = total - completed - incomputing;
}
