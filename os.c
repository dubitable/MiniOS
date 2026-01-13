#include "lib/filesystem.h"
#include "lib/command.h"

int main()
{
    Directory *root = initialize();
    Directory *active = root;

    Context ctx = {
        .root = root,
        .active = active};

    mini_terminal(&ctx);

    free_dir(root);
    return 0;
}