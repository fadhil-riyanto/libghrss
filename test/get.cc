#include "../header/ghrss.h"
#include <cstdio>
#include <sys/socket.h>

int main()
{
        char* id;
        gh_rss_ctx_t ctx;
        gh_rss_init(&ctx);
        
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

        printf("separator\n");

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
        
        
        // gh_rss_get_updates(&ctx, "archlinux", "linux");

        gh_rss_free(&ctx);
}