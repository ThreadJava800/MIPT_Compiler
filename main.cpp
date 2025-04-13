#include <boost/program_options.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

#include "driver.hpp"
#include "log.hpp"

namespace arg_parser = boost::program_options;

struct ProgramSettings_t
{
    bool interpret_mode;
    const std::string input_file_name;
    const std::string graph_dump_file_name;
};

static arg_parser::options_description createParser()
{
    arg_parser::options_description desc("Allowed options:");
    desc.add_options()
        ("help", "print help message")
        ("input", arg_parser::value<std::string>()->required(), "path to source file")
        ("interpret", "interpret given program after parsing")
        ("graph-dump", arg_parser::value<std::string>(), "create AST dump to the provided .png file");

    return desc;
}

static ProgramSettings_t parseCmd(const int argc, const char **argv, const arg_parser::options_description desc)
{
    arg_parser::variables_map var_map;
    arg_parser::store(arg_parser::parse_command_line(argc, argv, desc), var_map);

    if (var_map.count("help"))
    {
        std::cout << desc << '\n';
        exit(1);
    }

    arg_parser::notify(var_map);

    if (var_map.count("graph-dump") > 0)
    {
        return ProgramSettings_t{
            var_map.count("interpret") > 0,
            var_map["input"].as<std::string>(),
            var_map["graph-dump"].as<std::string>()
        };
    }
    return ProgramSettings_t{
        var_map.count("interpret") > 0,
        var_map["input"].as<std::string>().c_str(),
        ""
    };
}

int main(int argc, const char **argv)
{
    const arg_parser::options_description desc = createParser();
    const ProgramSettings_t settings = parseCmd(argc, argv, desc);

    Driver_t driver;

    if (!initLogging("compiler_log.txt"))
    {
        fprintf(stderr, "Failed to init log library!\n");
    }

    std::ifstream user_input(settings.input_file_name);
    if(!user_input) {
        USER_ERR("Cannot open file: %s\n", settings.input_file_name.c_str());
        return -1;
    }
    driver.proceedFrontEnd(user_input);

    if (settings.graph_dump_file_name != "")
    {
        driver.graphDump(settings.graph_dump_file_name.c_str());
    }
    if (settings.interpret_mode)
    {
        driver.interpret();
    }

    if (!deinitLogging())
    {
        fprintf(stderr, "Failed to deinit log library!\n");
    }
}