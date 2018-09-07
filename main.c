/******************************************************************
 * FileName : main.c
 * Author   : Lishuxiang
 * E-mail   : lishuxiang@cug.edu.cn
 * Time     : Fri 27 Apr 2018 10:53:27 PM CST
 * Function : 
******************************************************************/

#include<stdio.h>
#include"main.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#define ECHOFLAGS (ECHO | ECHOE | ECHOK | ECHONL)
static struct termios tty;

void set_key()
{
	struct termios new_tty;
	tcgetattr(0, &tty);
	new_tty = tty;
	new_tty.c_lflag &= ~(ICANON | ECHO | ISIG);
	new_tty.c_cc[VTIME] = 0;
	new_tty.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_tty);
	return;
}

 //如果option为0，则关闭回显，为1则打开回显
int set_disp_mode(int fd, int option)
{
	int err;
	struct termios term;
	if (tcgetattr(fd, &term) == -1) {
		perror("Cannot get the attribution of the terminal");
		return 1;
	}
	if (option)
		term.c_lflag |= ECHOFLAGS;
	else
		term.c_lflag &= ~ECHOFLAGS;
	err = tcsetattr(fd, TCSAFLUSH, &term);
	if (err == -1 && err == EINTR) {
		perror("Cannot set the attribution of the terminal");
		return 1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	//game menu
	printf
			("Welcome to the maze game. Choose the degree of difficulty to play.\n");
	printf("1.Easy\n");
	printf("2.Medium\n");
	printf("3.Hard\n");
	printf("4.Customize\n");
	printf("Your choice: ");
	int difficult;
	int **map;
	int map_height, map_width;
	scanf("%d", &difficult);
	switch (difficult) {
	case 1:
		map_height = 10;
		map_width = 30;
		break;
	case 2:
		map_height = 20;
		map_width = 50;
		break;
	case 3:
		map_height = 30;
		map_width = 100;
		break;
	case 4:
		printf("Input map height:");
		scanf("%d", &map_height);
		printf("Input map width:");
		scanf("%d", &map_width);
	}
	map = generateMap(map_height, map_width);
	printMap(map_height, map_width, map);
	//find start and goal
	int s_x, s_y, g_x, g_y;
	for (int i = 0; i < map_height; ++i) {
		for (int j = 0; j < map_width; ++j) {
			if (map[i][j] == START) {
				s_x = i;
				s_y = j;
			}
			if (map[i][j] == GOAL) {
				g_x = i;
				g_y = j;
			}
		}
	}
	set_disp_mode(STDIN_FILENO, 0);
	set_key();
	int ch;
	struct node *path;
	while (1) {
		ch = getchar();
		if (ch == 'q' || ch == 'Q') {
			set_disp_mode(STDIN_FILENO, 1);
			return 0;
		}
		if (ch == 'g' || ch == 'G') {
			path = AStar(map, map_height, map_width, s_x, s_y, g_x, g_y);
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
			}
		}
		if (ch == 27) {
			ch = getchar();
			if (ch != 91)
				continue;
			map[s_x][s_y] = SPACE;
			ch = getchar();
			switch (ch) {
			case 65:									//up
				if (s_x > 0 && map[s_x - 1][s_y] != BARRIER) {
					s_x--;
				}
				break;
			case 66:									//down
				if (s_x < map_height - 1 && map[s_x + 1][s_y] != BARRIER) {
					s_x++;
				}
				break;
			case 68:									//left
				if (s_y > 0 && map[s_x][s_y - 1] != BARRIER) {
					s_y--;
				}
				break;
			case 67:									//right
				if (s_y < map_width - 1 && map[s_x][s_y + 1] != BARRIER) {
					s_y++;
				}
				break;
			}
		}
		map[s_x][s_y] = START;
		system("clear");
		printMap(map_height, map_width, map);
		printf
				("Note: ↑ for up, ↓ for down, ← for left, → for right. + is your location and * is your goal. Q for exit.\n");
		if (s_x == g_x && s_y == g_y) {
			printf("You win.\n");
			break;
		}
	}
	set_disp_mode(STDIN_FILENO, 1);
	return 0;
}
