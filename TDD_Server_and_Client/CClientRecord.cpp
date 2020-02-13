#pragma warning(disable:4996)

#include "pch.h"
#include "CClientRecord.h"

const int ClientRecord::MAX_HEARTBEAT_TIMEOUT = 10000; //10 seconds

// constructor
ClientRecord::ClientRecord() : id_(0), status_(kFree), 
								ptask_(nullptr) 
{
	heartbeat_ = s_clock();
}

ClientRecord::ClientRecord(uint id) : id_(id), status_(kFree), 
								  ptask_(nullptr)
{
	heartbeat_ = s_clock();
}

uint ClientRecord::get_id() const{ return id_; }
void ClientRecord::set_id(uint id) { id_ = id; }

ClientRecord::ClientStatus ClientRecord::get_status() const { return status_; }
void ClientRecord::set_status(ClientStatus status) { status_ = status; }

Task* ClientRecord::get_task() const { return ptask_; }
void ClientRecord::set_task(Task *ptask) { ptask_ = ptask; }

void ClientRecord::set_breakdown() { set_status(kBreakdown); }
void ClientRecord::set_free() { set_status(kFree); }
void ClientRecord::set_in_computing() { set_status(kInComputing); }

bool ClientRecord::is_free() { return kFree == status_; }
bool ClientRecord::is_in_computing() { return kInComputing == status_; }
bool ClientRecord::is_breakdown() { return kBreakdown == status_; }

void ClientRecord::set_heartbeat(int64_t heartbeat) { heartbeat_ = heartbeat; }
int64_t ClientRecord::get_heartbeat() const { return heartbeat_; }

bool ClientRecord::is_timeout()
{
	return (s_clock() - get_heartbeat() > MAX_HEARTBEAT_TIMEOUT); 
}

