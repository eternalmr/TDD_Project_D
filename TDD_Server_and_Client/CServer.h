#pragma once

#include "pch.h"

const string default_server_ip{ "127.0.0.1" };
const string default_server_port{ "5555" };

class CServer {
public:
	CServer() : context(1), socket(context, ZMQ_PULL),
		ip_(default_server_ip),port_(default_server_port)
	{
		//socket.bind(get_ip_address());
	}

	explicit CServer(const string &ip) : context(1), socket(context,ZMQ_PULL),
		ip_(ip), port_(default_server_port)
	{
		//socket.bind(get_ip_address());
	}

	CServer(const string &ip, const string &port) : context(1), socket(context, ZMQ_PULL),
		ip_(ip), port_(port)
	{

	}

	string get_ip_address()
	{
		return "tcp://" + ip_ + ":" + port_;
	}

private:
	zmq::context_t context;
	zmq::socket_t socket;
	string ip_;
	string port_;
};

