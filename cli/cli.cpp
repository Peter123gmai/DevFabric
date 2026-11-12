#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <cstdlib>
#include <unordered_map>

#include "../libs/color.hpp"

namespace fs = std::filesystem;

//------------------------------------------------------------------------------
// Version
//------------------------------------------------------------------------------
namespace {
    constexpr char kAppName[] = "cli";
    constexpr char kVersion[] = "1.20";
}

//------------------------------------------------------------------------------
// Command interface
//------------------------------------------------------------------------------
// Every CLI command implements this interface. Adding a new command only
// requires a new subclass and registering it in CliApplication.
class Command {
public:
    virtual ~Command() = default;

    // Human-readable command name used for dispatch and help text.
    virtual std::string name() const = 0;

    // Executes the command with the provided arguments (excluding the
    // program name and the command token itself).
    // Returns 0 on success, non-zero on failure.
    virtual int execute(const std::vector<std::string>& args) = 0;
};

//------------------------------------------------------------------------------
// Help command
//------------------------------------------------------------------------------
class HelpCommand : public Command {
public:
    std::string name() const override { return "help"; }

    int execute(const std::vector<std::string>& /*args*/) override {
        std::cout << "Usage: " << kAppName << " <command> [options]\n"
                  << "\nCommands:\n"
                  << "  -v, --v, --version, -version   Show version information\n"
                  << "  -run <file> [file ...]         Run a code file (.cpp, .py, .rs, .sh)\n"
                  << "  -help, --help                  Show this help message\n"
                  << "\nExamples:\n"
                  << "  " << kAppName << " -v\n"
                  << "  " << kAppName << " -run main.py\n"
                  << "  " << kAppName << " -run app.cpp script.js\n"
                  << std::endl;
        return 0;
    }
};

//------------------------------------------------------------------------------
// Version command
//------------------------------------------------------------------------------
class VersionCommand : public Command {
public:
    std::string name() const override { return "version"; }

    int execute(const std::vector<std::string>& /*args*/) override {
        std::cout << kVersion << std::endl;
        return 0;
    }
};

//------------------------------------------------------------------------------
// File executor
//------------------------------------------------------------------------------
// Encapsulates the logic for detecting file types and executing them.
// Keeps execution details separate from command dispatching.
class FileExecutor {
public:
    // Returns the file extension including the dot (e.g. ".cpp").
    static std::string extension(const std::string& filepath) {
        return fs::path(filepath).extension().string();
    }

    // Executes a single file based on its extension.
    // Returns 0 on success, non-zero on failure.
    int run(const std::string& filepath) {
        if (!fs::exists(filepath)) {
            std::cerr << Color::Red << "Error: File not found: " << filepath
                      << Color::Reset << "\n";
            return 1;
        }

        const std::string ext = extension(filepath);
        const std::string output = fs::path(filepath).replace_extension("").string();
        std::string command;

        if (ext == ".cpp") {
            command = "g++ " + filepath + " -o " + output + " && ./" + output;
        } else if (ext == ".py") {
            command = "python " + filepath;
        } else if (ext == ".rs") {
            command = "rustc " + filepath + " -o " + output + " && ./" + output;
        } else if (ext == ".sh") {
            command = "bash " + filepath;
        } else {
            std::cerr << Color::Red << "Error: Unsupported file extension '"
                      << ext << "' for '" << filepath << "'" << Color::Reset
                      << "\n";
            return 1;
        }

        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << Color::Red << "Error: Failed to execute '"
                      << filepath << "'" << Color::Reset << "\n";
            return 1;
        }
        return 0;
    }
};

//------------------------------------------------------------------------------
// Run command
//------------------------------------------------------------------------------
// Executes one or more files using FileExecutor.
class RunCommand : public Command {
    FileExecutor executor_; // composition: RunCommand owns a FileExecutor

public:
    std::string name() const override { return "run"; }

    int execute(const std::vector<std::string>& args) override {
        // args[0] is the "-run" token itself; files start at index 1.
        if (args.size() < 2) {
            std::cerr << Color::Red << "Error: missing file argument after -run"
                      << Color::Reset << "\n";
            return 1;
        }

        for (size_t i = 1; i < args.size(); ++i) {
            if (int code = executor_.run(args[i]); code != 0) {
                return code; // stop on first failure
            }
        }
        return 0;
    }
};

//------------------------------------------------------------------------------
// CLI application
//------------------------------------------------------------------------------
// Owns all command instances (via unique_ptr) and dispatches the user's input
// to the matching command.
class CliApplication {
    std::unordered_map<std::string, std::unique_ptr<Command>> commands_;

    // Registers a command under all of its aliases.
    void register_command(std::unique_ptr<Command> cmd,
                          std::initializer_list<std::string> aliases) {
        for (const auto& alias : aliases) {
            commands_.emplace(alias, std::move(cmd));
        }
    }

public:
    CliApplication() {
        // Register built-in commands with their aliases.
        register_command(std::make_unique<HelpCommand>(),
                         {"-help", "--help"});
        register_command(std::make_unique<VersionCommand>(),
                         {"-v", "--v", "--version", "-version"});
        register_command(std::make_unique<RunCommand>(),
                         {"-run"});
    }

    // Parses argc/argv and dispatches to the correct command.
    // Returns the process exit code.
    int run(int argc, char* argv[]) {
        std::vector<std::string> args(argv, argv + argc);

        if (args.size() < 2) {
            std::cerr << Color::Red << "Error: Missing argument" << Color::Reset
                      << "\nUse -help for usage information\n";
            return 1;
        }

        const std::string& token = args[1];

        // Reject bare dash tokens.
        if (token == "-" || token == "--") {
            std::cerr << Color::Red << "Error: missing argument after `-` or `--`"
                      << Color::Reset << "\n"
                      << "Use -help for usage information\n";
            return 1;
        }

        // Look up the command in the registry.
        auto it = commands_.find(token);
        if (it == commands_.end()) {
            std::cerr << Color::Red << "Error: Unknown command '" << token << "'"
                      << Color::Reset << "\n"
                      << "Use -help for usage information\n";
            return 1;
        }

        // Pass the remaining arguments (everything after the command token)
        // to the command's execute method.
        std::vector<std::string> command_args(args.begin() + 1, args.end());
        return it->second->execute(command_args);
    }
};

//------------------------------------------------------------------------------
// Entry point
//------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    CliApplication app;
    return app.run(argc, argv);
}
