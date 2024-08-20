#ifndef GHRSS_H
#define GHRSS_H

#include <curl/curl.h>
#include <stdio.h>

typedef struct gh_rss_ctx {
        char *mem_ptr;
        size_t memsize;

        CURL* curl_ctx;
} gh_rss_ctx_t;

void gh_rss_init(gh_rss_ctx_t *ctx);
void gh_rss_free(gh_rss_ctx_t *ctx);

#endif