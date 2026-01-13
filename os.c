#include "lib/filesystem.h"
#include "lib/command.h"

int main()
{
    Context *ctx = initialize();

    mini_terminal(ctx);

    free_dir(ctx->root);

    return 0;
}