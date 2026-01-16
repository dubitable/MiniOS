#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "filesystem.h"
#include "command.h"

void rec_peek_dir(char *printto, int size, Directory *dir, int depth, int max_depth)
{
    int DIRNAME_SIZE = strlen(dir->name) + 3;
    char dirname[DIRNAME_SIZE];
    snprintf(dirname, DIRNAME_SIZE, "%s/\n", dir->name);
    snprintf(printto, size, "%s%s", printto, dirname);

    if (max_depth == 0)
    {
        return;
    }

    for (int i = 0; i < dir->dir_count; i++)
    {
        for (int d = 0; d < depth; d++)
            snprintf(printto, size, "%s%s", printto, "    ");
        snprintf(printto, size, "%s%s", printto, "|-- ");

        rec_peek_dir(printto, size, dir->directories[i], depth + 1, max_depth - 1);
    }

    for (int j = 0; j < dir->file_count; j++)
    {
        for (int d = 0; d < depth; d++)
            snprintf(printto, size, "%s%s", printto, "    ");

        int FILENAME_SIZE = strlen(dir->files[j]->name) + 6;
        char filename[FILENAME_SIZE];
        snprintf(filename, FILENAME_SIZE, "|-- %s\n", dir->files[j]->name);
        snprintf(printto, size, "%s%s", printto, filename);
    }
}

void peek_dir(char *printto, int size, Directory *dir, int max_depth)
{
    rec_peek_dir(printto, size, dir, 0, max_depth);
}

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

int is_int(char *line)
{
    int current = 0;

    while (line[current] != '\0')
    {
        if (!isdigit(line[current]))
        {
            return 0;
        }

        current += 1;
    }

    return 1;
}

int action_goto(Context *ctx, char *printto, int size, int argc, char **argv)
{
    if (argc < 2)
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] not enough arguments\n");
        return 0;
    }

    Directory *initial = ctx->active_dir;
    char *path = argv[1];

    char *pch = strtok(path, "/");
    while (pch != NULL)
    {
        if (strcmp(pch, "root") == 0)
        {
            ctx->active_dir = ctx->root_dir;
        }
        else if (strcmp(pch, "<-") == 0)
        {
            ctx->active_dir = ctx->active_dir->parent;
        }
        else
        {
            Directory *dir = find_dir(ctx, pch);

            if (dir == NULL)
            {
                snprintf(printto, size, "%s%s", printto, "[ERR] no such directory\n");
                ctx->active_dir = initial;
                return 0;
            }

            ctx->active_dir = dir;
        }

        pch = strtok(NULL, "/");
    }
    return 1;
};

int action_mkfile(Context *ctx, char *printto, int size, int argc, char **argv)
{
    if (argc < 2)
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] not enough arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] keyword root not allowed\n");
        return 0;
    }

    if (find_file(ctx, argv[1]))
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] file already created\n");
        return 0;
    }

    Directory *initial = ctx->active_dir;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(ctx, printto, size, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    File *file = create_file(strdup(argv[1]), ctx->active_dir);
    add_file(ctx->active_dir, file);
    ctx->active_dir = initial;
    return 1;
};

int action_mkdir(Context *ctx, char *printto, int size, int argc, char **argv)
{
    if (argc < 2)
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] not enough arguments\n");
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] keyword root not allowed\n");
        return 0;
    }

    if (find_dir(ctx, argv[1]))
    {
        snprintf(printto, size, "%s%s", printto, "[ERR] dir already created\n");
        return 0;
    }

    Directory *initial = ctx->active_dir;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(ctx, printto, size, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    Directory *dir = create_dir(strdup(argv[1]), ctx->active_dir);
    add_dir(ctx->active_dir, dir);
    ctx->active_dir = initial;
    return 1;
};

int DEFAULT_MAX_DEPTH = 5;

int action_peek(Context *ctx, char *printto, int size, int argc, char **argv)
{
    if (argc == 2)
    {
        if (!is_int(argv[1]))
        {
            snprintf(printto, size, "%s%s", printto, "[ERR] max depth not int\n");
            return 0;
        }
        peek_dir(printto, size, ctx->active_dir, atoi(argv[1]));
    }
    else
    {
        peek_dir(printto, size, ctx->active_dir, DEFAULT_MAX_DEPTH);
    }

    return 1;
}

int action_pong(Context *ctx)
{
    ctx->active_window = WINDOW_PONG;

    return 1;
}

void handle_command(Command cmd, int argc, char **argv, Context *ctx, char *printto, int size)
{
    switch (cmd)
    {
    case COMMAND_MKFILE:
        action_mkfile(ctx, printto, size, argc, argv);
        break;
    case COMMAND_MKDIR:
        action_mkdir(ctx, printto, size, argc, argv);
        break;
    case COMMAND_PEEK:
        action_peek(ctx, printto, size, argc, argv);
        break;
    case COMMAND_GOTO:
        action_goto(ctx, printto, size, argc, argv);
        break;
    case COMMAND_PONG:
        action_pong(ctx);
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
    if (strcmp(str, "pong") == 0)
        return COMMAND_PONG;
    return COMMAND_NONE;
}