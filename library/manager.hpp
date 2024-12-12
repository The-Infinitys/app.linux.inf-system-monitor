#pragma once
#include <list>
#include <string>
#include <vector>

#include "terminal.hpp"
#include "utils.hpp"

namespace manager {
struct task_info {
    int total;
    int running;
    int sleeping;
    int stopped;
    int zombie;
};

struct cpu_info {
    std::string name;
    double user_usage;
    double system_usage;
    double nice_usage;
    double idle;
    double IO_wait_usage;
    double hardware_interrupt_usage;
    double software_interrupt_usage;
    double stolen_usage;
};
struct mem_info {
    double total;
    double free;
    double used;
    double buff_cache;
};
struct swap_info {
    double total;
    double free;
    double used;
    double avail_mem;
};
struct machine_info {
    task_info task;
    cpu_info cpu;
    mem_info memory;
    swap_info swap;
    std::list<std::string> processes_txt;
};
std::vector<std::string> info_txt() {
    return string_split(terminal::cmd("top -b -n 1"), '\n');
    ;
}
task_info get_task_data(std::string str) {
    return {
        stoi(string_between(str, "Tasks: ", " total, ")),
        stoi(string_between(str, " total, ", " running, ")),
        stoi(string_between(str, " running, ", " sleeping, ")),
        stoi(string_between(str, " sleeping, ", " stopped, ")),
        stoi(string_between(str, " stopped, ", " zombie")),
    };
}

cpu_info get_cpu_data(std::string str) {
    std::vector<std::string> kws = {"%Cpu(s): ", " us, ", " sy, ",
                                    " ni, ",     " id, ", " wa, ",
                                    " hi, ",     " si, ", " st"};
    return {
        string_split(terminal::cmd("mpstat -u -P ALL"), '\n')[0],
        std::stod(string_between(str, kws[0], kws[1])),
        std::stod(string_between(str, kws[1], kws[2])),
        std::stod(string_between(str, kws[2], kws[3])),
        std::stod(string_between(str, kws[3], kws[4])),
        std::stod(string_between(str, kws[4], kws[5])),
        std::stod(string_between(str, kws[5], kws[6])),
        std::stod(string_between(str, kws[6], kws[7])),
        std::stod(string_between(str, kws[7], kws[8])),
    };
}
mem_info get_mem_data(std::string str) {
    std::vector<std::string> kws = {"Mem : ", " total, ", " free, ", " used, ",
                                    " buff/cache"};
    return {
        std::stod(string_between(str, kws[0], kws[1])),
        std::stod(string_between(str, kws[1], kws[2])),
        std::stod(string_between(str, kws[2], kws[3])),
        std::stod(string_between(str, kws[3], kws[4])),
    };
}
swap_info get_swap_data(std::string str) {
    std::vector<std::string> kws = {"Swap: ", " total, ", " free, ", " used. ",
                                    " avail Mem"};
    return {
        std::stod(string_between(str, kws[0], kws[1])),
        std::stod(string_between(str, kws[1], kws[2])),
        std::stod(string_between(str, kws[2], kws[3])),
        std::stod(string_between(str, kws[3], kws[4])),
    };
}

machine_info info() {
    std::vector<std::string> info_txt_data = info_txt();
    task_info task_info_data = get_task_data(info_txt_data[1]);
    cpu_info cpu_info_data = get_cpu_data(info_txt_data[2]);
    mem_info mem_info_data = get_mem_data(info_txt_data[3]);
    swap_info swap_info_data = get_swap_data(info_txt_data[4]);
    std::list<std::string> processes;
    for (int i = 5; i < info_txt_data.size(); i++) {
        processes.insert(processes.end(), info_txt_data[i]);
    }
    return {task_info_data, cpu_info_data, mem_info_data, swap_info_data,
            processes};
}
}  // namespace manager