#include "../header/ghrss.h"
#include <cstdio>
#include <sys/socket.h>
#include<unistd.h>

int main()
{
        char* id;
        gh_rss_ctx_t ctx;
        gh_rss_init(&ctx);
        
        gh_rss_get_updates(&ctx, "grpc", "grpc");
hitori:
        gh_rss_get_updates(&ctx, "grpc", "grpc");
        while (1) {
                id = gh_rss_list(&ctx);
                if (id != 0) {
                        printf("new update: %s\n", id);
                        gh_rss_seen(&ctx, id);
                        free(id);
                } else {
                        break;
                }
        }
        sleep(1);
        goto hitori;
        printf("separator\n");
        
        
        // gh_rss_get_updates(&ctx, "archlinux", "linux");

        gh_rss_free(&ctx);
}