
#include "pch.h"
#include "CClient.h"
#include "CLogShow.h"
#include "resource.h"
#include "ClientUI.h"
#include "MainFrm.h"
#include "CDisplayView.h"


string heartbeat_ipaddress = "tcp://" + default_client_ip + ":" + heartbeat_port;
string command_ipaddress = "tcp://" + default_client_ip + ":" + command_port;
string task_ipaddress = "tcp://" + default_client_ip + ":" + task_port;
string result_ipaddress = "tcp://" + default_client_ip + ":" + result_port;

CClient::CClient(uint id, const string &ip, const string &port) :
	id_(id), context(1),
	heartbeat_sender(context, ZMQ_PUSH),
	task_requester(context, ZMQ_REQ),
	result_sender(context, ZMQ_PUSH),
	command_receiver(context, ZMQ_SUB),
	ip_(ip), port_(port),
	start_flag(0), pause_flag(0), stop_flag(0)
	,simulation_progress(0),current_task_id(0)
	,exit_flag(false),task_finished(false)
	,server_has_no_pending_tasks(true)
{
	connect_to_ip_address();
	subscribe_specific_signal();
	GetSystemTimes(&IdleTime, &KernelTime, &UserTime);
}

CClient::~CClient()
{
	heartbeat_sender.close();
	task_requester.close();
	result_sender.close();
	command_receiver.close();

	zmq_ctx_term(&context);
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
	heartbeat_sender.connect(heartbeat_ipaddress);
	task_requester.connect(task_ipaddress);
	result_sender.connect(result_ipaddress);
	command_receiver.connect(command_ipaddress);
}

