#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

bool debug_mode = false;
bool show_fps = false;
int fps = 60;

#define TEXT_DEFAULT "\033[39m"
#define TEXT_YELLOW "\033[33m"
#define TEXT_LIGHTBLUE "\033[94m"

typedef struct Object {
    Vector2 position;
    void* data;
    int speed;
    void (*draw)(struct Object* obj);
} Object;

typedef struct TextLabel {
    char *str;
    int size;
    Font font;
    Color color;
    bool resize;
} TextLabel;

typedef struct Array {
    void *array;
    size_t size;
    size_t obj_size;
    size_t length;
} Array;

Array *__new_array(size_t data_size) {
    Array *array_s = malloc(sizeof(Array));
    array_s->array = malloc(data_size * 16);
    array_s->obj_size = data_size;
    array_s->size = 16;
    array_s->length = 0;

    return array_s;
}

#define new_array(data_type) __new_array(sizeof(data_type))
void insert(Array* array, void* data);

#define debug(...) if(debug_mode) printf(__VA_ARGS__)

void args(int argc, char ** argv);

void text_draw(Object *obj) {
    TextLabel *tl = (TextLabel*)obj->data;
    DrawTextEx(tl->font, tl->str, obj->position, tl->size, 0, tl->color);
}

void rain(Object *obj) {
    TextLabel *tl = (TextLabel*)obj->data;
    if((obj->position.y += obj->speed) >= GetRenderHeight()) {
        obj->position.y = -40;
        obj->position.x = rand() % 800;
        obj->speed = (rand() % 4) + 1;
    };

    text_draw(obj);
}

int main(int argc, char **argv) {
    args(argc, argv);

    srand(time(NULL));

    SetTraceLogLevel(debug_mode ? LOG_ALL : LOG_NONE);

    InitWindow(800, 400, "vmarch8");
    SetTargetFPS(fps);
    Font font = LoadFont("resources/dejavu.fnt");

    Array *objects = new_array(Object);
    for(int i = 0; i < 64; i++)
    insert(objects, &(Object){
        .position = {350, 200},
        .data = &(TextLabel) {
            "Я тебе люблю <3",
            20,
            font,
            RED,
        },
        .draw = rain,
        .speed = 1+(rand() % 4)
    });
    insert(objects, &(Object){
        .position = {350, 100},
        .data = &(TextLabel) {
            "Чингискан наступає з сєвєра",
            20,
            font,
            RED
        },
        .draw = rain
    });

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        show_fps ? DrawFPS(5, 5) : true;


        for(int i = 0; i < objects->length; i++) {
            ((Object*)objects->array)[i].draw((Object*)objects->array + i);
        }

        EndDrawing();
    }

    free(objects->array);
    free(objects);

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
void insert(Array* array, void* data) {
    if(array->length == array->size) {
        array->size *= 2;
        array->array = realloc(array->array, array->obj_size * array->size);

        debug("["TEXT_LIGHTBLUE"%p"TEXT_DEFAULT"]: New size ["TEXT_YELLOW"%zu"TEXT_DEFAULT"]\n", array, array->obj_size * array->size);
    }
    memcpy((array->array + ((array->length) * array->obj_size)), data, array->obj_size);
    array->length++;
    debug("[" TEXT_LIGHTBLUE "%p" TEXT_DEFAULT "]: New object [" TEXT_YELLOW "%p" TEXT_DEFAULT "]\n", array, data);
}
