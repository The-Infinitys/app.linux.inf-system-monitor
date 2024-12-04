#ifndef MEMORY
#define MEMORY

typedef struct {
    unsigned long long total_memory; // 総メモリ（バイト）
    unsigned long long free_memory;  // 空きメモリ（バイト）
} MemoryInfo;

// メモリ情報を取得する関数
int get_memory_info(MemoryInfo* mem_info);

// メモリ情報をMBとGB単位で表示する関数(いるかわからんかったけど一応)
void print_memory_info(MemoryInfo* mem_info);

#endif