#include <boost/program_options.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "driver.hpp"
#include "log.hpp"

namespace arg_parser = boost::program_options;

struct ProgramSettings_t
{
    bool interpret_mode;
    std::string input_file_name;
    std::optional<std::string> graph_dump_file_name;
    std::optional<std::string> output_file_name;
};

static arg_parser::options_description createParser()
{
    arg_parser::options_description desc("Allowed options:");
    desc.add_options()
        ("help", "print help message")
        ("input", arg_parser::value<std::string>()->required(), "path to source file")
        ("interpret", "interpret given program after parsing")
        ("graph-dump", arg_parser::value<std::string>(), "create AST dump to the provided .png file")
        ("output", arg_parser::value<std::string>(), "path to .ll output file");

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

    ProgramSettings_t program_settings;
    program_settings.interpret_mode = var_map.count("interpret") > 0;
    program_settings.input_file_name = std::move(var_map["input"].as<std::string>());
    program_settings.graph_dump_file_name = std::nullopt;
    program_settings.output_file_name = std::nullopt;

    if (var_map.count("graph-dump") > 0)
    {
        program_settings.graph_dump_file_name = std::move(var_map["graph-dump"].as<std::string>());
    }

    if (var_map.count("output") > 0)
    {
        program_settings.output_file_name = std::move(var_map["output"].as<std::string>());
    }
    return program_settings;
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
    const bool isSuccess = driver.proceedFrontEnd(user_input);
    if (!isSuccess) {
        return -1;
    }

    if (settings.graph_dump_file_name.has_value())
    {
        driver.graphDump(settings.graph_dump_file_name.value().c_str());
    }
    if (settings.interpret_mode)
    {
        driver.interpret();
    }
    if (settings.output_file_name.has_value())
    {
        driver.generateLLVMIR(settings.output_file_name.value().c_str());
    }

    if (!deinitLogging())
    {
        fprintf(stderr, "Failed to deinit log library!\n");
    }
}