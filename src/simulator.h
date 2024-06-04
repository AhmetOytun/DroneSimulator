#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "list.h"
#include "drone.h"
#include "background.h"

extern char filename[];
extern int CSIZE;/*cell size in raylib-grid*/
extern int MAP_HEIGHT; /*height of the map*/
extern int MAP_WIDTH; /*width of the map*/
extern int MAX_SURVIVOR_PER_CELL;/*max number of survivors in a cell*/
extern int MAX_HELPED_SURVIVORS_LIST_SIZE; /*max number of helped survivors*/
extern int FPS_COUNT;
extern int DRONE_COUNT;
extern double SURVIVOR_GENERATION_SPEED;
extern double DRONE_SPEED;
extern bool is_running;
extern Texture2D drone_texture;
extern Image drone_img;

/*type for a coordinate*/
typedef struct coord {
    int x;
    int y;
} Coord;

/*type for a survivor*/
typedef struct survivor {
    int status;
    Coord coord;
    struct tm discovery_time; /*you can use struct tm, too*/
    struct tm helped_time;
    char info[25];
} Survivor;

/*type for a cell in the map*/
typedef struct mapcell {
    Coord coord;       /*in our program simply i,j*/
    List *survivors; /*the survivors waiting help in this cell*/
} MapCell;

/*2d map type*/
typedef struct map {
    int height; /*height of the map*/
    int width;  /*width of the map*/
    MapCell **cells; /*pointer to 2d MapCell*/
} Map;


/*type for a drone*/
typedef struct drone {
    int status;
    int numberofhelped;
    Coord destination;
    Coord coord;
    Node *target_survivor;
    struct tm stime;
    char info[30];
    /*you can add more fields*/
} Drone;

/*lists*/
extern List *survivors;        
extern List *helpedsurvivors;
extern List *drones; 
extern Map map;

/*model.c*/
extern void init_map(int height, int width);
extern void freemap();
extern Drone *create_drone(Coord *coord, char *info, struct tm *stime);
extern Survivor *create_survivor(Coord *coord, char *info, struct tm *discovery_time);
extern void *survivor_generator(void *args);
extern void *drone_controller(void *args); 
extern void *drone_runner(void *drone);
extern void create_drones(pthread_t *drone);
extern void join_threads(pthread_t *drone, pthread_t survivor_thread, pthread_t controller_thread);
extern void create_file();
extern void init_map(int height, int width);
extern char* get_current_time();
extern double get_avarage_help_time();
extern void report();
extern void clean_helped_survivors();
extern void help_survivor(Drone *d, Survivor s);
extern void move_drone(Drone *drone);
extern void join_threads(pthread_t *drone, pthread_t survivor_thread, pthread_t controller_thread);

/*view.c*/
extern int windowheight;
extern int windowwidth;
extern void init_window(Map map);
extern void draw_rectangles();
extern void draw_drones();
extern void draw_map();

/* config.c */
extern int config_screen();
extern void input_box(Rectangle letterbox, char letter[], int *lettercount, int maxlettercount);
extern void load_drone_img();
extern void view_texts();
extern void init_input_boxes();
extern void calculate_values_from_input_boxes();
extern void enter_button();
extern void draw_background();
extern void load_font();
extern void set_icon();
extern void load_bg();
extern void draw_title();