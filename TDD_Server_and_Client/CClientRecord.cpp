#pragma warning(disable:4996)

#include "pch.h"
#include "CClientRecord.h"

const int ClientRecord::MAX_HEARTBEAT_TIMEOUT = 10000; //10 seconds

// constructor
ClientRecord::ClientRecord() : client_id_(0), client_status_(kFree), 
								ptask_(nullptr) 
{
	heartbeat_ = s_clock();
}

ClientRecord::ClientRecord(uint id) : client_id_(id), client_status_(kFree), 
								  ptask_(nullptr)
{
	heartbeat_ = s_clock();
}

uint ClientRecord::get_client_id() const { return client_id_; }
void ClientRecord::set_client_id(uint id) { client_id_ = id; }

ClientRecord::ClientStatus ClientRecord::get_client_status() const { return client_status_; }
void ClientRecord::set_client_status(ClientStatus status) { client_status_ = status; }

Task* ClientRecord::get_task() const { return ptask_; }
void ClientRecord::set_task(Task *ptask) { ptask_ = ptask; }

void ClientRecord::set_breakdown() { set_client_status(kBreakdown); }
void ClientRecord::set_status_free() { set_client_status(kFree); }
void ClientRecord::set_in_computing() { set_client_status(kInComputing); }

bool ClientRecord::is_free() { return kFree == client_status_; }
bool ClientRecord::is_in_computing() { return kInComputing == client_status_; }
bool ClientRecord::is_breakdown() { return kBreakdown == client_status_; }

void ClientRecord::set_heartbeat(int64_t heartbeat) { heartbeat_ = heartbeat; }
int64_t ClientRecord::get_heartbeat() const { return heartbeat_; }

bool ClientRecord::is_timeout()
{
	return (s_clock() - get_heartbeat() > MAX_HEARTBEAT_TIMEOUT); 
}

