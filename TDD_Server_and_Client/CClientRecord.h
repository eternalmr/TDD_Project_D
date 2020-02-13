#pragma once
#pragma warning(disable:4996)

//#include "pch.h"
#include "CTask.h"


class ClientRecord
{
public:
	enum ClientStatus { kFree = 0, kInComputing, kBreakdown };

public:
	ClientRecord();
	ClientRecord(uint id);

	uint get_client_id() const;
	ClientStatus get_client_status() const;
	Task* get_task() const;
	int64_t get_heartbeat() const;

	void set_client_id(uint id);
	void set_task(Task *ptask);
	void set_heartbeat(int64_t heartbeat);

	void set_breakdown();
	void set_status_free();
	void set_in_computing();

	bool is_free();
	bool is_in_computing();
	bool is_timeout();
	bool is_breakdown();

private:
	void set_client_status(ClientStatus status);

private:
	uint32_t client_id_;
	ClientStatus client_status_;
	Task *ptask_;
	int64_t heartbeat_;
	static const int MAX_HEARTBEAT_TIMEOUT; //millisecond
};

