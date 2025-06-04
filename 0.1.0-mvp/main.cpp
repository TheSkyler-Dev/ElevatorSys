#include <iostream>
#include <queue>
#include <string>
#include <format>
#include <sstream>

//GLOBAL: ANSI escape codes for text formatting
const std::string FG_RESET  = "\033[39m";
const std::string FG_RED    = "\033[31m"; // ERROR
const std::string FG_YELLOW = "\033[38;5;220m"; // WARN/STATUS
const std::string FG_GREEN  = "\033[32m"; // OK
const std::string FG_CYAN   = "\033[36m"; // INFO
const std::string FG_BLACK  = "\033[30m"; // CONTRAST
const std::string BOLD      = "\033[1m";

//GLOBAL: ANSI escape codes for Backgrounds
const std::string BG_BLUE   = "\033[48;5;75m";
const std::string BG_WHITE  = "\033[48;5;255m"; // OUTLINE
const std::string BG_RESET  = "\033[49m";

//UNIVERSAL RESET
const std::string RESET = "\033[0m";

//GLOBAL: queues
std::queue<int> request_queue;
std::queue<int> floor_queue;

//External elevator call menu
void call_menu(){
    std::cout << std::format("{}{}{}=========| Call Elevator |========={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}|{}           [1] - ^ UP ^          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}           [2] - v DOWN v        {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}==================================={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
};

//floor selection menu
void floor_select(){
    std::cout << std::format("{}{}{}=========| Select Floor |========={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}|{}        [1] - 1st Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [2] - 2nd Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [3] - 3rd Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [4] - 4th Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}        [5] - 5th Floor          {}{}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, FG_BLACK, RESET);
    std::cout << std::format("{}{}{}|{}                                 {}|{}\n", BG_WHITE, FG_BLACK, BOLD, BG_BLUE, BG_WHITE, RESET);
    std::cout << std::format("{}{}{}==================================={}\n", BG_WHITE, FG_BLACK, BOLD, RESET);
};

//Elevator movement
/*int elevator_movement(int dest){}*/ //TO BE IMPLEMENTED SOON

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

//MAIN
int main() {
    try {
        while (true) {
            int mode = 0;
            std::cout << std::format("{}Enter mode (1: Call Elevator, 2: Select Floor, 0: Exit):{} ", FG_CYAN, RESET);
            std::cin >> mode;

            if (mode == 0) break;

            switch (mode) {
                case 1:
                    call_menu();
                    int direction;
                    std::cin >> direction;
                    request_queue.push(direction);
                    std::cout << std::format("{}DEBUG: Request pushed to queue: {}{}{}{}\n", FG_YELLOW, BG_WHITE, FG_BLACK, queue_to_string(request_queue), RESET);
                    break;

                case 2:
                    floor_select();
                    int floor;
                    std::cin >> floor;
                    floor_queue.push(floor);
                    std::cout << std::format("{}DEBUG: Floor pushed to queue: {}{}{}{}\n", FG_YELLOW, BG_WHITE, FG_BLACK, queue_to_string(floor_queue), RESET);
                    break;

                default:
                    std::cout << std::format("{}{}{}Invalid mode{}\n", BG_WHITE, FG_RED, BOLD, RESET);
                    break;
            }
        }
    } catch (const std::exception& e ) {
        std::cout << e.what() << '\n';
        throw std::runtime_error("Invalid mode");
    }
    return 0;
}