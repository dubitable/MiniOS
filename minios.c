#include "lib/filesystem.h"
#include "lib/gui/minigui.h"

int main()
{
    Context *ctx = initialize();
    initialize_window(ctx);
    free_ctx(ctx);
    return 0;
}