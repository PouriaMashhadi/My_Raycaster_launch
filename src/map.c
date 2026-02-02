#include "map.h"
#include <stdio.h>

void SaveWorldMap(const char *filename, int map[W][H])
{
    FILE *file = fopen(filename, "wb");
    if (!file) return;
    fwrite(map, sizeof(int), W * H, file);
    fclose(file);
}

void LoadWorldMap(const char *filename, int map[W][H])
{
    FILE *file = fopen(filename, "rb");
    if (!file) return;
    fread(map, sizeof(int), W * H, file);
    fclose(file);
}
