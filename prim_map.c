/******************************************************************
 * FileName : prim_map.c
 * Author   : Lishuxiang
 * E-mail   : lishuxiang@cug.edu.cn
 * Time     : Fri 27 Apr 2018 07:35:10 AM CST
 * Function : generate the map by using prim algorithm
******************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SPACE 0
#define START 1
#define GOAL 2
#define BARRIER 3
#define PATH 4

struct walls {
	int x, y;
	int from_x, from_y;
	struct walls *next;
};

void printMap(int map_height, int map_width, int **map)
{
	//output boundary
	//up boundary
	   for(int i=0;i<map_width+2;++i){
	   printf("█");
	   } 
	printf("\n");
	for (int i = 0; i < map_height; ++i) {
		printf("█");
		for (int j = 0; j < map_width; ++j) {
			switch (map[i][j]) {
			case SPACE:
				printf(" ");
				break;
			case START:
				printf("+");
				break;
			case GOAL:
				printf("*");
				break;
			case BARRIER:
				printf("█");
				break;
			case PATH:
				printf("Y");
				break;
			}
		}
		printf("█");
		printf("\n");
	}
	   for (int i = 0; i < map_width+2; ++i) {
	   printf("█");
	   }
	printf("\n");
}

struct walls *addWall(struct walls *wall_list, int wall_x, int wall_y,
											int from_x, int from_y)
{
	struct walls *wall = (struct walls *)malloc(sizeof(struct walls));
	struct walls *now = NULL;
	wall->x = wall_x;
	wall->y = wall_y;
	wall->from_x = from_x;
	wall->from_y = from_y;
	wall->next = NULL;
	now = wall_list;
	if (wall_list != NULL) {
		while (now->next != NULL) {
			now = now->next;
		}
		now->next = wall;
	} else {
		wall_list = wall;
	}
	return wall_list;
}

int addItsWall(int map_height, int map_width, int **map,
							 struct walls **pwall_list, int x, int y)
{
	int count = 0;								//how much walls has been added
	//TODO
	//XXX the wall_list should always points to the head
	//XXX first judge the wall is real a wall not a space
	//four corners
	if (x == 0 && y == 0) {
		if (map[1][0] == BARRIER) {
			*pwall_list = addWall(*pwall_list, 1, 0, x, y);
			count++;
		}
		if (map[0][1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, 0, 1, x, y);
			count++;
		}
	}
	if (x == map_height - 1 && y == map_width - 1) {
		if (map[map_height - 2][map_width - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, map_height - 2, map_width - 1, x, y);
			count++;
		}
		if (map[map_height - 1][map_width - 2] == BARRIER) {
			*pwall_list = addWall(*pwall_list, map_height - 1, map_width - 2, x, y);
			count++;
		}
	}
	if (x == map_height - 1 && y == 0) {
		if (map[map_height - 2][0] == BARRIER) {
			*pwall_list = addWall(*pwall_list, map_height - 2, 0, x, y);
			count++;
		}
		if (map[map_height - 1][1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, map_height - 1, 1, x, y);
			count++;
		}
	}
	if (x == 0 && y == map_width - 1) {
		if (map[0][map_width - 2] == BARRIER) {
			*pwall_list = addWall(*pwall_list, 0, map_width - 2, x, y);
			count++;
		}
		if (map[1][map_width - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, 1, map_width - 1, x, y);
			count++;
		}
	}
	//four edge
	if (x == 0 && y > 0 && y < map_width - 1) {
		//up
		if (map[x + 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x + 1, y, x, y);
			count++;
		}
		if (map[x][y - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y - 1, x, y);
			count++;
		}
		if (map[x][y + 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y + 1, x, y);
			count++;
		}
	}
	if (x == map_height - 1 && y > 0 && y < map_width - 1) {
		//down
		if (map[x - 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x - 1, y, x, y);
			count++;
		}
		if (map[x][y - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y - 1, x, y);
			count++;
		}
		if (map[x][y + 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y + 1, x, y);
			count++;
		}
	}
	if (y == map_width - 1 && x > 0 && x < map_height - 1) {
		//right
		if (map[x][y - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y - 1, x, y);
			count++;
		}
		if (map[x - 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x - 1, y, x, y);
			count++;
		}
		if (map[x + 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x + 1, y, x, y);
			count++;
		}
	}
	if (y == 0 && x > 0 && x < map_height - 1) {
		//left
		if (map[x][y + 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y + 1, x, y);
			count++;
		}
		if (map[x - 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x - 1, y, x, y);
			count++;
		}
		if (map[x + 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x + 1, y, x, y);
			count++;
		}
	}
	//center
	if (x > 0 && y > 0 && x < map_height - 1 && y < map_width - 1) {
		if (map[x - 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x - 1, y, x, y);
			count++;
		}
		if (map[x + 1][y] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x + 1, y, x, y);
			count++;
		}
		if (map[x][y - 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y - 1, x, y);
			count++;
		}
		if (map[x][y + 1] == BARRIER) {
			*pwall_list = addWall(*pwall_list, x, y + 1, x, y);
			count++;
		}
	}
	return count;
	//struct walls* addWall(struct walls* wall_list,int wall_x,int wall_y,int from_x,int from_y)
}

struct walls *removeWall(struct walls *wall_list, struct walls *wall)
{
	struct walls *pre = NULL;
	struct walls *now = wall_list;
	while (now != NULL) {
		if (now == wall) {
			if (now == wall_list) {
				wall_list = wall_list->next;
				//free(now);
				break;
			} else {
				pre->next = now->next;
				//free(now);
				break;
			}
		}
		pre = now;
		now = now->next;
	}
	return wall_list;
}

int **generateMap(int map_height, int map_width)
{
	struct walls *wall_list = NULL;
	int list_count = 0;
	//1.all are walls
	int **map = (int **)malloc(sizeof(int *) * map_height);
	for (int i = 0; i < map_height; i++) {  
		map[i] = (int *)malloc(sizeof(int) * map_width);
	}
	for (int i = 0; i < map_height; ++i) {
		for (int j = 0; j < map_width; ++j) {
			map[i][j] = BARRIER;
		}
	}
	//2.random start and make it maze and add its walls into wall_list
	srand((unsigned)time(NULL));
	int s_x, s_y;//start   (x,y)
	int g_x,g_y;//goal   (x,y)
	s_x = rand() % map_height;
	s_y = rand() % map_width;
	map[s_x][s_y] = START;
	list_count += addItsWall(map_height, map_width, map, &wall_list, s_x, s_y);
	//3.is there any wall in the list?
	while (list_count != 0) {
		//4.random choose a wall in the list
		int contrary_x;
		int contrary_y;
		struct walls *wall = NULL;
		while(1){
			int random = rand() % list_count;
			wall = wall_list;
			for (int i = 0; i < random; ++i) {
				wall = wall->next;
			}
			//6.Is only one of the two cells that the wall divides visited?
			contrary_x = 2 * wall->x - wall->from_x;
			contrary_y = 2 * wall->y - wall->from_y;
			if(contrary_x<0||contrary_y<0||contrary_x>=map_height||contrary_y>=map_width){
				wall_list=removeWall(wall_list,wall); //bad wall make the passage out of boundary, remove it
				list_count--;
			} else {
				g_x=contrary_x;
				g_y=contrary_y;
				break;
			}
			if(list_count==0) {
				map[g_x][g_y]=GOAL;
				return map;
			}
		};
		//int cell1 = map[wall->from_x][wall->from_y];
		int cell2 = map[contrary_x][contrary_y];
		if (cell2 == BARRIER) {
			//7.Make the wall a passage and mark the unvisited cell as part of the maze. Add the neighboring walls of the cell to the wall list.  
			//remove the wall from the list
			wall_list = removeWall(wall_list, wall);
			list_count--;

			map[wall->x][wall->y] = SPACE;
			map[contrary_x][contrary_y] = SPACE;
			list_count +=
					addItsWall(map_height, map_width, map, &wall_list, contrary_x,
										 contrary_y);
		} else {
			wall_list = removeWall(wall_list, wall);
			list_count--;
		}
	}
	//if there is no wall in the list, then output the map
	map[g_x][g_y]=GOAL;
	return map;
}

int main(int argc, char *argv[])
{
	int map_height, map_width;
	printf("Please input map's height and width:\n");
	scanf("%d%d", &map_height, &map_width);
	int **map = NULL;
	map = generateMap(map_height, map_width);
	printMap(map_height, map_width, map);
	return 0;
}
