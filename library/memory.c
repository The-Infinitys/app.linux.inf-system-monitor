#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/sysinfo.h>
#endif

typedef struct {
    unsigned long long total_memory; //byteのメモリ全体
    unsigned long long free_memory; //byteの空きメモリ
} MemoryInfo;

int get_memory_info(MemoryInfo* mem_info) {
    if (mem_info == NULL) {
        return -1;
    }

#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    if (GlobalMemoryStatusEx(&statex)) {
        mem_info->total_memory = statex.ullTotalPhys;
        mem_info->free_memory = statex.ullAvailPhys;
        return 0;
    } else {
        return -1;
    }
#else
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        mem_info->total_memory = info.totalram * info.mem_unit;
        mem_info->free_memory = info.freeram * info.mem_unit;
        return 0;
    } else {
        return -1;
    }
#endif
}

void print_memory_info(MemoryInfo* mem_info) {
    if (mem_info == NULL) {
        fprintf(stderr, "Invalid memory info.\n");
        return;
    }
        //byte -> MB, GBしてるはず。あってるよね？
    double total_mb = mem_info->total_memory / (1024.0 * 1024.0);
    double total_gb = mem_info->total_memory / (1024.0 * 1024.0 * 1024.0);
    double free_mb = mem_info->free_memory / (1024.0 * 1024.0);
    double free_gb = mem_info->free_memory / (1024.0 * 1024.0 * 1024.0);

    printf("Total Memory: %.2f MB (%.2f GB)\n", total_mb, total_gb);
    printf("Free Memory:  %.2f MB (%.2f GB)\n", free_mb, free_gb);
}

int main() {
    MemoryInfo mem_info;
    if (get_memory_info(&mem_info) == 0) {
        print_memory_info(&mem_info);
    } else {
        printf("なんらかのエラーにより、メモリ容量を取得できません\n");
    }
    return 0;
}

/*
*　説明
*　 ↓メモリ情報を取得する関数
*        int get_memory_info(MemoryInfo* mem_info);
*
*   ↓メモリ情報をMBとGB単位で表示する関数(いるかわからんかったけど一応)
*        void print_memory_info(MemoryInfo* mem_info);
*/