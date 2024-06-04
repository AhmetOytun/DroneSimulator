#include "simulator.h"

/* initializes window */
void init_window(Map map) {
    /* window height is (cell size) x (map height)*/
    windowheight = map.height * CSIZE;

    /* window width is (cell size) x (map width)*/
    windowwidth = map.width * CSIZE;

    InitWindow(windowwidth, windowheight, "Drone simulator by BOMBA STUDIOS™");

    SetTargetFPS(FPS_COUNT);

    /* loads drone texture */
    drone_texture = LoadTextureFromImage(drone_img);
}

/* draws survivors as rects */
void draw_rectangles() {
    for(int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            if (map.cells[i][j].survivors != NULL &&
                map.cells[i][j].survivors->number_of_elements != 0) {
				int r = map.cells[i][j].survivors->number_of_elements;
                r = (r%15);
                /* changes color to a different shade of red according to size */
                DrawRectangle(j * CSIZE, i * CSIZE, CSIZE, CSIZE, (Color){250-(r*20), 0, 0, 255});
            }
        }
    }
}

/* draws drones */
void draw_drones() {
    Node *node = drones->head;
    Drone *d = NULL;
    for (int i = 0; i < DRONE_COUNT; i++) {
        d = (Drone *)node->data;
        /* resizes and draws drone texture as cell size */
        DrawTexturePro(drone_texture, (Rectangle){0, 0, drone_texture.width, drone_texture.height},
                       (Rectangle){d->coord.x * CSIZE, d->coord.y * CSIZE, CSIZE, CSIZE}, (Vector2){0, 0}, 0, WHITE);
        node = node->next;
    }
}

/* draws map */
void draw_map() {
    /* draw rects and drones */
    draw_rectangles();
    draw_drones();

    /* makes backgorund white */
    ClearBackground(RAYWHITE);

    for(int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            DrawRectangleLines(j * CSIZE, i * CSIZE, CSIZE, CSIZE, BLACK); /* draws the grid */
        }
    }
}
