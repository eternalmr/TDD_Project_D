
#include "client_pch.h"
#include "CClient.h"



CClient::CClient(uint id, const string &ip, const string &port) :
	id_(id), context(1),
	heartbeat_sender(context, ZMQ_PUSH),
	task_requester(context, ZMQ_REQ),
	result_sender(context, ZMQ_PUSH),
	command_receiver(context, ZMQ_SUB),
	ip_(ip), port_(port),
	start_flag(0), pause_flag(0), stop_flag(0)
{
	connect_to_ip_address();
	subscribe_specific_signal();
}

CClient& CClient::get_instance()
{
	static CClient client;
	return client;
}

void CClient::set_id(uint id)
{
	id_ = id;
}

uint CClient::get_id()
{
	return id_;
}

void CClient::set_ip_address(const string ip)
{
	ip_ = ip;
}

void CClient::connect_to_ip_address()
{
	heartbeat_sender.connect("tcp://localhost:1217");
	task_requester.connect("tcp://localhost:5560");
	result_sender.connect("tcp://localhost:5558");
	command_receiver.connect("tcp://localhost:5555");
}

string CClient::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

void CClient::subscribe_specific_signal()
{
	const char *start_filter = "start";
	const char *continue_filter = "continue";
	const char *pause_filter = "pause";
	const char *stop_filter = "stop";
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, start_filter, strlen(start_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, continue_filter, strlen(continue_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, pause_filter, strlen(pause_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, stop_filter, strlen(stop_filter));
}

void CClient::send_heartbeat(int max_num)
{
	int count = 0;
	std::string signal = "HEARTBEAT_" + std::to_string(id_);

	while (is_not_reach(max_num, count)) {
		s_send(heartbeat_sender, signal);
		//cout << "send heartbeat to server: " << id_ << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
	}

	heartbeat_sender.close();
}

bool CClient::is_not_reach(int max_num, int &count)
{
	return max_num == REPEAT_FOREVER ? true : count++ < max_num;
}

bool simulation_is_not_finished(int	task_num, int &count)
{
	return task_num == 0 ? true : count++ < task_num;
}

bool has_reached_endpoint(int input, int result)
{
	return (result - input == 5);
}

void CClient::simulation_wrap(int task_num)
{
	int task_input;
	int result;
	int count = 0;
	while (simulation_is_not_finished(task_num, count)) {
		// Send client id to server
		s_send(task_requester, std::to_string(id_));

		// Receive a task from server
		string new_task = s_recv(task_requester);
		std::cout << "**********************************************" << std::endl;
		std::cout << "Receive a new task: " << new_task << std::endl;

		// Do some work
		stop_flag = 0; //reset stop flag
		task_input = atoi(new_task.c_str());
		result = simulation(task_input);

		if (result == -1) {
			std::cout << "Simulation interrupt" << std::endl;
			break;
		}

		//Send results to sink
		string result_info = "Result of task[" + new_task
			+ "] is: " + std::to_string(result);
		s_send(result_sender, result_info);

		std::cout << "**********************************************" << std::endl;
	}//end of while
 	std::cout << "client: simulation finished" << std::endl;
	task_requester.close();
}

int CClient::simulation(int input)
{
	int result = input;

	while (!start_flag) {
		std::this_thread::yield();
	}

	while (true) {
		if (stop_flag) return -1;//interrupt simulation

		if (start_flag == 1 && pause_flag == 1) {
			std::this_thread::yield();
			continue;
		}

		result++;
		Sleep(SIM_DELAY);// sleep 1000 millisecond
		std::cout << "Result: " << result << std::endl;

		if (has_reached_endpoint(input, result)) {
			stop_flag = 1;
			std::cout << "Task finished!" << std::endl;
			break;
		}
	}

	return result;
}

CClient::SignalSet CClient::listen_from_server()
{
	std::string command = s_recv(command_receiver);

	if (command == "start" || 
		command == "start_" + std::to_string(id_))
		return kStart;
	if (command == "pause" || 
		command == "pause_" + std::to_string(id_))
		return kPause;
	if (command == "stop" ||
		command == "stop_" + std::to_string(id_))
		return kStop;
	if (command == "continue" || 
		command == "continue_" + std::to_string(id_))
		return kContinue;
	return kUnknow;
}

bool CClient::is_irrelevant(const SignalSet &signal) const
{
	if ((signal == kStart) && (start_flag == 0 && pause_flag == 0))
		return false;
	if ((signal == kPause) && (start_flag == 1 && pause_flag == 0))
		return false;
	if ((signal == kStop) && (start_flag == 1))
		return false;
	if ((signal == kContinue) && (start_flag == 1 && pause_flag == 1))
		return false;
	return true;
}

void CClient::execute_control_command(SignalSet control_signal)
{
	switch (control_signal) {
	case kStart: {
		start_flag = 1;
		cout << "Start simulation" << endl;
		break;
	}
	case kContinue: {
		pause_flag = 0;
		cout << "Continue simulation" << endl;
		break;
	}
	case kPause: {
		pause_flag = 1;
		cout << "Pause simulation" << endl;
		break;
	}
	case kStop: {
		start_flag = 0;
		pause_flag = 0;
		stop_flag = 1;
		cout << "Stop simulation" << endl;
		break;
	}
	default: {
		cout << "Unknown command" << endl;
	}
	}//end of switch
}