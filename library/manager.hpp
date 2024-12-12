#pragma once
#include <string>
#include <vector>

#include "utils.hpp"

namespace manager {
struct task_info {
    unsigned int total;
    unsigned int running;
    unsigned int sleeping;
    unsigned int stopped;
    unsigned int zombie;
};

struct cpu_info {
    std::string name;
    float user_usage;
    float system_usage;
    float nice_usage;
    float idle_usage;
    float IO_wait_usage;
    float hardware_interrupt_usage;
    float software_interrupt_usage;
};
struct mem_info {
    float total;
    float free;
    float used;
    float buff_cache;
};
struct swap_info {
    float total;
    float free;
    float used;
    float avail_mem;
};
struct process_info {
    unsigned int process_id;
    std::string user;
    int priority;
    std::string virt;
    std::string res;
    std::string shr;
    std::string status;
    float cpu_usage;
    float memory_usage;
    std::string time;
    std::string command_name;
};
struct machine_info {
    task_info task;
    cpu_info cpu;
    mem_info memory;
    swap_info swap;
};
std::vector<std::string> info_txt() {
    return string_split(terminal::cmd("top -b -n 1"), '\n');
    ;
}
}  // namespace manager