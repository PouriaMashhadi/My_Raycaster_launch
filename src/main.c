#include <raylib.h>
#include "map.h"
#include "player.h"
#include <math.h>
//Defining The Total Size of Window
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 650


int flag = 0 ; //it has three value I added Control Option
int nightmode=0;

//Introducing Walls locations
int world_map[W][H] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


int MapX(float x, float cellwidth) { return (int)(x / cellwidth); }
int MapY(float y, float cellheight) { return (int)(y / cellheight); }

int main(void)
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My BP Virtual Room Project ");
    SetTargetFPS(60);


    float cellwidth  = (float)WINDOW_WIDTH / W;
    float cellheight = (float)WINDOW_HEIGHT / H;

    
    float miniCell = 8.0f; //Define How big each cell in mini map is
    float map_startx = WINDOW_WIDTH - W * miniCell - 10;//The number  is for spliting frm edge
    float map_starty = 10;//It is also for spliting from upsidee of the edge

    Player player;
    player.pos   = (Vector2){ 1.5f * cellwidth, 1.5f * cellheight };
    player.dir   = (Vector2){ 1.0f, 0.0f };
    player.plane = (Vector2){ 0.0f, 0.66f };

    float moveSpeed = 150.0f;
    float rotSpeed  = 3.0f;

    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
    //Openning map by pressing M Key.
    if (IsKeyPressed(KEY_M))
    {
        if(flag==0) flag=1;
        else flag=0;
    }

       if (IsKeyPressed(KEY_C) && flag !=1)
    {
        if(flag==0) flag=2;
        else flag=0;
    }
    
    
    moveSpeed = 150.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) )
    {
      moveSpeed=250.0f;
    }
    


        if (flag==0)
        {
        MovePlayer(&player,dt,world_map,cellwidth,cellheight,moveSpeed,rotSpeed);
        }
        else if(flag == 1){//Loading and Saving the map only when in editing mode
            if (IsKeyPressed(KEY_S))
        {
            SaveWorldMap("map.bin", world_map);
        }

        if (IsKeyPressed(KEY_L))
        {
            LoadWorldMap("map.bin", world_map);
            player.pos   = (Vector2){ 1.5f * cellwidth, 1.5f * cellheight };
            player.dir   = (Vector2){ 1.0f, 0.0f };
            player.plane = (Vector2){ 0.0f, 0.66f };
        }

        }
        BeginDrawing();
        //At first we assume that every where is blue like we are in the middle of the sky 
        
        if(nightmode )ClearBackground(DARKBLUE);
        else ClearBackground(SKYBLUE);//Changing between night and light mode
        if(IsKeyPressed(KEY_T)&& MapX(player.pos.x,cellwidth)==1 && MapY(player.pos.y,cellheight)==1){
            if(nightmode==1)nightmode=0;
            else nightmode=1;
        }


        // We have to draw a ground like it starts from 0 th half of the height with full width
        if(nightmode)DrawRectangle(0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2, DARKGREEN);
        else DrawRectangle(0, WINDOW_HEIGHT/2, WINDOW_WIDTH, WINDOW_HEIGHT/2, GREEN);
        // ************** Raycasting 3D **************

        for (int x = 0; x < WINDOW_WIDTH; x++) {//I have to send one ray for each column 
            double cameraX = 2 * x / (double)WINDOW_WIDTH - 1;//camera space [-1, +1] and it shows the distance by center of view
            double rayDirX = player.dir.x + player.plane.x * cameraX;
            double rayDirY = player.dir.y + player.plane.y * cameraX;

            //How far the ray must travel to cross one grid line in X or Y long(shallow) short(steep)
            double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX); 
            double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);

        //Remember we have a float location now we have to modify it to a seprate value and put it as a cell index The function below will make it for us
            int mapX = (int)(player.pos.x / cellwidth);
            int mapY = (int)(player.pos.y / cellheight);

            double sideDistX, sideDistY; //distance to the next vertical/horizontal grid line
            int stepX, stepY;
            if (rayDirX < 0) {
                stepX = -1;
                sideDistX = (player.pos.x / cellwidth - mapX) * deltaDistX;
            } else {
                stepX = 1;
                sideDistX = (mapX + 1.0 - player.pos.x / cellwidth) * deltaDistX;
            }

            if (rayDirY < 0) {
                stepY = -1;
                sideDistY = (player.pos.y / cellheight - mapY) * deltaDistY;
            } else {
                stepY = 1;
                sideDistY = (mapY + 1.0 - player.pos.y / cellheight) * deltaDistY;
            }
            //DDA Loop
            int hit = 0;
            int side = 0;
            while (!hit) {
                if (sideDistX < sideDistY) {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                } else {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                if (world_map[mapX][mapY] > 0) hit = 1;
            }

            double perpWallDist = (side == 0)//avoid from fish eye mode
                ? (sideDistX - deltaDistX)
                : (sideDistY - deltaDistY);

            int lineHeight = (int)(WINDOW_HEIGHT / perpWallDist);

            

            int drawStart = -lineHeight / 2 + WINDOW_HEIGHT / 2;
            if (drawStart < 0) drawStart = 0;
            int drawEnd = lineHeight / 2 + WINDOW_HEIGHT / 2;
            if (drawEnd >= WINDOW_HEIGHT) drawEnd = WINDOW_HEIGHT - 1;

            if(nightmode) DrawLine(x, drawStart, x, drawEnd, BLACK);
            else DrawLine(x, drawStart, x, drawEnd, DARKGRAY);
        }
        // ************** MiniMap **************
        //Showing full map size by pressing M
            if(flag==1){
                for (int x = 0; x < W; x++) {
                    for (int y = 0; y < H; y++) {
                        Color color; 
                        if (x==1&&y==1)
                            {
                                color= YELLOW;
                                DrawRectangle(x*cellwidth, y*cellheight, cellwidth, cellheight, color);
                                 DrawText("BED",x*cellwidth,y*cellheight+5 , 30, DARKPURPLE);
                                continue;
                            }
                        color = world_map[x][y] ? DARKGRAY : LIGHTGRAY;
                        DrawRectangle(x*cellwidth, y*cellheight, cellwidth, cellheight, color);
                        DrawRectangleLines(x*cellwidth, y*cellheight, cellwidth, cellheight, GRAY);
                    }
        }
        DrawCircleV(player.pos, cellwidth/5, RED); 
        DrawLine(player.pos.x, player.pos.y,player.pos.x + player.dir.x*cellwidth,player.pos.y + player.dir.y*cellheight,BLUE); 
        
        Vector2 mousePos = GetMousePosition();
        int mapX = MapX(mousePos.x, cellwidth);
        int mapY = MapY(mousePos.y, cellheight);

        // Get player's current map cell
        int playerXmap = MapX(player.pos.x, cellwidth);
        int playerYmap = MapY(player.pos.y, cellheight);

    
        // Make sure we don't go outside map bounds
        if(mapX >= 0 && mapX < W && mapY >= 0 && mapY < H) {
                // Left click = add wall if empty
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && world_map[mapX][mapY] == 0 &&( mapX!=playerXmap || mapY!=playerYmap)&&( mapX!=1 || mapY!=1)) {
                    world_map[mapX][mapY] = 1;
                }
                // Right click = remove wall if exists
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && world_map[mapX][mapY] == 1 && (mapX!=0 && mapX!=15)  && (mapY!=0 && mapY!=15)) {
                    world_map[mapX][mapY] = 0;
                }
        }

        DrawText("ADD Wall : Click", 20, 10, 25, RED);
        DrawText("Remove Wall : Right_Click", WINDOW_WIDTH/2+180, 10, 25, RED);
        DrawText("MAP MODE", WINDOW_WIDTH/2-75, WINDOW_HEIGHT-40, 35, ORANGE);
         DrawText("Back (C) ",20,WINDOW_HEIGHT-40, 25, ORANGE);
        }
            
        else if(flag==0){
        for (int x = 0; x < W; x++) {//for creating mini map above right
            for (int y = 0; y < H; y++) {
               Color color;
               if (x==1&&y==1)
               {
                color= YELLOW;
                DrawRectangle(map_startx + x * miniCell,
                              map_starty + y * miniCell,
                              miniCell, miniCell, color);
                continue;
               }
               
                if(nightmode) {color = world_map[x][y] ? RED : LIGHTGRAY;}
               else { color = world_map[x][y] ? BLUE : LIGHTGRAY;}
                DrawRectangle(map_startx + x * miniCell,
                              map_starty + y * miniCell,
                              miniCell, miniCell, color);
            }
        }
        DrawText("Your fps is set as 60 !", 20, 10, 15, MAGENTA);
        DrawText("switching night/day mode press T while your on bed(yellow loc on map)! ", 20, 40, 15, RED);
        DrawText("press shift for sprint  ", 20, 70, 15, RED);
        DrawText("Press M to open map!",map_startx, map_starty+H*miniCell+5, 10, RED);
        DrawText("CONTROL OPTIONS (C) ",20,WINDOW_HEIGHT-40, 25, BLACK);
        DrawText("Created by Pouria_Mashhadi",WINDOW_WIDTH-220,WINDOW_HEIGHT-20, 15, BLACK);

        if (MapX(player.pos.x,cellwidth)==1 && MapY(player.pos.y,cellheight)==1)
        {
            DrawText("Press T to change theme !", WINDOW_WIDTH/2-100, WINDOW_HEIGHT-100, 40, DARKPURPLE);
        }
        // player on mini map
        if(nightmode){DrawCircle(map_startx + player.pos.x / cellwidth * miniCell,
                   map_starty + player.pos.y / cellheight * miniCell,
                   miniCell / 2, BLUE);}
        else{DrawCircle(map_startx + player.pos.x / cellwidth * miniCell,
                   map_starty + player.pos.y / cellheight * miniCell,
                   miniCell / 2, RED);}

        // player direction on mini
        DrawLine(map_startx + player.pos.x / cellwidth * miniCell,
                 map_starty + player.pos.y / cellheight * miniCell,
                 map_startx + (player.pos.x + player.dir.x * cellwidth / 2) / cellwidth * miniCell,
                 map_starty + (player.pos.y + player.dir.y * cellheight / 2) / cellheight * miniCell,
                 DARKPURPLE);
        }
        else{
            DrawText("Moving : W/A/S/D ",20, 20, 25, RED);
            DrawText("Changing Dir : Arrows  ",20, 60, 25, RED);
            DrawText("map : M",20,100, 25, RED);
            DrawText("SPRINT : SHIFT",20,140, 25, RED);
            DrawText("Changing night/Day mode: go to bed location + T",20, 180, 25, RED);
            DrawText("Adding Wall : Open map + Click",20, 220, 25, RED);
            DrawText("Removing Wall : open map + Right_CLick",20, 260, 25, RED);
            DrawText("Saving map sheet : open map + S",20,300, 25, RED);
            DrawText("Loading map sheet : open map + L",20, 340, 25, RED);
            DrawText("Back (C) ",20,WINDOW_HEIGHT-40, 25, BLACK);
            DrawText("Github : PouriaMashhadi",WINDOW_WIDTH-300,WINDOW_HEIGHT-40, 25, BLACK);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
