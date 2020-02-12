#include "pch.h"
#include "CTask.h"


class TaskTest : public testing::Test
{
public:
	TaskTest() : task_with_id(1) { }

public:
	Task task_with_no_parameter;
	Task task_with_id;
};


TEST_F(TaskTest, TestStoreStatusAfterSet) {
	task_with_id.set_task_result_saved();
	EXPECT_TRUE(task_with_id.is_saved());
}

TEST_F(TaskTest, TestComputingStatusAfterSet) {
	task_with_id.set_task_in_computing();
	EXPECT_TRUE(task_with_id.is_in_computing());
	task_with_id.set_task_finished();
	EXPECT_TRUE(task_with_id.is_finished());
}

TEST_F(TaskTest, TestInitialStoreStatusIsNotSave) {
	EXPECT_TRUE(task_with_id.is_not_save());
}

TEST_F(TaskTest, TestInitialComputingStatusIsNotStart) {
	EXPECT_TRUE(task_with_id.is_not_start());
}

TEST_F(TaskTest, TestIdOfTasksInTestSuite) {
	EXPECT_EQ(task_with_no_parameter.get_id(), 0);
	EXPECT_EQ(task_with_id.get_id(), 1);
}