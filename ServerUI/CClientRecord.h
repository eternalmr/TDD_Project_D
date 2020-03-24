#pragma once

#include "CTask.h"
#include "../project_paramters.h"

class ClientRecord
{
private:
	enum ClientStatus { kIdle, kInComputing, kBreakdown };

public:
	ClientRecord();
	explicit ClientRecord(uint id);

	uint get_id() const;

	Task* get_task() const;
	int64_t get_heartbeat() const;
	void set_id(uint id);
	void set_task(Task *ptask);
	void set_heartbeat(int64_t heartbeat);

	void set_breakdown();
	void set_idle();
	void reset_idle();
	void set_in_computing();

	bool is_idle();
	bool is_in_computing();
	bool is_breakdown();
	bool is_timeout();

private:
	void set_status(ClientStatus status);
	ClientStatus get_status() const;

public:
	static uint total_num;
	static uint in_computing_num;
	static uint breakdown_num;

private:
	uint id_;
	ClientStatus status_;
	Task *ptask_;
	int64_t heartbeat_;
};

