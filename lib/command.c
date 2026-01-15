#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "gui/terminal.h"
#include "filesystem.h"
#include "command.h"

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

int action_goto(TerminalState *state, int argc, char **argv)
{
    if (argc < 2)
    {
        print_stdout("[ERR] not enough arguments\n", state);
        return 0;
    }

    Directory *initial = state->ctx->active_dir;
    char *path = argv[1];

    char *pch = strtok(path, "/");
    while (pch != NULL)
    {
        if (strcmp(pch, "root") == 0)
        {
            state->ctx->active_dir = state->ctx->root_dir;
        }
        else if (strcmp(pch, "<-") == 0)
        {
            state->ctx->active_dir = state->ctx->active_dir->parent;
        }
        else
        {
            Directory *dir = find_dir(state->ctx, pch);

            if (dir == NULL)
            {
                print_stdout("[ERR] no such directory\n", state);
                state->ctx->active_dir = initial;
                return 0;
            }

            state->ctx->active_dir = dir;
        }

        pch = strtok(NULL, "/");
    }
    return 1;
};

int action_mkfile(TerminalState *state, int argc, char **argv)
{
    if (argc < 2)
    {
        print_stdout("[ERR] not enough arguments\n", state);
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        print_stdout("[ERR] keyword root not allowed\n", state);
        return 0;
    }

    if (find_file(state->ctx, argv[1]))
    {
        print_stdout("[ERR] file already created\n", state);
        return 0;
    }

    Directory *initial = state->ctx->active_dir;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(state, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    File *file = create_file(strdup(argv[1]), state->ctx->active_dir);
    add_file(state->ctx->active_dir, file);
    state->ctx->active_dir = initial;
    return 1;
};

int action_mkdir(TerminalState *state, int argc, char **argv)
{
    if (argc < 2)
    {
        print_stdout("[ERR] not enough arguments\n", state);
        return 0;
    }

    if (strcmp(argv[1], "root") == 0)
    {
        print_stdout("[ERR] keyword root not allowed\n", state);
        return 0;
    }

    if (find_dir(state->ctx, argv[1]))
    {
        print_stdout("[ERR] dir already created\n", state);
        return 0;
    }

    Directory *initial = state->ctx->active_dir;

    if (argc == 3)
    {
        char *goto_argv_array[] = {"goto", argv[2]};
        char **goto_argv = goto_argv_array;
        int result = action_goto(state, 2, goto_argv);

        if (!result)
        {
            return 0;
        }
    }

    Directory *dir = create_dir(strdup(argv[1]), state->ctx->active_dir);
    add_dir(state->ctx->active_dir, dir);
    state->ctx->active_dir = initial;
    return 1;
};

int DEFAULT_MAX_DEPTH = 5;

int action_peek(TerminalState *state, int argc, char **argv)
{
    if (argc == 2)
    {
        if (!is_int(argv[1]))
        {
            print_stdout("[ERR] max depth not int\n", state);
            return 0;
        }
        peek_dir(state->ctx->active_dir, atoi(argv[1]));
    }
    else
    {
        peek_dir(state->ctx->active_dir, DEFAULT_MAX_DEPTH);
    }

    return 1;
}

void handle_command(TerminalState *state, Command cmd, int argc, char **argv)
{
    switch (cmd)
    {
    case COMMAND_MKFILE:
        action_mkfile(state, argc, argv);
        break;
    case COMMAND_MKDIR:
        action_mkdir(state, argc, argv);
        break;
    case COMMAND_PEEK:
        action_peek(state, argc, argv);
        break;
    case COMMAND_GOTO:
        action_goto(state, argc, argv);
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