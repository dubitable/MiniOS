gcc minios.c lib/filesystem.c lib/command.c lib/helpers.c lib/minigui.c \
    $(pkg-config --cflags --libs raylib) \
    -o minios