#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "main_header.h"
#include <sstream>

class ElevatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset queues and floor
        while (!request_queue.empty()) request_queue.pop();
        while (!floor_queue.empty()) floor_queue.pop();
        while (!call_origin.empty()) call_origin.pop();
        current_floor = 1;
    }
};

TEST_F(ElevatorTest, BasicMovement) {
    current_floor = 1;
    auto future = elevator_movement(3);
    future.wait();
    EXPECT_EQ(current_floor, 3);
}

// Add more test cases as needed...

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}