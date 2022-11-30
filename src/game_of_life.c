// Copyright 2022 team
#include <stdio.h>
#include <ncurses.h>
#define N 25
#define M 80

void start(void);
void init(int m1[N][M], int m2[N][M]);
void cl(int m[N][M]);
int open_file(int m[N][M]);
void filler(int m[N][M], FILE *map);
int key_handler(char c, int *call_down);
void print_stats(int generation, int speed);
void draw(int m1[N][M], int m2[N][M], int *generation);
void draw_and_change(int m1[N][M], int m2[N][M]);
void draw_matrix(int m[N][M]);
void change_generation(int m1[N][M], int m2[N][M]);
void change_matrix(int m1[N][M], int m2[N][M], int i, int j);
int check(int m[N][M], int im, int jm);
int get(int m[N][M], int i, int j);

int main() {
    start();
    return 0;
}

void start(void) {
    int m1[N][M];
    int m2[N][M];
    init(m1, m2);
    if (open_file(m1)) {
        int generation = 0;
        int speed = 1;
        double last_time_call = 1;
        while (key_handler(getch(), &speed)) {
            print_stats(generation, speed);
            if (last_time_call > 1) {
                draw(m1, m2, &generation);
                last_time_call = 0;
            }
            last_time_call += 0.00002 * speed * speed;
        }
    }
    endwin();
}

void init(int m1[N][M], int m2[N][M]) {
    initscr();
    curs_set(FALSE);
    cl(m1);
    cl(m2);
}

void cl(int m[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            m[i][j] = 0;
        }
    }
}

int open_file(int m[N][M]) {
    FILE *map;
    char file = 0;
    int status = 0;
    int flag = 1;
    while (!status) {
        printw("Select Q for exit.\n");
        printw("Select 1 - 5 and press Enter.\nEnter number of script: ");
        refresh();
        scanw("%c", &file);
        if (file == '1') {
            map = fopen("map/1.txt", "r");
            status = 1;
        } else if (file == '2') {
            map = fopen("map/2.txt", "r");
            status = 1;
        } else if (file == '3') {
            map = fopen("map/3.txt", "r");
            status = 1;
        } else if (file == '4') {
            map = fopen("map/4.txt", "r");
            status = 1;
        } else if (file == '5') {
            map = fopen("map/5.txt", "r");
            status = 1;
        } else if (file == 'q' || file == 'Q') {
            flag = 0;
            status = 1;
        } else {
             printw("File not exist, try again!\n\n");
        }
    }
    if (flag) {
        nodelay(stdscr, TRUE);
        noecho();
        filler(m, map);
        fclose(map);
    }
    return flag;
}

void filler(int m[N][M], FILE *map) {
    char c = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            fscanf(map, "%c", &c);
            if (c == '0') m[i][j] = 0;
            if (c == '1') m[i][j] = 1;
        }
    }
}

int key_handler(char c, int *speed) {
    int flag = 1;
    if ((c == 'p' || c == 'P') && *speed < 9) {
        *speed = *speed + 1;
    }
    if ((c == 'o' || c == 'O') && *speed > 1) {
        *speed = *speed - 1;
    }
    if (c == 'q' || c == 'Q') {
        flag = 0;
    }
    if (c == ' ') {
        mvprintw(36, 0, "In pause");
        while (flag) {
            c = getch();
            if (c == ' ') break;
            if (c == 'q' || c == 'Q') {
                flag = 0;
            }
        }
        mvprintw(36, 0, "        ");
    }
    return flag;
}

void print_stats(int generation, int speed) {
    mvprintw(26, 0, "Generation: %d", generation);
    mvprintw(27, 0, "Speed: %d", speed);
    mvprintw(31, 0, "Space - pause");
    mvprintw(32, 0, "P - speed up");
    mvprintw(33, 0, "O - slow down");
    mvprintw(34, 0, "Q - exit");
    refresh();
}

void draw(int m1[N][M], int m2[N][M], int *generation) {
    if (*generation % 2 == 0) draw_and_change(m1, m2);
    if (*generation % 2 != 0) draw_and_change(m2, m1);
    *generation = *generation + 1;
    refresh();
}

void draw_and_change(int m1[N][M], int m2[N][M]) {
    draw_matrix(m1);
    change_generation(m1, m2);
}

void draw_matrix(int m[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (m[i][j] == 0) {
                mvaddch(i, j, '.');
            }
            if (m[i][j] == 1) {
                mvaddch(i, j, '#');
            }
        }
    }
    refresh();
}

void change_generation(int m1[N][M], int m2[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            change_matrix(m1, m2, i, j);
        }
    }
}

void change_matrix(int m1[N][M], int m2[N][M], int i, int j) {
    int neighborhoods = check(m1, i, j);
    if (m1[i][j] == 0 && neighborhoods == 3) m2[i][j] = 1;
    else if (m1[i][j] == 1 &&
            neighborhoods >= 2 &&
            neighborhoods <= 3) m2[i][j] = 1;
    else
        m2[i][j] = 0;
}

int check(int m[N][M], int im, int jm) {
    int sum = 0;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (i == 0 && j == 0) continue;
            if (get(m, i + im, j + jm) != 0) sum++;
        }
    }
    return sum;
}

int get(int m[N][M], int i, int j) {
    if (i > 24) {
        i = 0;
    }
    if (j > 79) {
        j = 0;
    }
    if (i < 0) {
        i = 24;
    }
    if (j < 0) {
        j = 79;
    }
    return m[i][j];
}
