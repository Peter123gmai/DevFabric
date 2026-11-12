# AGENTS.md — DevFabric

Project overview, coding conventions, and how to work with this repository.

## Project Summary

DevFabric is a small open-source CLI tool suite for developers, written in modern C++. The main entry point is `cli/cli.cpp`, compiled into `bin/cli` and exposed on `PATH` via `script/install.sh`.

**Supported OS:** Linux Fedora 45 Workstation (official)

## Repository Structure

```
DevFabric/
├── cli/cli.cpp          # Main CLI application (OOP, Command pattern)
├── libs/color.hpp       # ANSI color/style constants (inline constexpr)
├── config/cli.cfg       # Debug logging configuration (ini format)
├── debug/dbg-tools.cpp  # Debug tools placeholder (empty)
├── bin/cli              # Compiled binary (gitignored)
├── script/install.sh    # Compiles cli.cpp and adds bin/ to ~/.bashrc PATH
├── script/commit.sh     # Git add/commit/push helper with logging
├── README.md            # User-facing documentation
├── .gitignore           # Ignores bin/ and debug/
└── AGENTS.md            # This file
```

## How to Build

```bash
# From project root
g++ cli/cli.cpp -o bin/cli
# or use the install script (also adds bin/ to PATH in ~/.bashrc)
./script/install.sh
source ~/.bashrc
```

`bin/` and `debug/` are gitignored.

## How to Run

```bash
cli -v                      # Show version (1.20)
cli -help                   # Show help message
cli -run main.py            # Run a single file
cli -run app.cpp script.js  # Run multiple files in order
```

Supported `-run` extensions: `.cpp` (g++), `.py` (python), `.rs` (rustc), `.sh` (bash).

## Code Conventions

### cli/cli.cpp — Modern C++ OOP Design

The CLI uses a **Command pattern** for easy scaling:

- **`Command`** (abstract base class, `cli/cli.cpp:16`) — interface with `name()` and `execute(args)`. Add new commands by subclassing this.
- **`HelpCommand`** — prints usage text.
- **`VersionCommand`** — prints version string.
- **`RunCommand`** — executes files via `FileExecutor`.
- **`FileExecutor`** — detects extension via `std::filesystem::path::extension()` and runs with `std::system`.
- **`CliApplication`** — owns all commands via `std::unique_ptr<Command>`, stores them in `std::unordered_map<std::string, std::unique_ptr<Command>>` keyed by alias, dispatches in `run()`.
- **`main`** — constructs `CliApplication` and calls `app.run(argc, argv)`.

Key conventions:
- Anonymous namespace for file-local constants (`kAppName`, `kVersion`).
- `fs = std::filesystem` alias at top.
- `std::unique_ptr` for ownership; no raw owning pointers.
- `return` exit codes from `main` instead of `exit()`.
- `std::cerr` for errors, `std::cout` for normal output.
- Exit code `0` = success, `1` = failure.
- Comments explain non-obvious logic; every class/function has a note.

### libs/color.hpp

- `#pragma once`
- All constants are `inline constexpr const char*` inside `namespace Color` to avoid `misc-definitions-in-headers` warnings.
- Contains reset, style (bold, dim, underline, blink, reverse), foreground, bright foreground, and background ANSI codes.

### config/cli.cfg

INI format:
```ini
[Debug]
show_log = true
show_warn = true
show_error = true
```

## Dependencies

Required on the system:
- `gcc`, `g++` (for compiling the CLI and `.cpp` files)
- `python` (for `.py` execution)
- `rustc` (for `.rs` execution)
- `bash` (for `.sh` execution)
- `dotnet`, `npm` (documented in README for future language support)

## Adding a New Command

1. Create a class inheriting from `Command` (`cli/cli.cpp:16`).
2. Implement `name()` and `execute(const std::vector<std::string>&)`.
3. In `CliApplication::CliApplication()` (`cli/cli.cpp:139`), register it:
   ```cpp
   register_command(std::make_unique<YourCommand>(), {"-alias1", "--alias2"});
   ```

## Scripts

- `script/install.sh` — recompiles `cli/cli.cpp` to `bin/cli`, creates `debug/`, chmods binaries, appends `bin/` to `PATH` in `~/.bashrc`.
- `script/commit.sh` — interactive git add/commit/push with `log_commit.txt` logging. Run with caution; it prompts for commit message.

## Testing

No formal test suite exists. Verify manually:
```bash
g++ cli/cli.cpp -o bin/cli
./bin/cli -v
./bin/cli -help
./bin/cli -run <some-file>
```

## Lint / Typecheck

No automated lint or typecheck tooling is configured. The code compiles with `g++` using default C++17 (or newer) support. Ensure `<filesystem>`, `<memory>`, `<unordered_map>` are available (C++17).
