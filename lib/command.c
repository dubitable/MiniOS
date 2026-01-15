#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filesystem.h"
#include "command.h"
#include "helpers.h"

int action_goto(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return 0;
    }

    Directory *initial = ctx->active;
    char *path = argv[1];

    char *pch = strtok(path, "/");
    while (pch != NULL)
    {
        if (strcmp(pch, "root") == 0)
        {
            ctx->active = ctx->root;
        }
        else if (strcmp(pch, "<-") == 0)
        {
            ctx->active = ctx->active->parent;
        }
        else
        {
            Directory *dir = find_dir(ctx, pch);

            if (dir == NULL)
            {
                printf("[ERR] no such directory\n");
                ctx->active = initial;
                return 0;
            }

            ctx->active = dir;
        }

        pch = strtok(NULL, "/");
    }
    return 1;
};

int action_mkfile(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        printf("[ERR] keyword root not allowed\n");
        return 0;
    }

    if (find_file(ctx, argv[1]))
    {
        printf("[ERR] file already created\n");
        return 0;
    }

    Directory *initial = ctx->active;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(ctx, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    File *file = create_file(strdup(argv[1]), ctx->active);
    add_file(ctx->active, file);
    ctx->active = initial;
    return 1;
};

int action_mkdir(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        printf("[ERR] keyword root not allowed\n");
        return 0;
    }

    if (find_dir(ctx, argv[1]))
    {
        printf("[ERR] dir already created\n");
        return 0;
    }

    Directory *initial = ctx->active;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(ctx, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    Directory *dir = create_dir(strdup(argv[1]), ctx->active);
    add_dir(ctx->active, dir);
    ctx->active = initial;
    return 1;
};

int DEFAULT_MAX_DEPTH = 5;

int action_peek(Context *ctx, int argc, char **argv)
{
    if (argc == 2)
    {
        if (!is_int(argv[1]))
        {
            printf("[ERR] max depth not int\n");
            return 0;
        }
        peek_dir(ctx->active, atoi(argv[1]));
    }
    else
    {
        peek_dir(ctx->active, DEFAULT_MAX_DEPTH);
    }

    return 1;
}

void handle_command(Context *ctx, Command cmd, int argc, char **argv)
{
    switch (cmd)
    {
    case COMMAND_MKFILE:
        action_mkfile(ctx, argc, argv);
        break;
    case COMMAND_MKDIR:
        action_mkdir(ctx, argc, argv);
        break;
    case COMMAND_PEEK:
        action_peek(ctx, argc, argv);
        break;
    case COMMAND_GOTO:
        action_goto(ctx, argc, argv);
        break;
    default:
        break;
    }
}

Command command_from_string(const char *str)
{
    if (strcmp(str, "mkfile") == 0)
        return COMMAND_MKFILE;
    if (strcmp(str, "mkdir") == 0)
        return COMMAND_MKDIR;
    if (strcmp(str, "peek") == 0)
        return COMMAND_PEEK;
    if (strcmp(str, "goto") == 0)
        return COMMAND_GOTO;
    return COMMAND_NONE;
}