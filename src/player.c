#include "player.h"
#include <math.h>
#include <raylib.h>

void MovePlayer(Player *player, float dt, int world_map[16][16], float cellwidth, float cellheight, float moveSpeed, float rotSpeed)
{
    float dx = 0, dy = 0;
    if (IsKeyDown(KEY_W)) { dx += player->dir.x * moveSpeed * dt; dy += player->dir.y * moveSpeed * dt; }
    if (IsKeyDown(KEY_S)) { dx -= player->dir.x * moveSpeed * dt; dy -= player->dir.y * moveSpeed * dt; }
    if (IsKeyDown(KEY_A)) { dx += player->dir.y * moveSpeed * dt; dy -= player->dir.x * moveSpeed * dt; }
    if (IsKeyDown(KEY_D)) { dx -= player->dir.y * moveSpeed * dt; dy += player->dir.x * moveSpeed * dt; }

    // Rotation
    if (IsKeyDown(KEY_LEFT)) {
        double oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cos(-rotSpeed * dt) - player->dir.y * sin(-rotSpeed * dt);
        player->dir.y = oldDirX * sin(-rotSpeed * dt) + player->dir.y * cos(-rotSpeed * dt);
        double oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cos(-rotSpeed * dt) - player->plane.y * sin(-rotSpeed * dt);
        player->plane.y = oldPlaneX * sin(-rotSpeed * dt) + player->plane.y * cos(-rotSpeed * dt);
    }
    if (IsKeyDown(KEY_RIGHT)) {
        double oldDirX = player->dir.x;
        player->dir.x = player->dir.x * cos(rotSpeed * dt) - player->dir.y * sin(rotSpeed * dt);
        player->dir.y = oldDirX * sin(rotSpeed * dt) + player->dir.y * cos(rotSpeed * dt);
        double oldPlaneX = player->plane.x;
        player->plane.x = player->plane.x * cos(rotSpeed * dt) - player->plane.y * sin(rotSpeed * dt);
        player->plane.y = oldPlaneX * sin(rotSpeed * dt) + player->plane.y * cos(rotSpeed * dt);
    }

    float nextX = player->pos.x + dx;
    float nextY = player->pos.y + dy;

    int cellX = (int)(nextX / cellwidth);
    int cellY = (int)(player->pos.y / cellheight);
    if (world_map[cellX][cellY] == 0) player->pos.x = nextX;

    cellX = (int)(player->pos.x / cellwidth);
    cellY = (int)(nextY / cellheight);
    if (world_map[cellX][cellY] == 0) player->pos.y = nextY;
}
