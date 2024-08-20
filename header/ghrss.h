#ifndef GHRSS_H
#define GHRSS_H

#include <stdio.h>

typedef struct gh_rss_ctx {
        char *mem_ptr;
        size_t memsize;
} gh_rss_ctx_t;

extern void gh_rss_init(gh_rss_ctx_t *ctx);
extern void gh_rss_free(gh_rss_ctx_t *ctx);

#endif