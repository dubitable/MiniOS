#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "filesystem.h"

Directory *create_dir(char *name, Directory *parent)
{
    Directory *dir = malloc(sizeof(Directory));
    dir->name = name;
    dir->parent = parent;
    dir->dir_count = 0;
    dir->file_count = 0;
    dir->directories = NULL;
    dir->files = NULL;
    return dir;
}

Context *initialize()
{
    Directory *root = create_dir("root", NULL);
    Directory *active = root;

    Context *ctx = malloc(sizeof(Context));
    ctx->root = root;
    ctx->active = active;

    return ctx;
}

File *create_file(char *name, Directory *parent)
{
    File *file = malloc(sizeof(File));
    file->name = name;
    file->parent = parent;
    file->content = "";
    return file;
}

Directory *add_file(Directory *parent, File *file)
{
    parent->files = realloc(parent->files, (parent->file_count + 1) * sizeof(File *));
    parent->files[parent->file_count] = file;
    parent->file_count++;
    return parent;
}

Directory *add_dir(Directory *parent, Directory *dir)
{
    parent->directories = realloc(parent->directories, (parent->dir_count + 1) * sizeof(Directory *));
    parent->directories[parent->dir_count] = dir;
    parent->dir_count++;
    return parent;
}

void rec_peek_dir(Directory *dir, int depth, int max_depth)
{
    printf("%s\n", dir->name);

    if (max_depth == 0)
    {
        return;
    }

    for (int i = 0; i < dir->dir_count; i++)
    {
        for (int d = 0; d < depth; d++)
            printf("    ");
        printf("|-- ");
        rec_peek_dir(dir->directories[i], depth + 1, max_depth - 1);
    }

    for (int j = 0; j < dir->file_count; j++)
    {
        for (int d = 0; d < depth; d++)
            printf("    ");
        printf("|-- %s\n", dir->files[j]->name);
    }
}

void peek_dir(Directory *dir, int max_depth)
{
    rec_peek_dir(dir, 0, max_depth);
}

void free_dir(Directory *dir)
{
    for (int i = 0; i < dir->dir_count; i++)
    {
        free_dir(dir->directories[i]);
    }
    for (int j = 0; j < dir->file_count; j++)
    {
        free(dir->files[j]->content);
        free(dir->files[j]);
    }

    free(dir->files);
    free(dir->directories);
    free(dir);
}