gcc minios.c\
    lib/filesystem.c lib/command.c \
    lib/gui/minigui.c lib/gui/cube.c lib/gui/terminal.c \
    $(pkg-config --cflags --libs raylib) \
    -o minios \
    -Werror -Wall -Wextra