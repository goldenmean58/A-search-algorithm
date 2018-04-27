/******************************************************************
 * FileName : main.h
 * Author   : Lishuxiang
 * E-mail   : lishuxiang@cug.edu.cn
 * Time     : Fri 27 Apr 2018 08:35:50 PM CST
 * Function : 
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SPACE 0
#define START 1
#define GOAL 2
#define BARRIER 3
#define PATH 4

struct node {
	int x, y;
	int f, g, h;									//f(x)=g(x)+h(x)
	struct node *p;								//point ot parent node
	struct node *next;
	int is_openset;
	int is_closedset;
};

struct walls {
	int x, y;
	int from_x, from_y;
	struct walls *next;
};

int **generateMap(int map_height,int map_width);
void printMap(int map_height, int map_width, int **map);
struct node *AStar(int **map, int map_height,int map_width,int s_x,int s_y,int g_x,int g_y);
