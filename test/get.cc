#include "../header/ghrss.h"
#include <csignal>
#include <cstdio>
#include <sys/socket.h>
#include<unistd.h>

volatile int state = 0;

void signal_cb(int signum)
{
        state = 1;
}

int main()
{
        signal(SIGINT, signal_cb);

        char* id;
        gh_rss_ctx_t ctx;
        gh_rss_init(&ctx);
        
        
        while (!state) {
                gh_rss_get_updates(&ctx, "grpc", "grpc");

                id = gh_rss_list(&ctx);
                if (id != 0) {
                        printf("new update: %s\n", id);
                        gh_rss_seen(&ctx, id);
                        free(id);

                        
                        
                } else {
                        sleep(1);
                }
        }

        printf("separator\n");
        
        
        // gh_rss_get_updates(&ctx, "archlinux", "linux");

        gh_rss_free(&ctx);
}