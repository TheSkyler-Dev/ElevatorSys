#include <iostream>
#include <queue>
#include <string>
#include <format>
#include <sstream>
#include <list>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>
#include <vector>

//GLOBAL: Constant ANSI escape codes for text formatting
const std::string FG_RESET  = "\033[39m";
const std::string FG_RED    = "\033[31m"; // ERROR
const std::string FG_YELLOW = "\033[38;5;220m"; // WARN/STATUS
const std::string FG_GREEN  = "\033[32m"; // OK
const std::string FG_CYAN   = "\033[36m"; // INFO
const std::string FG_BLACK  = "\033[30m"; // CONTRAST
const std::string BOLD      = "\033[1m";

//GLOBAL: Constant ANSI escape codes for Backgrounds
const std::string BG_BLUE   = "\033[48;5;75m";
const std::string BG_WHITE  = "\033[48;5;255m"; // OUTLINE
const std::string BG_RESET  = "\033[49m";

//UNIVERSAL RESET CONST
const std::string RESET = "\033[0m";

//GLOBAL: queues
std::queue<int> request_queue;
std::queue<int> floor_queue;
std::queue<int> call_origin;

//External elevator call menu prefab
void call_menu(){
    std::cout << std::format("{}{}{}=========| Call Elevator |========={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}|{}           [1] - ^ UP ^          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}           [2] - v DOWN v        {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}==================================={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
};

//floor selection menu prefab
void floor_select(){
    std::cout << std::format("{}{}{}=========| Select Floor |=========={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}|{}        [1] - 1st Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [2] - 2nd Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [3] - 3rd Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [4] - 4th Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [5] - 5th Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}==================================={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
};

std::mutex floor_mutex;
//GLOBAL: Current Floor
int current_floor = 1; //first floor

//Door control function
void doors() {
    bool should_open = false;

    // Check floor_queue
    if (!floor_queue.empty() && current_floor == floor_queue.front()) {
        should_open = true;
        floor_queue.pop();
    }

    // Check call_origin
    if (!call_origin.empty() && current_floor == call_origin.front()) {
        should_open = true;
        call_origin.pop();

        // Also pop the corresponding request_queue entry if it exists
        if (!request_queue.empty()) {
            request_queue.pop();
        }
    }

    // Open doors if needed
    if (should_open) {
        std::cout << std::format("{}Door open{}\n", FG_GREEN, RESET);
    }
}

//Function: Define Elevator movement flow
std::future<void> elevator_movement(int dest) {
    return std::async(std::launch::async, [dest]() {
        std::list<int> floors = {1, 2, 3, 4, 5};

        if (dest >= 1 && dest <= floors.size()) {
            std::cout << std::format("{}{}Moving elevator to floor {}. Please wait...{}\n", 
                                   FG_YELLOW, BOLD, dest, RESET);
            while (current_floor != dest) {
                if (dest > current_floor) {
                    std::cout << std::format("{}{}/\\ - Floor {}{}\n", 
                                           FG_GREEN, BOLD, current_floor, RESET);
                    current_floor++;
                } else if (dest < current_floor) {
                    std::cout << std::format("{}{}\\/ - Floor {}{}\n", 
                                           FG_GREEN, BOLD, current_floor, RESET);
                    current_floor--;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            }
            doors();
        } else {
            std::cout << std::format("{}{}Error: {} is out of bounds.{}\n", 
                                   BG_WHITE, FG_RED, dest, RESET);
        }
    });
}

// Helper function to convert queue contents to string. DEBUG ONLY
template<typename T>
std::string queue_to_string(std::queue<T> q) {
    std::stringstream ss;
    ss << "[";
    while (!q.empty()) {
        ss << q.front();
        q.pop();
        if (!q.empty()) ss << ", ";
    }
    ss << "]";
    return ss.str();
}

void call_elev() {
    call_menu();
    int direction;
    int my_floor;
    std::cin >> direction;
    std::cout << std::format("{}{}Please enter your current floor: {}", FG_CYAN, BG_WHITE, RESET);
    std::cin >> my_floor;
    request_queue.push(direction);
    call_origin.push(my_floor);
    std::cout << std::format("{}DEBUG: Request pushed to queue: {}{}{}{}\n", FG_YELLOW, BG_WHITE, FG_BLACK, queue_to_string(request_queue), RESET);
    auto future = elevator_movement(my_floor);
}

void select_floor() {
    floor_select();
    int floor;
    std::cout << std::format("{}Hint: You are on floor {}.{}\n", FG_YELLOW, current_floor, RESET);
    std::cin >> floor;
    floor_queue.push(floor);
    std::cout << std::format("{}DEBUG: Floor pushed to queue: {}{}{}{}\n", FG_YELLOW, BG_WHITE, FG_BLACK, queue_to_string(floor_queue), RESET);
    auto future = elevator_movement(floor);
}

//MAIN
int main() {
    try {
        int request_count = 0;
        std::vector<std::future<void>> pending_movements;

        // Continue until at least 2 requests have been processed
        while (request_count < 2) {
            int mode = 0;
            std::cout << std::format("{}Enter mode (1: Call Elevator, 2: Select Floor, 0: Exit):{} ", 
                                   FG_CYAN, RESET);
            std::cin >> mode;

            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    call_elev();
                    request_count++;
                    break;
                }
                case 2: {
                    select_floor();
                    request_count++;
                    break;
                }
                default:
                    std::cout << std::format("{}{}{}Invalid mode{}\n", 
                                           BG_WHITE, FG_RED, BOLD, RESET);
                    break;
            }

            // Optional: Print how many more requests are needed
            if (request_count < 2) {
                std::cout << std::format("{}Please make {} more request(s){}\n", 
                                       FG_CYAN, 2 - request_count, RESET);
            }
        }

        // Wait for any pending elevator movements to complete before exiting
        for (auto& future : pending_movements) {
            if (future.valid()) {
                future.wait();
            }
        }

    } catch (const std::exception& e) {
        std::cout << e.what() << '\n';
        throw std::runtime_error("Invalid mode");
    }
    return 0;
}