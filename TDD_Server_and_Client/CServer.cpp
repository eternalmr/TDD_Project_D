#include "pch.h"
#include "CServer.h"

CServer::CServer() : context(1), heartbeat_receiver(context, ZMQ_PULL),
	ip_(default_server_ip),port_(default_server_port)
{
	heartbeat_receiver.bind(get_ip_address());
}

CServer::CServer(const string &ip) : context(1), heartbeat_receiver(context,ZMQ_PULL),
	ip_(ip), port_(default_server_port)
{
	//socket.bind(get_ip_address());
}

CServer::CServer(const string &ip, const string &port) : context(1), heartbeat_receiver(context, ZMQ_PULL),
	ip_(ip), port_(port)
{

}

string CServer::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

void CServer::receive_heartbeat()
{
	int id;
	string heartbeat_signal;

	//heartbeat_receiver.bind("tcp://*:1217");

	while (true) {
		auto raw_signal = s_recv(heartbeat_receiver);
		std::tie(id, heartbeat_signal) = decode_signal(raw_signal);
		update_client_heartbeat(id);
	}
}

void CServer::receive_heartbeat_test_only()
{
	int id;
	string heartbeat_signal;

	int count = 0;
	while (count < 6) {
		auto raw_signal = s_recv(heartbeat_receiver);
		std::tie(id, heartbeat_signal) = decode_signal(raw_signal);

		if (is_not_connect_to_client(id)){
			add_new_client(id);
		}

		update_client_heartbeat(id);

		std::cout << "Heartbeat of client[" << clients[id].get_id() << "] : "
			<< clients[id].get_heartbeat() << std::endl;

		count++;
	}

	heartbeat_receiver.close();
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

void CServer::update_client_heartbeat(uint id)
{
	int64_t this_moment = s_clock();//TODO : get heartbeat moment from client
	clients[id].set_heartbeat(this_moment);
}

bool CServer::is_not_connect_to_client(uint id)
{
	auto it = clients.find(id);
	return it == clients.end() ? true : false;
}

void CServer::add_new_client(uint id)
{
	clients[id] = ClientRecord(id);
}
