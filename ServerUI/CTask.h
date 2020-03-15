#pragma once
//#pragma warning(disable:4996)

class Task
{
private:
	enum ComputeStatus { kNotStart = 0, kInComputing, kFinished };
	enum   StoreStatus { kNotSave = 0, kSaved };

public:
	explicit Task(uint id = 1);
	int get_id() const;

	void set_not_start();
	void set_in_computing();
	void set_finished();
	void set_result_saved();
	void set_result_not_saved();

	bool is_not_start();
	bool is_in_computing();
	bool is_finished();
	bool is_not_save();
	bool is_saved();

	uint get_simulation_progress();
	void set_simulation_progress(uint progress);

private:
	void set_id(uint id);

private:
	uint progress_;
	unsigned int id_;
	ComputeStatus compute_status_;
	StoreStatus store_status_;
};

