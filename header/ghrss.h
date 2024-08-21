#ifndef GHRSS_H
#define GHRSS_H

#include <curl/curl.h>
#include <map>
#include <stdio.h>
#include <string>

struct memory_struct {
        char *mem_ptr;
        size_t memsize;
};

struct memory_db_cache {
        std::map<std::string, int> db;
        int size;
};

typedef struct gh_rss_ctx {
        CURL* curl_ctx;
        struct memory_struct memory_struct;
        struct memory_db_cache memdb;
} gh_rss_ctx_t;

void gh_rss_init(gh_rss_ctx_t *ctx);
void gh_rss_get_updates(gh_rss_ctx_t *ctx, const char* username, const char* repositort);
char* gh_rss_list(gh_rss_ctx_t *ctx);
void gh_rss_seen(gh_rss_ctx_t *ctx, char* special);
void gh_rss_free(gh_rss_ctx_t *ctx);

#endif