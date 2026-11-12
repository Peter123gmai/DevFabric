# DevFabric

Open-source CLI tools for developers.

## Installation

```bash
git clone https://github.com/<your-username>/DevFabric.git
cd DevFabric
./script/install.sh
source ~/.bashrc
```

This compiles `cli/cli.cpp` into `bin/cli` and adds `bin/` to your `PATH`.

## Usage

### Check version

```bash
cli -v
```

Aliases: `--v`, `--version`, `-version`

### Run a code file

```bash
cli -run <file> [file ...]
```

Supported languages:

| Language | Requirements |
|----------|--------------|
| dotnet | dotnet SDK |
| nodejs | npm |
| python | python |
| c/c++ | gcc, gcc-g++ |

## Debug

Debug options are configured in `config/cli.cfg`:

```ini
[Debug]
show_log = true
show_warn = true
show_error = true
```

## Dependencies

- dotnet
- npm (nodejs)
- python
- gcc, gcc-g++

## Scripts

- `script/install.sh` — compile the CLI and add it to `PATH`
- `script/commit.sh` — git add, commit, and push with logging