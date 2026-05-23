# ask — Build Plan

## What

Unix pipe filter for LLMs. Single C binary. Works with any OpenAI-compatible API.

## Why

- `llm` (Python, 12k stars) and `mods` (Go, 4.5k stars) exist but need runtimes
- Zero C implementations exist for this pattern
- Teaches: C, HTTP, JSON parsing, SSE streaming, Unix pipes

## Architecture

```
stdin → [buffer] → [json payload] → [HTTP POST] → [SSE stream] → stdout
```

## Build Steps (in order)

### Phase 1: stdin + args (Day 1 morning)
- [ ] Read stdin into dynamic buffer
- [ ] Parse argv for prompt
- [ ] Print both to verify

### Phase 2: HTTP request (Day 1 afternoon)
- [ ] Build OpenAI chat completion JSON payload
- [ ] POST to endpoint using libcurl
- [ ] Print raw response

### Phase 3: SSE streaming (Day 2 morning)
- [ ] Parse Server-Sent Events (data: {...})
- [ ] Extract token content from JSON chunks
- [ ] Print tokens as they arrive

### Phase 4: Polish (Day 2 afternoon)
- [ ] TTY detection (isatty) — format differently when piped vs terminal
- [ ] Error handling (connection refused, invalid response)
- [ ] Env var config (ASK_ENDPOINT, ASK_MODEL)
- [ ] --help flag
- [ ] Makefile

### Phase 5: Ship
- [ ] Test with bitnet-server
- [ ] Test with Ollama
- [ ] Push to GitHub
- [ ] Post on X/Reddit

## Key Technical Decisions

1. **libcurl** for HTTP (not raw sockets) — weekend project, optimize later
2. **Manual JSON construction** (sprintf, not a JSON library) — payloads are simple
3. **Manual SSE parsing** (string matching on "data: ") — format is trivial
4. **No malloc where avoidable** — stack buffers for small things, heap for stdin

## Files

```
ask/
├── main.c          # everything in one file for now
├── Makefile        # build
├── README.md       # usage docs
├── PLAN.md         # this file
└── LEARNING.md     # concepts learned along the way
```

## Learning Goals

- Dynamic memory (malloc/realloc/free)
- File descriptors (stdin = fd 0)
- HTTP protocol (headers, body, streaming)
- JSON format (manual construction + parsing)
- Server-Sent Events (SSE) protocol
- Environment variables (getenv)
- TTY detection (isatty)
- Makefile basics
- Error handling in C (no exceptions, just return codes)
