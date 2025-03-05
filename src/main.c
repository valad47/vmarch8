#include "raylib.h"
#include "raymath.h"
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
    bool visible;
    void* data;
    int speed;
    void (*draw)(struct Object* obj);
} Object;

typedef struct TextLabel {
    char *str;
    float size;
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

void text_draw(Object *obj);
void rain(Object *obj);
void center_pulse(Object *obj);

Font load_font(float fontSize);

int main(int argc, char **argv) {
    args(argc, argv);

    srand(time(NULL));

    SetTraceLogLevel(debug_mode ? LOG_ALL : LOG_NONE);

    InitWindow(800, 400, "vmarch8");
    SetTargetFPS(fps);

    Font font = load_font(32);

    Array *objects = new_array(Object);
    for(int i = 0; i < 42; i++)
    insert(objects, &(Object){
        .position = {350, 200},
        .data = &(TextLabel) {
            "Я тебе люблю <3",
            32,
            font,
            PINK,
        },
        .draw = rain,
        .speed = 1+(rand() % 4),
        .visible = true
    });
    insert(objects, &(Object){
        .position = {350, 100},
        .data = &(TextLabel) {
            "Чингисхан наступає з сєвєра",
            32,
            font,
            RED
        },
        .draw = center_pulse,
        .visible = true
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

void text_draw(Object *obj) {
    if(!obj->visible) return;
    TextLabel *tl = (TextLabel*)obj->data;
    DrawTextEx(tl->font, tl->str, obj->position, tl->size, 1, tl->color);
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

void center_pulse(Object *obj) {
    TextLabel *tl = (TextLabel*)obj->data;
    if(tl->resize && (tl->size+=.3)>=42)
        tl->resize = false;
    else if(!tl->resize && (tl->size-=.3)<=32)
    tl->resize = true;

    obj->position = Vector2Subtract((Vector2){400, 200}, Vector2Scale(MeasureTextEx(tl->font, tl->str, tl->size, 1), .5));

    text_draw(obj);
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

Font load_font(float fontSize) {
    int codepoints[512] = { 0 };
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;   // Basic ASCII characters
    for (int i = 0; i < 255; i++) codepoints[96 + i] = 0x400 + i;   // Cyrillic characters
    const unsigned char font_raw[] = {
        #embed "../resources/Bebas_Neue_Cyrillic.ttf"
    };
    return LoadFontFromMemory(".ttf", font_raw, sizeof(font_raw), fontSize, codepoints, 512);
}
