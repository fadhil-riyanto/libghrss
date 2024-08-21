#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <stdlib.h>
#include "header/ghrss.h"
#include <curl/curl.h>
#include <string>
#include <fmt/core.h>


static void _init_curl(gh_rss_ctx_t *ctx)
{
        ctx->curl_ctx = curl_easy_init();
}

// static size_t curl_write_mem_cb(void* contents, size_t size, size_t n, void *userp)
// {
//         size_t realsize = size * n;

// }

static inline void _peform(gh_rss_ctx_t *ctx, std::string builded_url)
{
        CURLcode res;
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_URL, builded_url.c_str());
        curl_easy_setopt(ctx->curl_ctx, CURLOPT_FOLLOWLOCATION, 1L);


        res = curl_easy_perform(ctx->curl_ctx);

        if (res != CURLE_OK) {
                fprintf(stderr, "%s\n", curl_easy_strerror(res));
        }
}

static inline std::string build_github_query(const char* username, const char* reponame)
{
        std::string res = fmt::format("https://github.com/{0}/{1}/releases.atom", 
                                                username, reponame);

        return res;
}

void gh_rss_init(gh_rss_ctx_t *ctx) 
{
        /* init heap */
        // ctx->mem_ptr = (char*)malloc(1);
        // ctx->memsize =   0;
        ctx->memory_struct.mem_ptr = (char*)malloc(1);
        ctx->memory_struct.memsize = 1;

        /* init curl */
        curl_global_init(CURL_GLOBAL_ALL);
        _init_curl(ctx);

        /* test purpose */
        

}

void gh_rss_get_updates(gh_rss_ctx_t *ctx, const char* username, const char* repo)
{
        std::string url = build_github_query(username, repo);
        _peform(ctx, url.c_str());
}

void gh_rss_free(gh_rss_ctx_t *ctx) 
{
        free(ctx->memory_struct.mem_ptr);

        curl_easy_cleanup(ctx->curl_ctx);
        curl_global_cleanup();
}