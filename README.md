# ask

Pipe anything to a local LLM. Single binary. No dependencies.

```bash
cat error.log | ask "summarize"
git diff | ask "commit message"
curl api.com/data | ask "extract emails as json"
```

Works with any OpenAI-compatible API (bitnet-server, Ollama, llama.cpp, GPT-4).

## Install

```bash
make
sudo cp ask /usr/local/bin/
```

## Usage

```bash
# Set endpoint (default: http://localhost:11435)
export ASK_ENDPOINT=http://localhost:11435
export ASK_MODEL=bitnet-2b4t

# Pipe input + prompt
echo "hello world" | ask "translate to french"

# Prompt only (no stdin)
ask "what is 2+2"

# File input
ask "find bugs" < main.c
```

## Config

Environment variables:
- `ASK_ENDPOINT` — API base URL (default: `http://localhost:11435`)
- `ASK_MODEL` — model name (default: empty, uses server default)

## Build

```bash
make        # build
make clean  # clean
```

Requires: C compiler, libcurl.

## Design

- Reads stdin → builds chat completion request → streams response to stdout
- Detects TTY vs pipe (different output formatting)
- Streams tokens as they arrive (Server-Sent Events)
- Single C file, ~500 lines

## License

MIT
