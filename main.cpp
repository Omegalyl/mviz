#include "raylib.h"
#include <cstdio>
#include <cstdint>
#include <unistd.h>
#include <cassert>
#include <cstring>

#define ARRAY_LEN(x) sizeof(x) / sizeof(x[0])

typedef struct
{
    float l;
    float r;
} SteroFrame;

SteroFrame global_frames[44100];
size_t global_frames_count = 0;

void callback(void *bufferData, unsigned int frames)
{

    size_t capacity = ARRAY_LEN(global_frames);
    // space left for frames
    if ((capacity - global_frames_count) >= frames) {
        memcpy(global_frames + global_frames_count, bufferData, sizeof(SteroFrame)*frames);
        global_frames_count += frames;
    } else if (capacity >= frames)
    {
        memmove(global_frames, global_frames + frames, sizeof(SteroFrame)*(capacity - frames));
        memcpy(global_frames + (capacity - frames), bufferData, sizeof(SteroFrame)*frames);
    } else {
        memcpy(global_frames, bufferData, sizeof(SteroFrame)*capacity);
        global_frames_count = capacity;
    }
}

int main(void)
{
    InitWindow(800, 600, "mviz");
    SetTargetFPS(60);

    InitAudioDevice();
    Music music = LoadMusicStream("Pantera - Forever Tonight.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5f);

    assert(music.stream.channels == 2);
    assert(music.stream.sampleSize == 32);

    AttachAudioStreamProcessor(music.stream, callback);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(music);

        if (IsKeyPressed(KEY_SPACE))
        {
            if (IsMusicStreamPlaying(music))
            {
                PauseMusicStream(music);
            }
            else
            {
                ResumeMusicStream(music);
            }
        };

        int w = GetRenderWidth();
        int h = GetRenderHeight();

        BeginDrawing();
        ClearBackground(BLACK);
        float cell_width = (float)w / global_frames_count;
        for (size_t i = 0; i < global_frames_count; i++)
        {
            float sample = global_frames[i].l;
            if (sample > 0)
            {
                DrawRectangle(i * cell_width, h / 2 - h/2*sample, 1, h/2*sample, RED);
            }
            else
            {
                DrawRectangle(i * cell_width, h / 2, 1, h/2*sample, RED);
            }
        }
        EndDrawing();
    }

    return 0;
}