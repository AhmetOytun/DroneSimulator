#ifdef _WIN32
#define localtime_r(a,b) localtime_s(b,a)
#endif
#include "simulator.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
Map map;
int numberofcells = 0;
List *survivors;
List *drones;
List *helpedsurvivors;
char filename[50];

/* creates a file with the current time */
void create_file(){
    /* opens a folder named reports */
    mkdir("reports", 0777);
    struct tm *timenow;

    time_t now = time(NULL);
    timenow = gmtime(&now);
    /* creates a file with the current time */
    strftime(filename, sizeof(filename), "./reports/report at %Y-%m-%d_%H:%M:%S.txt", timenow);
    FILE * f = fopen(filename, "w");
    if(f == NULL){
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, "FPS count: %d\n", FPS_COUNT);
    fprintf(f, "Drone speed: %f\n", DRONE_SPEED);
    fprintf(f, "Survivor generation speed: %f\n", SURVIVOR_GENERATION_SPEED);
    fprintf(f, "Drone count: %d\n", DRONE_COUNT);
    fprintf(f, "Map height: %d\n", MAP_HEIGHT);
    fprintf(f, "Map width: %d\n", MAP_WIDTH);
    fprintf(f, "Cell size: %d\n", CSIZE);
    fprintf(f, "Max survivor per cell: %d\n", MAX_SURVIVOR_PER_CELL);
    fprintf(f, "Max helped survivors list size: %d\n\n", MAX_HELPED_SURVIVORS_LIST_SIZE);
    fclose(f);
}

/* initializes the map */
void init_map(int height, int width) {
    /* create drone list */
    drones = create_list(sizeof(Drone), DRONE_COUNT);

    /* create reports directory */
    create_file();

    /* initialize map */
    map.height = height;
    map.width = width;
    numberofcells = height * width;

    /* create survivor list */
    survivors = create_list(sizeof(Survivor), numberofcells * MAX_SURVIVOR_PER_CELL);

    /* create helped survivors list */
    helpedsurvivors = create_list(sizeof(Survivor), MAX_HELPED_SURVIVORS_LIST_SIZE);

    /* allocate memory for the map cells */
    map.cells = malloc(sizeof(MapCell *) * map.height);

    for (int j = 0; j < map.height; j++) {
        /*rows for each pointer*/
        map.cells[j] = malloc(sizeof(MapCell) * map.width);
    }

    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            map.cells[i][j].coord.x = i;
            map.cells[i][j].coord.y = j; /**/
            map.cells[i][j].survivors = create_list(sizeof(Survivor), MAX_SURVIVOR_PER_CELL);
        }
    }
}

/* frees the map */
void freemap() {
    /* frees the map */
    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            List *list = map.cells[i][j].survivors;
            list->destroy(list);
        }
        free(map.cells[i]);
    }
    free(map.cells);
}

/* creates a survivor */
Survivor *create_survivor(Coord *coord, char *info, struct tm *discovery_time) {
    Survivor *s = malloc(sizeof(Survivor));

    memset(s, 0, sizeof(Survivor));
    memcpy(&(s->discovery_time), discovery_time, sizeof(struct tm));
    strncpy(s->info, info, sizeof(s->info));
    memcpy(&(s->coord), coord, sizeof(Coord));

    return s;
}

/* generates survivors */
void *survivor_generator(void *args) {
    if(SURVIVOR_GENERATION_SPEED == 0){
        return NULL;
    }
    srand(time(NULL));
    double waittime = 1.0 / SURVIVOR_GENERATION_SPEED;
    while(is_running){
            if (map.cells != NULL) {
                time_t traw;
                struct tm t; /*used for localtime*/

                /*survivor info*/
                char info[5] = {'A' + (rand() % 26), 
                                'A' + (rand() % 26),
                                '0' + (rand() % 9),
                                '0' + (rand() % 9)}; 

                Coord coord = {rand() % map.height, rand() % map.width};

                time(&traw);
                localtime_r(&traw, &t);

                Survivor *s = create_survivor(&coord, info, &t);
                List *list = map.cells[coord.x][coord.y].survivors;

                pthread_mutex_lock(&mutex);

                add(survivors, s);
                add(list, s);

                pthread_mutex_unlock(&mutex);
            }
            WaitTime(waittime);
    }
    return NULL;
}

/* creates a drone */
Drone *create_drone(Coord *coord, char *info, struct tm *stime) { 
    Drone *d = malloc(sizeof(Drone));

    memset(d, 0, sizeof(Drone));
    memcpy(&(d->stime), stime, sizeof(struct tm));
    strncpy(d->info, info, sizeof(d->info));
    memcpy(&(d->coord), coord, sizeof(Coord));

    return d;
}

/* gets the current time and return it as a char* */
char* get_current_time(){
    time_t traw;
    struct tm t; /*used for localtime*/
    time(&traw);
    localtime_r(&traw, &t);

    char *time = asctime(&t);
    time[strlen(time) - 1] = '\0'; /*remove \n*/
    return time;
};

/* gets the avarage help time for each survivor */
double get_avarage_help_time(){
    double sum = 0;
    Node *node = helpedsurvivors->head;
    for(int i = 0; i < helpedsurvivors->number_of_elements; i++){
        Survivor *s = (Survivor *)node->data;
        sum += difftime(mktime(&s->helped_time), mktime(&s->discovery_time));
        node = node->next;
    }
    
    return sum / (double)helpedsurvivors->number_of_elements;
}

