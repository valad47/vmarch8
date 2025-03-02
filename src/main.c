#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool debug_mode = false;
bool show_fps = false;
int fps = 0;

#define TEXT_DEFAULT "\033[39m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_LIGHTBLUE "\033[94m"

int main(int argc, char **argv) {
    for(int i = 1; i < argc; i++) {
        if(!strcmp("--debug", argv[i])) {
            debug_mode = true;
            continue;
        }
        if(!strncmp("--fps=", argv[i], 6)) {
            fps = atoi(argv[i]+6);
            continue;
        }
        if(!strcmp("--show-fps", argv[i])) {
            show_fps = true;
            continue;
        }
        if(!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
            printf(
            "Usage: vmarch8 [flags]\n"
            "\n"
            "Flags:\n"
            "\t-h, --help\t- print this message\n"
            "\t--debug\t\t- enable debug information\n"
            "\t--show-fps\t- show fps\n"
            "\t--fps=<int>\t- set program FPS (default is inf)\n"
            );
            return 0;
        }

        printf("[" TEXT_YELLOW "WARNING" TEXT_DEFAULT "] Got unexpected argument " TEXT_LIGHTBLUE "%s" TEXT_DEFAULT "\n", argv[i]);
    }

    SetTraceLogLevel(debug_mode ? LOG_ALL : LOG_NONE);
    InitWindow(800, 400, "vmarch8");
    SetTargetFPS(fps);


    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        show_fps ? DrawFPS(5, 5) : true;

        EndDrawing();
    }

    CloseWindow();
}
