#include <stdio.h>

int msgLoadPC(const char* filename);
const char* msgSearchPC(const char* tag);
void msgDumpPC(unsigned int max_entries);

int main(void) {
    const char* msg;

    if (!msgLoadPC("hei_00")) {
        return 1;
    }

    msgDumpPC(10);

    msg = msgSearchPC("stg0_00_00");
    if (msg) {
        printf("search stg0_00_00: %s\n", msg);
    } else {
        printf("stg0_00_00 not found\n");
    }

    return 0;
}
