#include "../header/ghrss.h"
#include <sys/socket.h>

int main()
{
        gh_rss_ctx_t ctx;
        gh_rss_init(&ctx);
        
        gh_rss_get_updates(&ctx, "grpc", "grpc");
        gh_rss_get_updates(&ctx, "grpc", "grpc");
        gh_rss_get_updates(&ctx, "grpc", "grpc");
        // gh_rss_get_updates(&ctx, "archlinux", "linux");

        gh_rss_free(&ctx);
}