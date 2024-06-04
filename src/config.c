#include "simulator.h"

/* needs to be included here */
#include "comic.h"

/* window width and height */
int windowheight = 0;
int windowwidth = 0;

/* font, background and drone image(they are getting decompressed) */
Font font;
Texture2D background;
Image drone_img;

/* input boxes  */
Rectangle fpsbox = {80, 60, 50, 20};
Rectangle droneCountBox = {145, 100, 50, 20};
Rectangle droneSpeedBox = {145, 140, 50, 20};
Rectangle survivorGenerationSpeedBox = {250, 180, 50, 20};
Rectangle mapHeightBox = {135, 220, 50, 20};
Rectangle mapWidthBox = {135, 260, 50, 20};
Rectangle cellSizeBox = {115, 300, 50, 20};
Rectangle maxSurvivorPerCellBox = {210, 340, 70, 20};
Rectangle maxHelpedSurvivorsListSizeBox = {280, 380, 70, 20};
Rectangle enterButton = {200, 420, 100, 50};

/* condition  */
bool doneButtonPressed = false;

/* input letters */
char FPS[4] = "\0";
char droneCount[4] = "\0";
char droneSpeed[4] = "\0";
char survivorGenerationSpeed[4] = "\0";
char mapHeight[4] = "\0";
char mapWidth[4] = "\0";
char cellSize[4] = "\0";
char maxSurvivorPerCell[6] = "\0";
char maxHelpedSurvivorsListSize[6] = "\0";

/* values */
int CSIZE;
int MAP_HEIGHT;
int MAP_WIDTH;
int MAX_SURVIVOR_PER_CELL;
int MAX_HELPED_SURVIVORS_LIST_SIZE;
int FPS_COUNT;
int DRONE_COUNT;
double SURVIVOR_GENERATION_SPEED;
double DRONE_SPEED;
bool is_running;
Texture2D drone_texture;

/* letter counts */
int fpslettercount = 0;
int dronelettercount = 0;
int dronespeedlettercount = 0;
int survivorGenerationSpeedlettercount = 0;
int mapHeightlettercount = 0;
int mapWidthlettercount = 0;
int cellSizelettercount = 0;
int maxSurvivorPerCelllettercount = 0;
int maxHelpedSurvivorsListSizelettercount = 0;

void input_box(Rectangle letterbox, char letter[], int *lettercount, int maxlettercount){
    if (CheckCollisionPointRec(GetMousePosition(), letterbox)) {
        int key = GetCharPressed();
        while (key > 0) {
                if ((key >= 48) && (key <= 57) && (*lettercount < maxlettercount)){
                    letter[*lettercount] = (char)key;
                    letter[*lettercount+1] = '\0';
                    (*lettercount)++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                (*lettercount)--;
                if (*lettercount < 0){
                    *lettercount = 0;
                }
                letter[*lettercount] = '\0';
            }
    }
}

void load_drone_img(){
    /* loads drone image */
    drone_img.format = DRONE_FORMAT;
    drone_img.height = DRONE_HEIGHT;
    drone_img.width = DRONE_WIDTH;
    drone_img.data = DRONE_DATA;
    drone_img.mipmaps = 1;
}

void view_texts(){
        /* writing these texts without a function is much more simpler */
        DrawTextEx(font,"FPS:", (Vector2){40, 60}, 20,1, BLACK);
        DrawTextEx(font,FPS, (Vector2){93, 60}, 20,1, BLACK);
        DrawRectangleLines(80, 60, 50, 20, BLACK);

        DrawTextEx(font,"Drone Count:", (Vector2){40, 100}, 20,1, BLACK);
        DrawTextEx(font,droneCount, (Vector2){160, 100}, 20,1, BLACK);
        DrawRectangleLines(145, 100, 50, 20, BLACK);

        DrawTextEx(font,"Drone Speed:", (Vector2){40, 140}, 20,1, BLACK);
        DrawTextEx(font,droneSpeed, (Vector2){160, 140}, 20,1, BLACK);
        DrawRectangleLines(145, 140, 50, 20, BLACK);

        DrawTextEx(font,"Survivor Generation Speed:", (Vector2){40, 180}, 20,1, BLACK);
        DrawTextEx(font,survivorGenerationSpeed, (Vector2){263, 180}, 20,1, BLACK);
        DrawRectangleLines(250, 180, 50, 20, BLACK);

        DrawTextEx(font,"Map Height:", (Vector2){40, 220}, 20,1, BLACK);
        DrawTextEx(font,mapHeight, (Vector2){150, 220}, 20,1, BLACK);
        DrawRectangleLines(135, 220, 50, 20, BLACK);

        DrawTextEx(font,"Map Width:", (Vector2){40, 260}, 20,1, BLACK);
        DrawTextEx(font,mapWidth, (Vector2){150, 260}, 20,1, BLACK);
        DrawRectangleLines(135, 260, 50, 20, BLACK);

        DrawTextEx(font,"Cell Size:", (Vector2){40, 300}, 20,1, BLACK);
        DrawTextEx(font,cellSize, (Vector2){130, 300}, 20,1, BLACK);
        DrawRectangleLines(115, 300, 50, 20, BLACK);

        DrawTextEx(font,"Max Survivor Per Cell:", (Vector2){40, 340}, 20,1, BLACK);
        DrawTextEx(font,maxSurvivorPerCell, (Vector2){223, 340}, 20,1, BLACK);
        DrawRectangleLines(210, 340, 70, 20, BLACK);

        DrawTextEx(font,"Max Helped Survivors List Size:", (Vector2){40, 380}, 20,1, BLACK);
        DrawTextEx(font,maxHelpedSurvivorsListSize, (Vector2){295, 380}, 20,1, BLACK);
        DrawRectangleLines(280, 380, 70, 20, BLACK);
}

void init_input_boxes(){
        /* making input boxes using input_box function */
        input_box(fpsbox, FPS, &fpslettercount,3);
        input_box(droneCountBox, droneCount, &dronelettercount,3);
        input_box(droneSpeedBox, droneSpeed, &dronespeedlettercount,3);
        input_box(survivorGenerationSpeedBox, survivorGenerationSpeed, &survivorGenerationSpeedlettercount,3);
        input_box(mapHeightBox, mapHeight, &mapHeightlettercount,3);
        input_box(mapWidthBox, mapWidth, &mapWidthlettercount,3);
        input_box(cellSizeBox, cellSize, &cellSizelettercount,3);
        input_box(maxSurvivorPerCellBox, maxSurvivorPerCell, &maxSurvivorPerCelllettercount,5);
        input_box(maxHelpedSurvivorsListSizeBox, maxHelpedSurvivorsListSize, &maxHelpedSurvivorsListSizelettercount,5);
}

/* gets values */
void calculate_values_from_input_boxes(){
    if (fpslettercount > 0){
        FPS_COUNT = atoi(FPS);
    }
    if (dronelettercount > 0){
        DRONE_COUNT = atoi(droneCount);
    }
    if (dronespeedlettercount > 0){
        DRONE_SPEED = atoi(droneSpeed);
    }
    if (survivorGenerationSpeedlettercount > 0){
        SURVIVOR_GENERATION_SPEED = atoi(survivorGenerationSpeed);
    }
    if (mapHeightlettercount > 0){
        MAP_HEIGHT = atoi(mapHeight);
    }
    if (mapWidthlettercount > 0){
        MAP_WIDTH = atoi(mapWidth);
    }
    if (cellSizelettercount > 0){
        CSIZE = atoi(cellSize);
    }
    if (maxSurvivorPerCelllettercount > 0){
        MAX_SURVIVOR_PER_CELL = atoi(maxSurvivorPerCell);
    }
    if (maxHelpedSurvivorsListSizelettercount > 0){
        MAX_HELPED_SURVIVORS_LIST_SIZE = atoi(maxHelpedSurvivorsListSize);
    }
}

/* check enter button */
void enter_button(){
    DrawRectangleLines(200, 420, 100, 50, BLACK);
    DrawTextEx(font,"Done", (Vector2){230, 435}, 20,1, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), enterButton)) {
        DrawRectangle(200, 420, 100, 50, GRAY);
        DrawTextEx(font,"Done", (Vector2){230, 435}, 20,1, BLACK);

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            doneButtonPressed = true;
        }

    }
}

