# Learning Log

Concepts learned while building `ask`.

---

## Session 1: Project Setup

### What is a Unix pipe?
Programs in Unix can be chained: output of one becomes input of another.
`stdout` of program A → `stdin` of program B.

```bash
cat file.txt | grep "error" | wc -l
#  ↑ stdout    ↑ stdin/stdout  ↑ stdin
```

`ask` fits into this chain — it reads stdin and writes to stdout.

### File Descriptors
Every process gets 3 by default:
- fd 0 = stdin (keyboard or pipe input)
- fd 1 = stdout (terminal or pipe output)
- fd 2 = stderr (error messages, always terminal)

### Dynamic Memory in C
Unlike Python/JS, C doesn't grow arrays automatically.
- `malloc(size)` — allocate bytes on heap
- `realloc(ptr, new_size)` — grow/shrink allocation
- `free(ptr)` — release memory
- If you forget `free` → memory leak
- If you use after `free` → crash (use-after-free bug)

---

---

## Session 1: Concepts Explained

### OpenAI Compatibility — Why?

OpenAI invented a REST API format for talking to LLMs:
```
POST /v1/chat/completions
{"messages": [{"role": "user", "content": "hello"}]}
```

Everyone copied this format: Ollama, llama.cpp, vLLM, LiteLLM, Anthropic adapters.

Why it matters for `ask`: by targeting this format, we work with ANY backend.
bitnet-server, Ollama, GPT-4, Claude (via adapter) — all speak the same protocol.

One tool, any brain.

### snprintf — String Building in C

C has no string concatenation like Python (`"hello" + "world"`).
Instead you FORMAT strings into a buffer:

```c
char buf[100];
snprintf(buf, 100, "Hello %s, you are %d years old", name, age);
//                   ↑ format string   ↑ variables fill in %s, %d
```

Format specifiers:
- %s = string (char*)
- %d = integer
- %zu = size_t (unsigned size)
- %f = float

`snprintf(dest, max_size, format, ...)`:
- `dest` = where to write
- `max_size` = don't write more than this many bytes (prevents buffer overflow)
- `format` = template with %s/%d placeholders
- `...` = values to fill in

### strstr and strchr — Finding Text in C

```c
char *strstr(haystack, needle)  // find substring in string
char *strchr(string, character) // find single character in string
```

Both return a POINTER to where they found it, or NULL if not found.

In handle_response:
```c
pos = strstr(pos, "\"content\":\"")  // find "content":" in the response
pos += 11;                           // skip past those 11 characters
end = strchr(pos, '"');              // find the closing quote
fwrite(pos, 1, end - pos, stdout);  // print everything between pos and end
```

Visual:
```
..."content":"bonjour"...
              ^pos    ^end
              |_______|
              fwrite prints this
```

### Escaped Characters in JSON

JSON uses backslash for special characters:
- \" = literal quote (not end of string)
- \n = newline
- \\ = literal backslash

Our parser currently prints these literally (\Bonjour\).
We'll fix this later by converting escapes.

---

(More entries added as we build)