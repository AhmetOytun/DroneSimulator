#ifdef _WIN32
#define localtime_r(a,b) localtime_s(b,a)
#endif
#include "simulator.h"

int main(int argc, char* argv[]) {
    /* check if the configuration is correct */
    if(config_screen() == 1){
        /* fps, mapwidth, mapheight or csize == 0 */
        return 1;
    }

    /* threads */
    pthread_t drone[DRONE_COUNT];
    pthread_t survivor_thread;
    pthread_t controller_thread;

    /*initializes map*/
    init_map(MAP_HEIGHT, MAP_WIDTH);

    /*initializes window*/
    init_window(map);

    /* program is running */
    is_running = true;

    /* create drones with threads */
    create_drones(drone);

    /* creates survivor thread */
    pthread_create(&survivor_thread, NULL, survivor_generator, NULL);

    /* creates controller thread */
    pthread_create(&controller_thread, NULL, drone_controller, NULL);

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        /* update model:survivors, drones etc. */

        /*draws new updated map*/
        draw_map();

        EndDrawing();
    }

    /* closing... */
    is_running = false;

    /* wait for threads to finish */
    join_threads(drone, survivor_thread, controller_thread);

    /*free the map*/
    freemap();

    /* unload textures */
    UnloadTexture(drone_texture);

    /* free lists */
    destroy(drones);
    destroy(survivors);
    destroy(helpedsurvivors);

    CloseWindow();

    return 0;
}