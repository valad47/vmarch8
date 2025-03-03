#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

bool debug_mode = false;
bool show_fps = false;
int fps = 0;

#define TEXT_DEFAULT "\033[39m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_LIGHTBLUE "\033[94m"

typedef struct Object {
    Vector2 position;
    void* data;
    void (*draw)(struct Object* obj);
} Object;

typedef struct TextLabel {
    char *str;
    int size;
    Font font;
    Color color;
    bool resize;
} TextLabel;

void args(int argc, char ** argv);

void text_draw(Object *obj) {
    TextLabel *tl = (TextLabel*)obj->data;
    DrawTextEx(tl->font, tl->str, obj->position, tl->size, 0, tl->color);
}

void pulse(Object *obj) {
    TextLabel *tl = (TextLabel*)obj->data;
    if(tl->resize && (tl->size++) == 30) {
        tl->resize = false;
    } else if(!tl->resize && (tl->size--) == 20) {
        tl->resize = true;
    }
    text_draw(obj);
}

int main(int argc, char **argv) {
    args(argc, argv);

    SetTraceLogLevel(debug_mode ? LOG_ALL : LOG_NONE);

    InitWindow(800, 400, "vmarch8");
    SetTargetFPS(fps);
    Font font = LoadFont("resources/dejavu.fnt");

    Object objects[] = {
        {
            .position = {350, 200},
            .data = &(TextLabel) {
                "Я тебе люблю <3",
                20,
                font,
                RED
            },
            .draw = text_draw
        },
        {
            .position = {350, 100},
            .data = &(TextLabel) {
                "Чингискан наступає з сєвєра",
                20,
                font,
                RED
            },
            .draw = pulse
        }
    };

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        show_fps ? DrawFPS(5, 5) : true;

        for(int i = 0; i < sizeof(objects) / sizeof(Object); i++) {
            objects[i].draw(&objects[i]);
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseWindow();
}

void args(int argc, char **argv) {
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
            exit(0);
        }
        printf("[" TEXT_YELLOW "WARNING" TEXT_DEFAULT "] Got unexpected argument " TEXT_LIGHTBLUE "%s" TEXT_DEFAULT "\n", argv[i]);
    }
}
