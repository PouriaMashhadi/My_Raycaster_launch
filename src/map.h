#ifndef MAP_H
#define MAP_H

#define W 16
#define H 16

void SaveWorldMap(const char *filename, int map[W][H]);
void LoadWorldMap(const char *filename, int map[W][H]);

#endif
