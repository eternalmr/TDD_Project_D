
#include "pch.h"
#include "CClient.h"
#include "CLogShow.h"
#include "resource.h"

#define NO_TASK 0

CClient::CClient(uint id, const string &ip, const string &port) :
	id_(id), context(1),
	heartbeat_sender(context, ZMQ_PUSH),
	task_requester(context, ZMQ_REQ),
	result_sender(context, ZMQ_PUSH),
	command_receiver(context, ZMQ_SUB),
	ip_(ip), port_(port),
	start_flag(false), pause_flag(false), stop_flag(false)
	,simulation_progress(0),current_task_id(NO_TASK)
	,exit_flag(false),task_finished(false)
	,server_has_no_pending_tasks(true)
	,not_receive_new_tasks(false)
{
	//connect_to_ip_address();
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

void CClient::set_ip(const string ip)
{
	ip_ = ip;
}

void CClient::connect_sockets_to_ip()
{
	heartbeat_sender.connect(get_ip_address(std::to_string(heartbeat_port)));
	task_requester.connect(get_ip_address(std::to_string(task_port)));
	result_sender.connect(get_ip_address(std::to_string(result_port)));
	command_receiver.connect(get_ip_address(std::to_string(control_port)));
}

void CClient::disconnect_sockets_to_ip()
{
	heartbeat_sender.disconnect(get_ip_address(std::to_string(heartbeat_port)));
	task_requester.disconnect(get_ip_address(std::to_string(task_port)));
	result_sender.disconnect(get_ip_address(std::to_string(result_port)));
	command_receiver.disconnect(get_ip_address(std::to_string(control_port)));
}

string CClient::get_ip_address(string port)
{
	return "tcp://" + ip_ + ":" + port;
}

void CClient::subscribe_specific_signal()
{
	const char *start_filter = "start";
	const char *continue_filter = "continue";
	const char *pause_filter = "pause";
	const char *stop_filter = "stop";
	const char *task_filter = "newTask";
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,    start_filter, strlen(start_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE, continue_filter, strlen(continue_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,    pause_filter, strlen(pause_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,     stop_filter, strlen(stop_filter));
	command_receiver.setsockopt(ZMQ_SUBSCRIBE,	   task_filter, strlen(task_filter));
}

void CClient::send_heartbeat()
{
	string signal;

	while (!exit_flag) {
		signal = std::to_string(id_) + "_" 
			   + std::to_string(current_task_id) + "_" 
			   + std::to_string(simulation_progress);
		s_send(heartbeat_sender, signal);
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
	}
	OutputDebugString(TEXT("�����߳����˳���"));
}

void CClient::receive_control_command()
{
	Command cmd;
	while (!exit_flag) {
		cmd = listen_from_server();
		if (is_irrelevant(cmd)) continue;
		execute_control_command(cmd);
	}
	OutputDebugString(TEXT("�����߳����˳���"));
}

void CClient::receive_tasks()
{
	int new_task_id;

	while (!exit_flag)
	{
		//�ж�server�Ƿ�������
		//��һ�����λstill_has_task�����
		//�����򷢳���������û����wait�ڴ�
		//������û�������ˣ�server�򷵻�һ��no_task����Ϣ��
		//��server��û�����񣬵��յ������񣬴�pub�˿���client��һ����Ϣ
		//�յ���Ϣ��client���½�still_has_task�����Ϊtrue

		while (server_has_no_pending_tasks || not_receive_new_tasks) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		try {
			new_task_id = get_new_task_from_server();
		}
		catch (zmq::error_t &e) {
			OutputDebugString(CA2T(e.what()));
			break;
		}
		catch (...) {
			OutputDebugString(TEXT("Unknown exception occur in CClient::receive_tasks"));
			break;
		}

		if (new_task_id == NO_TASK) {
			current_task_id = NO_TASK;
			server_has_no_pending_tasks = true;
			continue;
		}

		put_task_into_queue(new_task_id);
		wait_simulation_finish();
	}
	OutputDebugString(TEXT("��������߳����˳���"));
}


void CClient::wait_simulation_finish()
{
	//task_finished = false;
	//while (!task_finished) {
	//	std::this_thread::yield();
	//}

	task_finished = false;
	std::unique_lock<std::mutex> locker(sim_mtx);
	while (!task_finished && !exit_flag) {
		sim_finished_notifier.wait(locker);
	}
}

void CClient::start_threads()
{
	heartbeat_thread = std::thread(&CClient::send_heartbeat, this);
	task_thread = std::thread(&CClient::receive_tasks, this);
	simulation_thread = std::thread(&CClient::wrap_simulation_process, this);
	control_thread = std::thread(&CClient::receive_control_command, this);

	task_thread.detach();
	control_thread.detach();
	simulation_thread.detach();
}

void CClient::put_task_into_queue(int new_task_id)
{
	std::unique_lock<std::mutex> locker(queue_mtx);
	task_queue.push(new_task_id);
	locker.unlock();
	new_task_notifier.notify_one(); //֪ͨ�����߳��յ�������
}

int CClient::get_new_task_from_server()
{
	CString str;
	s_send(task_requester, std::to_string(id_));
	string new_task = s_recv(task_requester);
	int new_task_id = std::atoi(new_task.c_str());

	if (new_task_id == NO_TASK) {
		server_has_no_pending_tasks = true;
		str.Format(TEXT("Server has no pending task right now\r\n"));
		AddLog(str, TLP_DETAIL);
	}
	else {
		str.Format(TEXT("Receive a new task: %d \r\n"), new_task_id);
		AddLog(str, TLP_NORMAL);
	}

	return new_task_id;
}

void CClient::wrap_simulation_process()
{
	int result;

	while (!exit_flag) {
		try {
			current_task_id = get_task_from_queue();
		}
		catch (...) {
			OutputDebugString(TEXT("��ֹ����..."));
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
	OutputDebugString(TEXT("�����߳����˳�"));
}

void CClient::set_task_status_to_finished()
{
	task_finished = true;
	sim_finished_notifier.notify_one();
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
	set_simulation_progress(0);
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

	stop_flag = false;
	set_simulation_progress(0);

	while (!start_flag) {
		std::this_thread::yield();
	}

	while (true) {
		if (stop_flag || exit_flag) 
			return -1;//interrupt simulation

		if (start_flag && pause_flag ) {
			std::this_thread::yield();
			continue;
		}

		result++;
		Sleep(SIM_DELAY);
		set_simulation_progress((result-task_id) * 100 / 5);

		if (has_reached_endpoint(task_id, result)) {
			stop_flag = true;
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

uint CClient::get_simulation_progress()
{
	return simulation_progress;
}

void CClient::set_simulation_progress(uint percent)
{
	simulation_progress = percent;
}

uint CClient::get_task_id()
{
	return current_task_id;
}

void CClient::exit()
{
	//�����ж˿ڵķ����ӳٺͽ����ӳ�����Ϊ0�룬���ر����ж˿ں�context
	heartbeat_sender.setsockopt(ZMQ_LINGER,0);
	task_requester.setsockopt(ZMQ_LINGER, 0);
	result_sender.setsockopt(ZMQ_LINGER, 0);
	command_receiver.setsockopt(ZMQ_LINGER, 0);

	heartbeat_sender.setsockopt(ZMQ_RCVTIMEO, 0);
	task_requester.setsockopt(ZMQ_RCVTIMEO, 0);
	result_sender.setsockopt(ZMQ_RCVTIMEO, 0);
	command_receiver.setsockopt(ZMQ_RCVTIMEO, 0);

	stop_flag = true;
	exit_flag = true;
}

CClient::SignalSet CClient::listen_from_server()
{
	std::string command;
	try {
		command = s_recv(command_receiver);
	}
	catch (zmq::error_t &e) {
		OutputDebugString(CA2T(e.what()));
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
	if (command == "newTask" ||
		command == "newTask_" + std::to_string(id_))
		return kNewTask;
	return kUnknown;
}

bool CClient::is_irrelevant(const SignalSet &signal) const
{
	if ((signal == kStart) && (!start_flag) && (!pause_flag))
		return false;
	if ((signal == kPause) && start_flag && (!pause_flag))
		return false;
	if ((signal == kStop) && start_flag)
		return false;
	if ((signal == kContinue) && start_flag && pause_flag)
		return false;
	if ((signal == kNewTask))
		return false;
	return true;
}

void CClient::execute_control_command(SignalSet control_signal)
{
	switch (control_signal) {
	case kStart: {
		start_flag = true;
		AddLog(TEXT("Start simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kContinue: {
		pause_flag = false;
		AddLog(TEXT("Continue simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kPause: {
		pause_flag = true;
		AddLog(TEXT("Pause simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kStop: {
		start_flag = false;
		pause_flag = false;
		stop_flag = true;
		AddLog(TEXT("Stop simulation\r\n"), TLP_NORMAL);
		break;
	}
	case kNewTask: {
		server_has_no_pending_tasks = false;
		AddLog(TEXT("Sever has new tasks\r\n"), TLP_NORMAL);
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