#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filesystem.h"
#include "command.h"
#include "helpers.h"

void where(Directory *current)
{
    if (current == NULL)
    {
        return;
    }

    where(current->parent);
    printf("%s/", current->name);
}

bool action_goto(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return false;
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
                return false;
            }

            ctx->active = dir;
        }

        pch = strtok(NULL, "/");
    }
    return true;
};

Command cmd_goto = {
    .name = COMMAND_GOTO,
    .action = &action_goto};

bool action_mkfile(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return false;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        printf("[ERR] keyword root not allowed\n");
        return false;
    }

    if (find_file(ctx, argv[1]))
    {
        printf("[ERR] file already created\n");
        return false;
    }

    Directory *initial = ctx->active;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        bool result = action_goto(ctx, 2, goto_argv);

        if (!result)
        {
            return false;
        }
    }

    File *file = create_file(strdup(argv[1]), ctx->active);
    add_file(ctx->active, file);
    ctx->active = initial;
    return true;
};

Command cmd_mkfile = {
    .name = COMMAND_MKFILE,
    .action = &action_mkfile};

bool action_mkdir(Context *ctx, int argc, char **argv)
{
    if (argc < 2)
    {
        printf("[ERR] not enough arguments\n");
        return false;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        printf("[ERR] keyword root not allowed\n");
        return false;
    }

    if (find_dir(ctx, argv[1]))
    {
        printf("[ERR] dir already created\n");
        return false;
    }

    Directory *initial = ctx->active;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        bool result = action_goto(ctx, 2, goto_argv);

        if (!result)
        {
            return false;
        }
    }

    Directory *dir = create_dir(strdup(argv[1]), ctx->active);
    add_dir(ctx->active, dir);
    ctx->active = initial;
    return true;
};

Command cmd_mkdir = {
    .name = COMMAND_MKDIR,
    .action = &action_mkdir};

int DEFAULT_MAX_DEPTH = 5;

bool action_pkdir(Context *ctx, int argc, char **argv)
{
    if (argc == 2)
    {
        if (!is_int(argv[1]))
        {
            printf("[ERR] max depth not int\n");
            return false;
        }
        peek_dir(ctx->active, atoi(argv[1]));
    }
    else
    {
        peek_dir(ctx->active, DEFAULT_MAX_DEPTH);
    }

    return true;
}

Command cmd_pkdir = {
    .name = COMMAND_PKDIR,
    .action = &action_pkdir};

void handle_command(Context *ctx, CommandName cmd, int argc, char **argv)
{
    switch (cmd)
    {
    case COMMAND_MKFILE:
        cmd_mkfile.action(ctx, argc, argv);
        break;
    case COMMAND_MKDIR:
        cmd_mkdir.action(ctx, argc, argv);
        break;
    case COMMAND_PKDIR:
        cmd_pkdir.action(ctx, argc, argv);
        break;
    case COMMAND_GOTO:
        cmd_goto.action(ctx, argc, argv);
        break;
    default:
        break;
    }
}

typedef struct
{
    int argc;
    char **argv;
} Args;

Args *parse_line(char line[])
{
    Args *args = malloc(sizeof(Args));
    args->argc = 0;
    args->argv = NULL;

    char *pch = strtok(line, " ");
    while (pch != NULL)
    {
        args->argc++;

        args->argv = realloc(args->argv, args->argc * sizeof(char *));
        args->argv[args->argc - 1] = strdup(pch);

        pch = strtok(NULL, " ");
    }

    return args;
}

void free_args(Args *args)
{
    if (args == NULL)
    {
        return;
    }

    for (int i = 0; i < args->argc; i++)
    {
        free(args->argv[i]);
    }

    free(args->argv);
    free(args);
}

CommandName command_from_string(const char *str)
{
    if (strcmp(str, "mkfile") == 0)
        return COMMAND_MKFILE;
    if (strcmp(str, "mkdir") == 0)
        return COMMAND_MKDIR;
    if (strcmp(str, "pkdir") == 0)
        return COMMAND_PKDIR;
    if (strcmp(str, "goto") == 0)
        return COMMAND_GOTO;
    return COMMAND_NONE;
}

void mini_terminal(Context *ctx)
{
    bool running = true;

    while (running)
    {
        where(ctx->active);
        printf(" > ");

        char line[1024];

        if (fgets(line, sizeof(line), stdin))
        {
            line[strcspn(line, "\n")] = 0;
        }

        if (strcmp(line, "exit") == 0)
        {
            running = false;
            break;
        }

        Args *args = parse_line(line);

        if (args->argc == 0)
        {
            continue;
        }

        CommandName cmd = command_from_string(args->argv[0]);

        handle_command(ctx, cmd, args->argc, args->argv);

        free_args(args);
    }
}