//#pragma warning(disable:4996)
#include "pch.h"
#include "CTask.h"

// constructors
Task::Task(uint id):id_(id)
{
	set_not_start();
	set_result_not_saved();
}

// get functions
int Task::get_id() const { return id_; }

// set functions
void Task::set_id(uint id) { id_ = id; }


void Task::set_in_computing() { compute_status_ = kInComputing; }
void Task::set_result_saved() { store_status_ = kSaved; }
void Task::set_result_not_saved() { store_status_ = kNotSave; }

bool Task::is_not_start() { return kNotStart == compute_status_; }
bool Task::is_in_computing() { return kInComputing == compute_status_; }
bool Task::is_finished() { return kFinished == compute_status_; }
bool Task::is_not_save() { return kNotSave == store_status_; }
bool Task::is_saved() { return kSaved == store_status_; }

uint Task::get_simulation_progress() { return progress_; }
void Task::set_simulation_progress(uint progress) { progress_ = progress; }

void Task::set_not_start() {
	compute_status_ = kNotStart; 
	set_simulation_progress(0);
}

void Task::set_finished() { 
	compute_status_ = kFinished; 
	set_simulation_progress(100);
}