void CClient::disconnect_to_ip_address()
{
	heartbeat_sender.disconnect(heartbeat_ipaddress);
	task_requester.disconnect(heartbeat_ipaddress);
	result_sender.disconnect(result_ipaddress);
	command_receiver.disconnect(command_ipaddress);
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
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,    start_filter, strlen(start_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, continue_filter, strlen(continue_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,    pause_filter, strlen(pause_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,     stop_filter, strlen(stop_filter));
}

void CClient::send_heartbeat(int max_num)
{
	int count = 0;
	string signal;
	CString str;

	while (is_not_reach(max_num, count) && !exit_flag) {
		signal = std::to_string(id_) + "_" 
			   + std::to_string(current_task_id) + "_" 
			   + std::to_string(simulation_progress);
		s_send(heartbeat_sender, signal);
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
	}
	OutputDebugString(TEXT("心跳线程已退出。"));
	//heartbeat_sender.close();
}

void CClient::receive_command()
{
	Command cmd;
	while (!exit_flag) {
		cmd = listen_from_server();
		if (is_irrelevant(cmd)) continue;
		execute_control_command(cmd);
	}
	OutputDebugString(TEXT("控制线程已退出。"));
}

void CClient::receive_tasks()
{
	int new_task_id;

	while (!exit_flag)
	{
		//判断server是否还有任务；
		//用一个标记位still_has_task来标记
		//若有则发出任务请求，没有则wait在此
		//（若是没有任务了，server则返回一条no_task的消息）
		//当server从没有任务，到收到新任务，从pub端口向client发一条消息
		//收到消息后，client重新将still_has_task标记置为true

		while (server_has_no_pending_tasks) {
			OutputDebugString(TEXT("server has no pending tasks\r\n"));
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		try {
			new_task_id = get_new_task_from_server();
		}
		catch (zmq::error_t &e) {
			OutputDebugString(CA2T(e.what()));
			continue;
		}
		catch (...) {
			OutputDebugString(TEXT("Unknown exception occur in CClient::receive_tasks"));
			continue;
		}

		if (new_task_id == -1) {
			server_has_no_pending_tasks = true;
			OutputDebugString(TEXT("no new task"));
			continue;
		}

		put_task_into_queue(new_task_id);
		wait_simulation_finish();
	}
	OutputDebugString(TEXT("任务接收线程已退出。"));
}


void CClient::wait_simulation_finish()
{
	task_finished = false;
	while (!task_finished) {
		std::this_thread::yield();
	}
}

void CClient::put_task_into_queue(int new_task_id)
{
	std::unique_lock<std::mutex> locker(queue_mtx);
	task_queue.push(new_task_id);
	locker.unlock();
	new_task_notifier.notify_one(); //通知仿真线程收到新任务
}

int CClient::get_new_task_from_server()
{
	CString str;
	s_send(task_requester, std::to_string(id_));
	string new_task = s_recv(task_requester);
	int new_task_id = std::atoi(new_task.c_str());
	str.Format(TEXT("Receive a new task: %d \r\n"), new_task_id);
	AddLog(str, TLP_NORMAL);
	return new_task_id;
}

void CClient::wrap_simulation_process(int task_num)
{
	int result;

	while (!exit_flag) {
		try {
			current_task_id = get_task_from_queue();
		}
		catch (...) {
			OutputDebugString(TEXT("终止仿真..."));
			break;
		}
		
		result = start_simulation(current_task_id);

		if (result == -1) { 
			reset_current_task_to_undo();
			clear_temp_simulation_data();
			AddLog(TEXT("current task has been interrupted"), TLP_ERROR);
			continue;
		}

		save_result_to_database(result);
		set_task_status_to_finished();
	}//end of while
	OutputDebugString(TEXT("仿真线程已退出"));
}

void CClient::set_task_status_to_finished()
{
	task_finished = true;
}

void CClient::save_result_to_database(int result)
{
	CString str;
	string result_info = std::to_string(current_task_id) + "_" + std::to_string(result);
	s_send(result_sender, result_info);
	str.Format(TEXT("Result of task[%d] is: %d\r\n"), current_task_id, result);
	AddLog(str, TLP_NORMAL);
}

void CClient::reset_current_task_to_undo()
{
	//TODO
}

void CClient::clear_temp_simulation_data()
{
	//TODO
}

uint CClient::get_task_from_queue()
{
	std::unique_lock<std::mutex> locker(queue_mtx);
	while (task_queue.empty() && !exit_flag) {
		new_task_notifier.wait(locker);
	}

	if (exit_flag)
		throw "exit process";

	uint task_id = task_queue.front();
	task_queue.pop();
	locker.unlock();
	return task_id;
}

int CClient::start_simulation(int task_id)
{
	int result = task_id;

	stop_flag = 0;
	set_progress(0);

	while (!start_flag) {
		std::this_thread::yield();
	}

	while (true) {
		if (stop_flag || exit_flag) 
			return -1;//interrupt simulation

		if (start_flag == 1 && pause_flag == 1) {
			std::this_thread::yield();
			continue;
		}

		result++;
		Sleep(SIM_DELAY);
		set_progress((result-task_id) * 100 / 5);

		if (has_reached_endpoint(task_id, result)) {
			stop_flag = 1;
			break;
		}
	}

	return result;
}

unsigned long long CClient::FileTimeSub(FILETIME ftEndTime, FILETIME ftStartTime)
{
	unsigned long long nEndTime = (unsigned long long)ftEndTime.dwHighDateTime << 32 | ftEndTime.dwLowDateTime;
	unsigned long long nStartTime = (unsigned long long)ftStartTime.dwHighDateTime << 32 | ftStartTime.dwLowDateTime;
	return nEndTime - nStartTime;
}

double CClient::get_cpu_status()
{
	FILETIME CurrentIdleTime, CurrentKernelTime, CurrentUserTime;

	GetSystemTimes(&CurrentIdleTime, &CurrentKernelTime, &CurrentUserTime);

	unsigned long long nDeltaIdleTime = FileTimeSub(CurrentIdleTime, IdleTime);
	unsigned long long nDeltaKernelTime = FileTimeSub(CurrentKernelTime, KernelTime);
	unsigned long long nDeltaUserTime = FileTimeSub(CurrentUserTime, UserTime);

	IdleTime = CurrentIdleTime;
	KernelTime = CurrentKernelTime;
	UserTime = CurrentUserTime;

	if (nDeltaKernelTime + nDeltaUserTime == 0) return 0;

	double cpu_status = ((double)(nDeltaKernelTime + nDeltaUserTime - nDeltaIdleTime) * 100) / (nDeltaKernelTime + nDeltaUserTime);
	return cpu_status;
}

double CClient::get_memoery_status()
{
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

	DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024);
	DWORDLONG avalid_memory = statex.ullAvailPhys / (1024 * 1024);
	DWORDLONG usePhys = physical_memory - avalid_memory;

	double memory_status = ((double)usePhys / (double)physical_memory) * 100;
	return memory_status;
}

uint CClient::get_progress()
{
	return simulation_progress;
}

void CClient::set_progress(uint percent)
{
	simulation_progress = percent;
}

uint CClient::get_task_id()
{
	return current_task_id;
}

void CClient::exit()
{
	//将所有端口的发送延迟和接收延迟设置为0秒，并关闭所有端口和context
	heartbeat_sender.setsockopt(ZMQ_LINGER,0);
	task_requester.setsockopt(ZMQ_LINGER, 0);
	result_sender.setsockopt(ZMQ_LINGER, 0);
	command_receiver.setsockopt(ZMQ_LINGER, 0);

	heartbeat_sender.setsockopt(ZMQ_RCVTIMEO, 0);
	task_requester.setsockopt(ZMQ_RCVTIMEO, 0);
	result_sender.setsockopt(ZMQ_RCVTIMEO, 0);
	command_receiver.setsockopt(ZMQ_RCVTIMEO, 0);

	stop_flag = 1;
	exit_flag = true;
}

CClient::SignalSet CClient::listen_from_server()
{
	std::string command;
	try {
		command = s_recv(command_receiver);
	}
	catch (zmq::error_t &e) {
		//e.what();
		CString str(e.what());
		OutputDebugString(str);
		return kUnknown;
	}
	catch (...)
	{
		return kUnknown;
	}

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
	return kUnknown;
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
		AddLog(TEXT("Start simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kContinue: {
		pause_flag = 0;
		AddLog(TEXT("Continue simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kPause: {
		pause_flag = 1;
		AddLog(TEXT("Pause simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kStop: {
		start_flag = 0;
		pause_flag = 0;
		stop_flag = 1;
		AddLog(TEXT("Stop simulation\r\n"), TLP_NORMAL);
		break;
	}
	default: {
		AddLog(TEXT("Unknown command\r\n"), TLP_NORMAL);
	}
	}//end of switch
}

bool CClient::is_not_reach(int max_num, int &count)
{
	return max_num == REPEAT_FOREVER ? true : count++ < max_num;
}

bool CClient::simulation_is_not_finished(int	task_num, int &count)
{
	return task_num == 0 ? true : count++ < task_num;
}


bool CClient::has_reached_endpoint(int input, int result)
{
	return (result - input == 5);
}