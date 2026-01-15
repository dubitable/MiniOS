#include "lib/filesystem.h"
#include "lib/command.h"
#include "lib/minigui.h"

int main()
{
    Context *ctx = initialize();
    open_terminal(ctx);
    free_ctx(ctx);

    return 0;
}