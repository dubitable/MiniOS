#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

#include "gui/cube.h"

typedef struct File File;
typedef struct Dir Directory;

struct Dir
{
    char *name;
    struct Dir *parent;

    int dir_count;
    int file_count;

    struct Dir **directories;
    File **files;
};

struct File
{
    char *name;
    Directory *parent;
    char *content;
};

typedef enum
{
    WINDOW_WELCOME,
    WINDOW_TERMINAL
} Window;

typedef struct
{
    Directory *root_dir;
    Directory *active_dir;

    Window active_window;
} Context;

enum
{
    PATH_SIZE = 100,
    INPUT_SIZE = 100,
    STDOUT_SIZE = 200,
};

typedef struct
{
    Context *ctx;
    char path[PATH_SIZE];
    Point2D cursor;

    char input[INPUT_SIZE];
    int input_count;

    char std_out[STDOUT_SIZE];
} TerminalState;

Context *initialize();

Directory *create_dir(char *name, Directory *parent);
File *create_file(char *name, Directory *parent);

Directory *add_file(Directory *parent, File *file);
Directory *add_dir(Directory *parent, Directory *dir);

Directory *find_dir(Context *ctx, char *dir_name);
File *find_file(Context *ctx, char *filename);

void free_dir(Directory *dir);
void free_ctx(Context *ctx);

void where(Directory *current, char *buffer, int size);

#endif