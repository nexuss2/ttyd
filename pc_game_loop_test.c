int PCGameLoopRunWithContent(const char* map, const char* msg);

int main(int argc, char** argv) {
    const char* map = "aaa_00";
    const char* msg = "hei_00";

    if (argc > 1) {
        map = argv[1];
    }

    if (argc > 2) {
        msg = argv[2];
    }

    return PCGameLoopRunWithContent(map, msg);
}
