#include <cstdio>
#include <fstream>

#include "log.hpp"
#include "frontend.hpp"

int main(int argc, char **argv)
{
    if (!initLogging("compiler_log.txt"))
    {
        fprintf(stderr, "Failed to init log library!\n");
    }

    DBG_ERR("GO FUCK YOURSELF");

    if (argc != 2)
    {
        return -1;
    }
    std::ifstream user_input(argv[1]);
    if(!user_input) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return -1;
    }
    proceedFrontEnd(user_input);

    if (!deinitLogging())
    {
        fprintf(stderr, "Failed to deinit log library!\n");
    }
}