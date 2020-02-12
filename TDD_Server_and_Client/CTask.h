#pragma once
#pragma warning(disable:4996)



//typedef unsigned int uint;

class Task
{
private:
	enum ComputeStatus { kNotStart = 0, kInComputing, kFinished };
	enum   StoreStatus { kNotSave = 0, kSaved };

public:
	Task();
	Task(uint id);
	int get_id() const;

	void set_task_not_start();
	void set_task_in_computing();
	void set_task_finished();
	void set_task_result_saved();
	void set_task_result_not_saved();

	bool is_not_start();
	bool is_in_computing();
	bool is_finished();
	bool is_not_save();
	bool is_saved();

private:
	void set_id(uint id);

private:
	unsigned int id_;
	ComputeStatus compute_status_;
	StoreStatus store_status_;
};

