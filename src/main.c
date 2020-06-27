#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

float fov_factor = 120;

bool is_running = false;

void setup(void)
{
    // Allocate the required bytes in memory for the color buffer
    color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

    // Creating a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        window_width,
        window_height);

    int point_count = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3_t new_point = {.x = x, .y = y, .z = z};
                cube_points[point_count++] = new_point;
            }
        }
    }
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}

vec2_t project(vec3_t point) // translates 3D view to 2D, don't we have all 2D screens? ;)
{
    vec2_t projected_point = {
        .x = (fov_factor * point.x),
        .y = (fov_factor * point.y),
    };

    return projected_point;
}

void update(void)
{
    for (int i = 0; i < N_POINTS; i++)
    {
        vec3_t point = cube_points[i];

        vec2_t projected_point = project(point);

        // we need to save 2d vector in an array for projected points
        projected_points[i] = projected_point;
    }
}

void render(void)
{
    // Loop all projected points
    for (int i = 0; i < N_POINTS; i++)
    {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + (window_width / 2),
            projected_point.y + (window_height / 2),
            4,
            4,
            0xFFFFFF00
        );
    }

    render_color_buffer();
    clear_color_buffer(0xFF000000);

    SDL_RenderPresent(renderer);
}

int main(int argc, char *args[])
{
    is_running = initialize_window();

    setup();

    vec3_t myvec = {2.0, 3.0, -4.0};

    while (is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}