#pragma once
#include <string>

using std::string;


const string default_server_ip{ "127.0.0.1" };
const string default_server_port{ "5555" };

const string default_client_ip{ "127.0.0.1" };
const string default_client_port{ "5555" };

const int HEARTBEAT_INTERVAL = 100;

const int MAX_HEARTBEAT_TIMEOUT = 1000; //10 seconds

const int REPEAT_FOREVER = 0;

const int SIM_DELAY = 100; //ms