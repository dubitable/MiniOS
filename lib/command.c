#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "filesystem.h"
#include "command.h"

void action_mkfile(Context *ctx, int argc, char **argv)
{
    File *file = create_file(strdup(argv[1]), ctx->active);
    add_file(ctx->active, file);
};

Command cmd_mkfile = {
    .name = COMMAND_MKFILE,
    .action = &action_mkfile,
    .argc = 2};

void action_mkdir(Context *ctx, int argc, char **argv)
{
    Directory *dir = create_dir(strdup(argv[1]), ctx->active);
    add_dir(ctx->active, dir);
};

Command cmd_mkdir = {
    .name = COMMAND_MKDIR,
    .action = &action_mkdir,
    .argc = 2};

void action_pkdir(Context *ctx, int argc, char **argv)
{
    peek_dir(ctx->active);
}

Command cmd_pkdir = {
    .name = COMMAND_PKDIR,
    .action = &action_pkdir,
    .argc = 1};

Directory *find_dir(Context *ctx, char *dir_name)
{
    for (int i = 0; i < ctx->active->dir_count; i++)
    {
        Directory *dir = ctx->active->directories[i];
        if (strcmp(dir->name, dir_name) == 0)
        {
            return dir;
        }
    }

    return NULL;
}

void action_goto(Context *ctx, int argc, char **argv)
{
    char *path = argv[1];

    if (strcmp(path, "root") == 0)
    {
        ctx->active = ctx->root;
        return;
    }

    char *pch = strtok(path, "/");
    while (pch != NULL)
    {
        if (strcmp(pch, "..") == 0)
        {
            ctx->active = ctx->active->parent;
        }
        else
        {
            Directory *dir = find_dir(ctx, pch);

            if (dir == NULL)
            {
                printf("[ERR] No such directory\n");
                return;
            }

            ctx->active = dir;
        }

        pch = strtok(NULL, "/");
    }
};

Command cmd_goto = {
    .name = COMMAND_GOTO,
    .action = &action_goto,
    .argc = 2};

void handle_command(Context *ctx, CommandName cmd, int argc, char **argv)
{
    switch (cmd)
    {
    case COMMAND_MKFILE:
        if (argc == cmd_mkfile.argc)
        {
            cmd_mkfile.action(ctx, argc, argv);
        }
        break;
    case COMMAND_MKDIR:
        if (argc == cmd_mkdir.argc)
        {
            cmd_mkdir.action(ctx, argc, argv);
        }
        break;
    case COMMAND_PKDIR:
        if (argc == cmd_pkdir.argc)
        {
            cmd_pkdir.action(ctx, argc, argv);
        }
        break;
    case COMMAND_GOTO:
        if (argc == cmd_goto.argc)
        {
            cmd_goto.action(ctx, argc, argv);
        }
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
        printf("%s > ", ctx->active->name);

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