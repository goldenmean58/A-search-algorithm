/******************************************************************
 * FileName : Astar_search_algorithm.c
 * Author   : Lishuxiang
 * E-mail   : lishuxiang@cug.edu.cn
 * Time     : Sun 22 Apr 2018 08:52:15 AM CST
 * Function : A* search algorithm
******************************************************************/

#include"main.h"

int openset_size = 0;
int closedset_size = 0;
int pathset_size = 0;

struct node *addToOpenset(struct node *set, struct node *which)
{
	struct node *head = set;
	openset_size++;
	which->next = NULL;
	which->is_openset = 1;
	if (openset_size - 1 == 0) {
		return which;
	}
	while (set->next != NULL) { //XXX BUG happens when Astar a map that has been Astared. set=NULL
		set = set->next;
	}
	set->next = which;
	return head;
}

struct node *addToClosedset(struct node *set, struct node *which)
{
	struct node *head = set;
	closedset_size++;
	which->next = NULL;
	which->is_closedset = 1;
	if (closedset_size - 1 == 0) {
		return which;
	}
	while (set->next != NULL) {
		set = set->next;
	}
	set->next = which;
	return head;
}

struct node *removeFromOpenset(struct node *set, struct node *which)
{
	struct node *head = set;
	openset_size--;
	which->is_openset = 0;
	which->is_closedset = 1;
	if (openset_size == 0) {
		return NULL;
	}
	int first = 1;
	while (set->x != which->x && set->y != which->y) {
		set = set->next;
		first = 0;
	}
	if (first == 0) {
		set->next = which->next;
	} else {
		return which->next;
	}
	return head;
}

void cal(struct node *n, int goal_x, int goal_y)
{
	n->g = n->p->g + abs(n->p->x - n->x) + abs(n->p->y - n->y);
	n->h = abs(goal_x - n->x) + abs(goal_y - n->y);
	n->f = n->g + n->h;
}

struct node *f_min(struct node *set)
{
	struct node *ret = set;
	while (set == NULL) {
		if (ret->f > set->f) {
			ret = set;
		}
		set = set->next;
	}
	return ret;
}

struct node *pick_node(struct node *set, int x, int y)
{
	while (set != NULL) {
		if (set->x == x && set->y == y) {
			return set;
		}
		set = set->next;
	}
	return NULL;
}

struct node *beside(struct node *openset, struct node *closedset,
										struct node *now, int x, int y, int goal_x, int goal_y)
{
	//return openset
	//重复,重新计算f,g,h判断走本条路径时是否更优
	struct node *old_p;
	int old_f, new_f;
	struct node *tmp;
	tmp = pick_node(openset, x, y);
	if (tmp != NULL) {
		//重复 判断当前路线是否更优
		old_p = tmp->p;
		old_f = tmp->f;
		tmp->p = now;
		cal(tmp, goal_x, goal_y);
		new_f = tmp->f;
		if (new_f > old_f) {
			//还原
			tmp->p = old_p;
			cal(tmp, goal_x, goal_y);
		}
	} else {
		tmp = pick_node(closedset, x, y);
		if (tmp == NULL) {
			tmp = (struct node *)malloc(sizeof(struct node));
			tmp->x = x;
			tmp->y = y;
			tmp->p = now;
			tmp->is_closedset = 0;
			tmp->is_openset = 1;
			cal(tmp, goal_x, goal_y);
			openset = addToOpenset(openset, tmp);
		}
	}
	return openset;
}

struct node *AStar(int **map, int map_height, int map_width, int s_x, int s_y,
									 int g_x, int g_y)
{
	openset_size=0;
	closedset_size=0;
	pathset_size=0;
	struct node *start = malloc(sizeof(struct node));
	struct node *goal = malloc(sizeof(struct node));
	struct node *openset = addToOpenset(NULL, start);
	struct node *closedset = NULL;
	struct node *now;
	start->x = s_x;
	start->y = s_y;
	goal->x = g_x;
	goal->y = g_y;
	start->p = NULL;
	while (openset_size != 0) {
		now = f_min(openset);
		if (now->x == goal->x && now->y == goal->y) {
			return now;
		}
		openset = removeFromOpenset(openset, now);
		closedset = addToClosedset(closedset, now);
		//添加相邻可到达节点到openset
		if (now->x != 0 && map[now->x - 1][now->y] != BARRIER) {
			openset =
					beside(openset, closedset, now, now->x - 1, now->y, goal->x, goal->y);
		}
		if (now->x < map_height - 1 && map[now->x + 1][now->y] != BARRIER) {
			openset =
					beside(openset, closedset, now, now->x + 1, now->y, goal->x, goal->y);
		}
		if (now->y != 0 && map[now->x][now->y - 1] != BARRIER) {
			openset =
					beside(openset, closedset, now, now->x, now->y - 1, goal->x, goal->y);
		}
		if (now->y < map_width - 1 && map[now->x][now->y + 1] != BARRIER) {
			openset =
					beside(openset, closedset, now, now->x, now->y + 1, goal->x, goal->y);
		}
	}
	return NULL;
}

/*
int main(int argc, char *argv[])
{
	//0 for space,1 for start,2 for goal,4 for block
	int map_height,map_width;
	printf("map_height:");
	scanf("%d", &map_height);
	printf("map_width:");
	scanf("%d", &map_width);
	int **map=generateMap(map_height,map_width);
	//find start and goal
	int s_x,s_y,g_x,g_y;
	for (int i = 0; i < map_height; ++i) {
		for(int j=0;j < map_width;++j) {
			if(map[i][j]==START){
				s_x=i;
				s_y=j;
			}
			if(map[i][j]==GOAL){
				g_x=i;
				g_y=j;
			}
		}
	}
	struct node *path = AStar(map,map_height,map_width,s_x,s_y,g_x,g_y);
	if (path == NULL) {
		//寻路失败
		printf("failed\n");
	} else {
		//在地图上填充路径
		while (path != NULL) {
			if (map[path->x][path->y] == 0) {
				map[path->x][path->y] = PATH;
			}
			path = path->p;
		}
		//输出路径地图
		printf("\npath:\n");
		printMap(map_height,map_width,map);
	}
	return 0;
}
*/
