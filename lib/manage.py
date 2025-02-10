import subprocess
from typing import List, NamedTuple

class TaskInfo(NamedTuple):
    total: int
    running: int
    sleeping: int
    stopped: int
    zombie: int

class CpuInfo(NamedTuple):
    name: str
    user_usage: float
    system_usage: float
    nice_usage: float
    idle: float
    IO_wait_usage: float
    hardware_interrupt_usage: float
    software_interrupt_usage: float
    stolen_usage: float

class MemInfo(NamedTuple):
    total: float
    free: float
    used: float
    buff_cache: float

class SwapInfo(NamedTuple):
    total: float
    free: float
    used: float
    avail_mem: float

class MachineInfo(NamedTuple):
    task: TaskInfo
    cpu: CpuInfo
    memory: MemInfo
    swap: SwapInfo
    processes_txt: List[str]

def cmd(command: str) -> str:
    result = ""
    try:
        result = subprocess.check_output(command, shell=True, text=True)
    except subprocess.CalledProcessError as e:
        print(f"Failed to execute command: {command}", file=sys.stderr)
    return result

def string_split(s: str, delimiter: str) -> List[str]:
    return s.split(delimiter)

def string_between(s: str, start: str, end: str) -> str:
    return s.split(start)[1].split(end)[0]

def info_txt() -> List[str]:
    return string_split(cmd("top -b -n 1"), '\n')

def get_task_data(s: str) -> TaskInfo:
    return TaskInfo(
        int(string_between(s, "Tasks: ", " total, ")),
        int(string_between(s, " total, ", " running, ")),
        int(string_between(s, " running, ", " sleeping, ")),
        int(string_between(s, " sleeping, ", " stopped, ")),
        int(string_between(s, " stopped, ", " zombie"))
    )

def get_cpu_data(s: str) -> CpuInfo:
    kws = ["%Cpu(s): ", " us, ", " sy, ", " ni, ", " id, ", " wa, ", " hi, ", " si, ", " st"]
    return CpuInfo(
        float(string_between(s, kws[0], kws[1])),
        float(string_between(s, kws[1], kws[2])),
        float(string_between(s, kws[2], kws[3])),
        float(string_between(s, kws[3], kws[4])),
        float(string_between(s, kws[4], kws[5])),
        float(string_between(s, kws[5], kws[6])),
        float(string_between(s, kws[6], kws[7])),
        float(string_between(s, kws[7], kws[8]))
    )

def get_mem_data(s: str) -> MemInfo:
    kws = ["Mem : ", " total, ", " free, ", " used, ", " buff/cache"]
    return MemInfo(
        float(string_between(s, kws[0], kws[1])),
        float(string_between(s, kws[1], kws[2])),
        float(string_between(s, kws[2], kws[3])),
        float(string_between(s, kws[3], kws[4]))
    )

def get_swap_data(s: str) -> SwapInfo:
    kws = ["Swap: ", " total, ", " free, ", " used. ", " avail Mem"]
    return SwapInfo(
        float(string_between(s, kws[0], kws[1])),
        float(string_between(s, kws[1], kws[2])),
        float(string_between(s, kws[2], kws[3])),
        float(string_between(s, kws[3], kws[4]))
    )

def info() -> MachineInfo:
    info_txt_data = info_txt()
    task_info_data = get_task_data(info_txt_data[1])
    cpu_info_data = get_cpu_data(info_txt_data[2])
    mem_info_data = get_mem_data(info_txt_data[3])
    swap_info_data = get_swap_data(info_txt_data[4])
    processes = info_txt_data[5:]
    return MachineInfo(task_info_data, cpu_info_data, mem_info_data, swap_info_data, processes)