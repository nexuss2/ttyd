#include <stdio.h>

int msgLoadPC(const char* filename);
void msgDumpPC(unsigned int max_entries);

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: %s msg_file_without_txt\n", argv[0]);
        printf("example: %s hei_00\n", argv[0]);
        return 1;
    }

    if (!msgLoadPC(argv[1])) {
        printf("failed to load message file %s\n", argv[1]);
        return 1;
    }

    msgDumpPC(9999);
    return 0;
}