/* reports the number of survivors helped and the avarage help time for each survivor */
void report(){
    FILE *f = fopen(filename, "a");
    if(f == NULL){
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, "Report at: %s\n", get_current_time());
    fprintf(f, "Number of survivors helped: %d\n", helpedsurvivors->number_of_elements);
    fprintf(f, "Avarage help time for each survivor: %f\n\n", get_avarage_help_time());
    fclose(f);
};

/* cleans the helped survivors list */
void clean_helped_survivors(){
    helpedsurvivors->destroy(helpedsurvivors);
    helpedsurvivors = create_list(sizeof(Survivor), MAX_HELPED_SURVIVORS_LIST_SIZE);
}

/* helps the survivor */
void help_survivor(Drone *d, Survivor s) {
    Survivor *saved_survivor = NULL;

    time_t traw;
    struct tm t; /*used for localtime*/
    time(&traw);
    localtime_r(&traw, &t);

    saved_survivor = pop(map.cells[d->coord.y][d->coord.x].survivors, &s);
    saved_survivor->helped_time = t;

    pthread_mutex_lock(&mutex);

    removenode(survivors, d->target_survivor);
    add(helpedsurvivors, saved_survivor);

    if(helpedsurvivors->number_of_elements == MAX_HELPED_SURVIVORS_LIST_SIZE){
        report();
        clean_helped_survivors();
        //printf("Helped survivors list is full, cleaning...\n");
    }

    d->numberofhelped++;
    d->status = 0;

    pthread_mutex_unlock(&mutex);
}

/* moves the drone */
void move_drone(Drone *drone) {

    if(drone->coord.x < drone->destination.x){
        drone->coord.x++;
    }else if(drone->coord.x > drone->destination.x){
        drone->coord.x--;
    }
    
    if(drone->coord.y < drone->destination.y){
        drone->coord.y++;
    }else if(drone->coord.y > drone->destination.y){
        drone->coord.y--;
    }   
}

/* runs the drone */
void *drone_runner(void *drone) {
    if(DRONE_SPEED == 0){
        return NULL;
    }
    double waittime = 1.0 / DRONE_SPEED;
    Drone *d = drone;
    Survivor s = {0};
    while(is_running){       
            if(d->status == 1){
                if(d->coord.x == d->destination.x && d->coord.y == d->destination.y){
                    /* drone has reached the destination */
                    help_survivor(d, s);
                }else{
                    /*move drone*/
                    move_drone(d);
                }
            }
            WaitTime(waittime);
    }
    return NULL;
}

/* runs the drone controller */
void *drone_controller(void *args) {
    if(DRONE_COUNT == 0){
        return NULL;
    }
    double waittime = 1.0 / FPS_COUNT;
    /* if the program is running */
    while(is_running){
        
        /* if there are no drones exit */
        if(DRONE_COUNT == 0){
            break;
        }

        Node *drone_node = drones->head;
        Drone *dr = (Drone *)drone_node->data;

        int survivors_count = 0;

        /* for each drone */
        for (int i = 0; i < DRONE_COUNT; i++) {
            /* if the drone is busy, skip */
            if(dr->status == 1){
                drone_node = drone_node->next;
                dr = (Drone *)drone_node->data;
                continue;
            }

            pthread_mutex_lock(&mutex);
            
            survivors_count = survivors->number_of_elements;

            if(survivors_count == 0){
                pthread_mutex_unlock(&mutex);
                break;
            }

            Node *survivor_node = survivors->tail;
            Survivor *s = (Survivor *)survivor_node->data;

            for(int j = 0; j < survivors_count; j++){
                /* if the survivor is already getting help, skip */
                if(s->status == 1){
                    survivor_node = survivor_node->prev;
                    s = (Survivor *)survivor_node->data;
                    continue;
                }

                dr->target_survivor = survivor_node;

                dr->destination.x = s->coord.y;
                dr->destination.y = s->coord.x;

                dr->status = 1;
                s->status = 1;

                break;
            }

            pthread_mutex_unlock(&mutex);
        }
        WaitTime(waittime);
    }
    return NULL;
}

/* creates drones with threads */
void create_drones(pthread_t *drone){
    for (int i = 0; i < DRONE_COUNT; i++) {
        Coord coord = {-1, -1};

        time_t traw;
        struct tm t; /*used for localtime*/

        time(&traw);
        localtime_r(&traw, &t);

        char info[30];
        sprintf(info, "Drone %d", i+1);
        Drone *d = create_drone(&coord, info, &t);
        drones->add(drones, d);
        Node *node = drones->head;
        Drone *dr = (Drone *)node->data;
        pthread_create(&drone[i], NULL, drone_runner,dr);// remember to check if its correct within the thread
    }
}

/* waits for the threads to finish */
void join_threads(pthread_t *drone, pthread_t survivor_thread, pthread_t controller_thread){
    for (int i = 0; i < DRONE_COUNT; i++) {
        pthread_join(drone[i], NULL);
    }
    pthread_join(survivor_thread, NULL);
    pthread_join(controller_thread, NULL);
}