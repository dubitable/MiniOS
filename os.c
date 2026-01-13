#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char *name;
} File;

typedef struct Dir
{
    char *name;

    int dir_count;
    int file_count;

    struct Dir **directories;
    File **files;
} Directory;

Directory *create_dir(char *name)
{
    Directory *root = malloc(sizeof(Directory));
    root->name = name;
    root->dir_count = 0;
    root->file_count = 0;
    root->directories = NULL;
    root->files = NULL;
    return root;
}

File *create_file(char *name)
{
    File *file = malloc(sizeof(File));
    file->name = name;
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

void rec_peek_dir(Directory *dir, int depth) {
    printf("%s\n", dir->name);

    for (int i = 0; i < dir->dir_count; i++) {
        for (int d = 0; d < depth; d++) printf("    ");
        printf("|-- ");
        rec_peek_dir(dir->directories[i], depth + 1);
    }

    for (int j = 0; j < dir->file_count; j++) {
        for (int d = 0; d < depth; d++) printf("    ");
        printf("|-- %s\n", dir->files[j]->name);
    }
}

void peek_dir(Directory *dir) {
    rec_peek_dir(dir, 0);
}

void free_dir(Directory *dir)
{
    for (int i = 0; i < dir->dir_count; i++)
    {
        free_dir(dir->directories[i]);
    }
    for (int j = 0; j < dir->file_count; j++)
    {
        free(dir->files[j]);
    }

    free(dir->files);
    free(dir->directories);
    free(dir);
}

int main()
{
    Directory *root = create_dir("root");
    
    Directory *myFiles = create_dir("myFiles");
    File* file1 = create_file("myTxt.txt");
    File* file2 = create_file("myPy.py");

    Directory *mySecrets = create_dir("mySecrets");
    File* secret1 = create_file("mySecret1.txt");
    File* secret2 = create_file("mySecret2");

    add_dir(root, myFiles);
    add_dir(myFiles, mySecrets);

    add_file(myFiles, file1);
    add_file(myFiles, file2);

    add_file(mySecrets, secret1);
    add_file(mySecrets, secret2);

    peek_dir(root);

    free_dir(root);

    return 0;
}