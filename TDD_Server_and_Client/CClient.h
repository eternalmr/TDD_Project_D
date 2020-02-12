#pragma once

#include "pch.h"

const string default_client_ip{ "127.0.0.1" };
const string default_client_port{ "5555" };

class CClient {
public:
	CClient() : context(1), socket(context, ZMQ_PUSH),
		ip_(default_client_ip), port_(default_client_port)
	{
		//socket.bind(get_ip_address());
	}

	explicit CClient(const string &ip) : context(1), socket(context, ZMQ_PUSH),
		ip_(ip), port_(default_client_port)
	{
		//socket.bind(get_ip_address());
	}

	CClient(const string &ip, const string &port) : context(1), socket(context, ZMQ_PUSH),
		ip_(ip), port_(port)
	{

	}

	string get_ip_address()
	{
		return "tcp://" + ip_ + ":" + port_;
	}

private:
	uint id_;
	zmq::context_t context;
	zmq::socket_t socket;
	string ip_;
	string port_;
};

