#pragma once
#pragma warning(disable:4996)

//#include "pch.h"
#include "CTask.h"


class ClientRecord
{
private:
	enum ClientStatus { kFree = 0, kInComputing, kBreakdown };

public:
	ClientRecord();
	ClientRecord(uint id);

	uint get_id() const;

	Task* get_task() const;
	int64_t get_heartbeat() const;
	void set_id(uint id);
	void set_task(Task *ptask);
	void set_heartbeat(int64_t heartbeat);

	void set_breakdown();
	void set_free();
	void set_in_computing();

	bool is_free();
	bool is_in_computing();
	bool is_timeout();
	bool is_breakdown();

private:
	void set_status(ClientStatus status);
	ClientStatus get_status() const;

private:
	uint32_t id_;
	ClientStatus status_;
	Task *ptask_;
	int64_t heartbeat_;
	static const int MAX_HEARTBEAT_TIMEOUT; //millisecond
};

