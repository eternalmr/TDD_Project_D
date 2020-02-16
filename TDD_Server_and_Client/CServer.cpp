#include "pch.h"
#include "CServer.h"

CServer::CServer(const string &ip, const string &port) : 
	context(1), 
	heartbeat_receiver(context, ZMQ_PULL),
	task_assigner(context, ZMQ_REP), 
	command_sender(context, ZMQ_PUB),
	ip_(ip), 
	port_(port)
{
	//heartbeat_receiver.unbind()
}



void CServer::bind_sockets_to_ip()
{
	heartbeat_receiver.bind(get_ip_address());
	command_sender.bind("tcp://127.0.0.1:5556");
	task_assigner.bind("tcp://127.0.0.1:5560"); //TODO : 
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
	clients[id] = ClientRecord(id);
}

void CServer::add_new_task(Task task)
{
	tasks.push_back(task);
}

//========================= heartbeat related functions ==================================
void CServer::receive_heartbeat(int max_num)
{
	int id;
	int count = 0;
	string heartbeat_signal;

	while (is_not_reach(max_num, count)) {
		auto raw_signal = s_recv(heartbeat_receiver);
		std::tie(id, heartbeat_signal) = decode_signal(raw_signal);

		if (is_not_connect_to_client(id)){
			add_new_client(id);
		}
		update_heartbeat_of_client(id);

		std::cout << "Heartbeat of client[" << id << "] : "
			<< clients[id].get_heartbeat() << std::endl;
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
void CServer::mark_breakdown_client()
{
	for (int i = 0; i < clients.size(); i++) {
		if ( !clients[i].is_timeout() || clients[i].is_breakdown() )
			continue;

		clients[i].set_breakdown();
		std::cout << "Client[" << clients[i].get_id() 
				  << "] is breakdown!" << std::endl;
		
		if (clients[i].get_task()) {
			clients[i].get_task()->set_not_start();
			std::cout << "Reset task[" << clients[i].get_task()->get_id()
					  << "] status to not start" << std::endl;
		}
	}
}

Task* CServer::get_undo_task()
{
	Task *ptask = nullptr;
	for (int i = 0; i < tasks.size(); i++) {
		if (tasks[i].is_not_start()) {
			ptask = &tasks[i];
			break;
		}
	}
	return ptask;
}

void CServer::send_command_to_client(uint id, string command)
{
	// publish start command to client(id)
	s_send(command_sender, command + "_" + std::to_string(id));
}

void CServer::send_command_to_all_client(string command)
{
	// publish start command to all clients
	s_send(command_sender, command);
}

void CServer::assign_tasks()
{
	// TODO : check tasks and clients status

	int workload = 0;
	Task* undo_task_pointer;
	Task* ptask;
	while (true) {//TODO : simulation is finished
		// update tasks and clients status
		mark_breakdown_client();

		// get a undo task
		undo_task_pointer = get_undo_task();
		if (!undo_task_pointer) break; // all task is completed and stored, than break

		// TODO : a_free_worker = get_free_worker(workers queue)
		string reply = s_recv(task_assigner);//reply client id
		uint id = stoi(reply);
		clients[id].set_free();
		std::cout << "Receive request from client[" << reply << "]" << std::endl;

		ptask = clients[id].get_task();
		if (ptask != nullptr && (ptask->is_in_computing())) {
			ptask->set_finished();
			std::cout << "Task[" << ptask->get_id() << "] is accomplished by client["
				<< id << "]" << std::endl;
		}

		// TODO : assign_task_to(a_free_worker, a_undo_task)
		clients[id].set_task(undo_task_pointer);//update clients
		workload = undo_task_pointer->get_id();
		s_send(task_assigner, std::to_string(workload));
		undo_task_pointer->set_in_computing();
		clients[id].set_in_computing();
		std::cout << "Task[" << undo_task_pointer->get_id() << "] is assigned to client["
			<< id << "]" << std::endl;
	}// end of while

	std::cout << "All tasks is finished!" << std::endl;
}