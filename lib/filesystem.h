#ifndef FILESYSTEM_H_INCLUDED
#define FILESYSTEM_H_INCLUDED

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

typedef struct
{
    Directory *root;
    Directory *active;
} Context;

Context *initialize();

Directory *create_dir(char *name, Directory *parent);
File *create_file(char *name, Directory *parent);

Directory *add_file(Directory *parent, File *file);
Directory *add_dir(Directory *parent, Directory *dir);

void peek_dir(Directory *dir, int max_depth);

void free_dir(Directory *dir);

#endif