/* draws background */
void draw_background(){
    DrawTextureEx(background, (Vector2){30, 0}, 0, 0.45, (Color){255,255,255,50});
}

/* loads font from compressed header file */
void load_font(){
    font = LoadFont_Comic();
}

/* sets executable icon from compressed header file(this doesn't work) */
void set_icon(){
    Image icon = {0};
    icon.format = DRONE_FORMAT;
    icon.height = DRONE_HEIGHT;
    icon.width = DRONE_WIDTH;
    icon.data = DRONE_DATA;
    icon.mipmaps = 1;

    SetWindowIcon(icon);
}

/* loads background from compressed header file */
void load_bg(){
    Image bg = {0};

    bg.format = BACKGROUND_FORMAT;
    bg.height = BACKGROUND_HEIGHT;
    bg.width = BACKGROUND_WIDTH;
    bg.data = BACKGROUND_DATA;
    bg.mipmaps = 1;

    background = LoadTextureFromImage(bg);
}

/* draws title */
void draw_title(){
    DrawTextEx(font,"Drone Simulator by BOMBA STUDIOS config screen", (Vector2){55, 15}, 20,1, BLACK);
}

/* config screen */
int config_screen() {
    SetTraceLogLevel(LOG_NONE);
    InitWindow(500, 500, "Drone simulator by BOMBA STUDIOS™ - config screen");
    SetTargetFPS(60);

    load_font();
    load_bg();
    set_icon();
    load_drone_img();

    while(true) {
        /* sets mouse cursor */
        if(CheckCollisionPointRec(GetMousePosition(), fpsbox) || CheckCollisionPointRec(GetMousePosition(), droneCountBox) || CheckCollisionPointRec(GetMousePosition(), droneSpeedBox) || CheckCollisionPointRec(GetMousePosition(), survivorGenerationSpeedBox) || CheckCollisionPointRec(GetMousePosition(), mapHeightBox) || CheckCollisionPointRec(GetMousePosition(), mapWidthBox) || CheckCollisionPointRec(GetMousePosition(), cellSizeBox) || CheckCollisionPointRec(GetMousePosition(), maxSurvivorPerCellBox) || CheckCollisionPointRec(GetMousePosition(), maxHelpedSurvivorsListSizeBox)) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        init_input_boxes();

        BeginDrawing();

        draw_title();

        draw_background();

        ClearBackground(RAYWHITE);

        view_texts();

        enter_button();

        EndDrawing();

        if(IsKeyPressed(KEY_ENTER) || doneButtonPressed){
            break;
        }

        if(IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()){
            CloseWindow();
            exit(0);
        }
    }

    calculate_values_from_input_boxes();
    CloseWindow();

    /* if any of these values are 0, return 1(we handle this in controller) */
    if(FPS_COUNT == 0 || MAP_HEIGHT == 0 || MAP_WIDTH == 0 || CSIZE == 0){
        return 1;
    }

    return 0;
}