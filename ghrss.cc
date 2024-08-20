#include <stdlib.h>
#include "header/ghrss.h"
#include <curl/curl.h>

void gh_rss_init(gh_rss_ctx_t *ctx) 
{
        /* init heap */
        ctx->mem_ptr = (char*)malloc(1);
        ctx->memsize = 0;
}

void gh_rss_free(gh_rss_ctx_t *ctx) 
{
        free(ctx->mem_ptr);
}