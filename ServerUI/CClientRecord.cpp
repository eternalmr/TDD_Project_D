#pragma warning(disable:4996)

#include "pch.h"
#include "CTask.h"
#include "CClientRecord.h"

uint ClientRecord::total_num = 0;
uint ClientRecord::in_computing_num = 0;
uint ClientRecord::breakdown_num = 0;

// constructor
ClientRecord::ClientRecord(uint id) : 
	id_(id), status_(kIdle), ptask_(nullptr)
{
	heartbeat_ = s_clock();
	total_num++;
}

ClientRecord::ClientRecord() : id_(0), status_(kIdle), ptask_(nullptr)
{
	heartbeat_ = s_clock();
	total_num++;
}

uint ClientRecord::get_id() const{ return id_; }
void ClientRecord::set_id(uint id) { id_ = id; }

ClientRecord::ClientStatus ClientRecord::get_status() const { return status_; }
void ClientRecord::set_status(ClientStatus status) { status_ = status; }

Task* ClientRecord::get_task() const { return ptask_; }
void ClientRecord::set_task(Task *ptask) { ptask_ = ptask; }

bool ClientRecord::is_idle() { return kIdle == status_; }
bool ClientRecord::is_in_computing() { return kInComputing == status_; }
bool ClientRecord::is_breakdown() { return kBreakdown == status_; }

void ClientRecord::set_heartbeat(int64_t heartbeat) { heartbeat_ = heartbeat; }
int64_t ClientRecord::get_heartbeat() const { return heartbeat_; }

bool ClientRecord::is_timeout()
{
	return (s_clock() - get_heartbeat() > MAX_HEARTBEAT_TIMEOUT); 
}

void ClientRecord::set_in_computing() { 
	set_status(kInComputing); 
	in_computing_num++;
}

void ClientRecord::set_breakdown() { 
	set_status(kBreakdown); 
	breakdown_num++;
	if (is_in_computing())
		in_computing_num--;
}

void ClientRecord::set_idle() { 
	set_status(kIdle); 
	in_computing_num--;
}

void ClientRecord::reset_idle() {
	set_status(kIdle);
	breakdown_num--;
}