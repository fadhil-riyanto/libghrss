#include <cstdio>
#include <stdlib.h>
#include "header/ghrss.h"
#include <curl/curl.h>
#include <string>
#include <fmt/core.h>

static void _init_curl(gh_rss_ctx_t *ctx)
{
        ctx->curl_ctx = curl_easy_init();
}

static inline void _peform(gh_rss_ctx_t *ctx, std::string builded_url)
{
        CURLcode res;
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_URL, "https://google.com");
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_FOLLOWLOCATION, 1L);


        res = curl_easy_perform(ctx->curl_ctx);

        if (res != CURLE_OK) {
                fprintf(stderr, "%s\n", curl_easy_strerror(res));
        }
}

static inline std::string build_github_query(std::string username, std::string reponame)
{
        std::string res = fmt::format("https://github.com/{0}/{1}/releases.atom", 
                                                username, reponame);

        return res;
}

void gh_rss_init(gh_rss_ctx_t *ctx) 
{
        /* init heap */
        ctx->mem_ptr = (char*)malloc(1);
        ctx->memsize =   0;

        /* init curl */
        curl_global_init(CURL_GLOBAL_ALL);
        _init_curl(ctx);

        /* test purpose */
        _peform(ctx, "google.com");

}

void gh_rss_free(gh_rss_ctx_t *ctx) 
{
        free(ctx->mem_ptr);

        curl_global_cleanup();
}