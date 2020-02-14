
#include "pch.h"
#include "CClient.h"

CClient::CClient() : context(1), heartbeat_sender(context, ZMQ_PUSH),
					ip_(default_client_ip), port_(default_client_port)
{
	//socket.bind(get_ip_address());
}

CClient::CClient(const string &ip) : context(1), heartbeat_sender(context, ZMQ_PUSH),
											ip_(ip), port_(default_client_port)
{
	//socket.bind(get_ip_address());
}

CClient::CClient(const string &ip, const string &port) : context(1), heartbeat_sender(context, ZMQ_PUSH),
												ip_(ip), port_(port)
{

}

CClient::CClient(uint id) : id_(id), context(1), heartbeat_sender(context, ZMQ_PUSH),
							ip_(default_client_ip), port_(default_client_port) 
{
	heartbeat_sender.connect(get_ip_address());
}

string CClient::get_ip_address()
{
	return "tcp://" + ip_ + ":" + port_;
}

void CClient::send_heartbeat()
{
	//zmq::socket_t heartbeat_sender(context, ZMQ_PUSH);
	//heartbeat_sender.connect("tcp://localhost:1217");

	std::string signal = "HEARTBEAT_" + std::to_string(id_);
	// send heartbeat at regular interval
	while (true) {
		s_send(heartbeat_sender, signal);
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
	}

	heartbeat_sender.close();
}

void CClient::send_heartbeat_test_only()
{
	std::string signal = "HEARTBEAT_" + std::to_string(id_);
	// send heartbeat at regular interval
	int count = 0;
	while (count < 3) {
		s_send(heartbeat_sender, signal);
		std::this_thread::sleep_for(std::chrono::milliseconds(HEARTBEAT_INTERVAL));
		count++;
	}

	heartbeat_sender.close();
}
