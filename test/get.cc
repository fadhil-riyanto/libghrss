#include "../header/ghrss.h"
#include <sys/socket.h>

int main()
{
        gh_rss_ctx_t ctx;
        gh_rss_init(&ctx);

        gh_rss_free(&ctx);
}