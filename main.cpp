#include <stdio.h>

#include <iostream>

#define MAXLINE 1024

void print(std::string str) {
    std::cout << str << std::endl;
    return;
}

std::string exec_command(std::string cmd) {
    FILE* fp;
    char buf[1024];
    std::string result = "";
    // コマンドを指定してパイプをオープン
    const char* cmd_char = cmd.c_str();
    if ((fp = popen(cmd_char, "r")) == NULL) {
        fprintf(stderr, "Failed to open pipe: argv[1]=%s", cmd_char);
        return "";
    }

    // パイプからのコマンド実行結果を受け取り表示
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        result += buf;
    }
    pclose(fp);
    return result;
}

int main(int, char**) {
    print("Hello, from Infinity-task-manager!\n");
    print(exec_command("ls"));
    return 0;
}
