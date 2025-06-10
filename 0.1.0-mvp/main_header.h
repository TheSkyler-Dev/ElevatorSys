
#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

#include <queue>
#include <string>
#include <future>
#include <mutex>

// Global variables declarations
extern std::queue<int> request_queue;
extern std::queue<int> floor_queue;
extern std::queue<int> call_origin;
extern std::mutex floor_mutex;
extern int current_floor;

// Color constants
extern const std::string FG_RESET;
extern const std::string FG_RED;
extern const std::string FG_YELLOW;
extern const std::string FG_GREEN;
extern const std::string FG_CYAN;
extern const std::string FG_BLACK;
extern const std::string BOLD;
extern const std::string BG_BLUE;
extern const std::string BG_WHITE;
extern const std::string BG_RESET;
extern const std::string RESET;

// Function declarations
void call_menu();
void floor_select();
void doors();
std::future<void> elevator_movement(int dest);
template<typename T>
std::string queue_to_string(std::queue<T> q);
void call_elev();
void select_floor();

#endif // MAIN_HEADER_H