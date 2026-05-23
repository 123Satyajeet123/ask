#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

size_t handle_response(char *data, size_t size, size_t nmemb, void *userdata) {
    (void)userdata;
    char *pos = data;
    while ((pos = strstr(pos, "\"content\":\"")) != NULL) {
        pos += 11;
        char *end = strchr(pos, '"');
        if (end) {
            fwrite(pos, 1, end - pos, stdout);
            pos = end;
        }
    }
    fflush(stdout);
    return size * nmemb;
}

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--help") == 0) {
      fprintf(stderr, "usage: ask <prompt>\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Pipe text to an LLM. Works with any OpenAI-compatible API.\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "  echo \"hello\" | ask \"translate to french\"\n");
      fprintf(stderr, "  ask \"what is 2+2\"\n");
      fprintf(stderr, "\n");
      fprintf(stderr, "Environment:\n");
      fprintf(stderr, "  ASK_ENDPOINT  API base URL (default: http://localhost:11435)\n");
      return argc < 2 ? 1 : 0;
  }

    // Endpoint from env or default
    const char *endpoint = getenv("ASK_ENDPOINT");
    if (!endpoint) {
        endpoint = "http://localhost:11435";
    }

    char url[512];
    snprintf(url, sizeof(url), "%s/v1/chat/completions", endpoint);

    // Init curl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "ask: failed to init curl\n");
        return 1;
    }

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_response);

    // Read stdin if piped
    size_t capacity = 4096;
    char *buf = malloc(capacity);
    if (!buf) {
        perror("malloc");
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return 1;
    }
    size_t len = 0;

    if (!isatty(0)) {
        ssize_t n;
        while ((n = read(0, buf + len, capacity - len)) > 0) {
            len += n;
            if (len == capacity) {
                capacity *= 2;
                char *new_buf = realloc(buf, capacity);
                if (!new_buf) {
                    perror("realloc");
                    free(buf);
                    curl_slist_free_all(headers);
                    curl_easy_cleanup(curl);
                    return 1;
                }
                buf = new_buf;
            }
        }
    }
    buf[len] = '\0';

    // Strip trailing newline
    while (len > 0 && buf[len - 1] == '\n') {
        buf[--len] = '\0';
    }

    // Build JSON payload
    char *payload = malloc(len + 512);
    if (len > 0) {
        snprintf(payload, len + 512,
            "{\"messages\":[{\"role\":\"user\",\"content\":\"%s\\n\\n%s\"}],\"stream\":true}",
            argv[1], buf);
    } else {
        snprintf(payload, 512,
            "{\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],\"stream\":true}",
            argv[1]);
    }

    // Send request
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_perform(curl);
    printf("\n");

    // Cleanup
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(payload);
    free(buf);

    return 0;
}
