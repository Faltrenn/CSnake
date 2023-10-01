#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    unsigned int x;
    unsigned int y;
} Vector2;

typedef struct body {
    Vector2 position;
    Vector2 move_direction;
    struct body *next;
} SnakeBody;

SnakeBody * create_snake_body(Vector2 position, Vector2 move_direction, SnakeBody *next) {
    SnakeBody *snake = malloc(sizeof(SnakeBody));
    snake->position = position;
    if(next != NULL) {
        Vector2 md;
        md.x = 0;
        md.y = 0;
        snake->move_direction = md;
    } else {
        snake->move_direction = move_direction;
    }
    snake->next = next;

    return snake;
}

SnakeBody * get_snake_head(SnakeBody *snake) {
    for(SnakeBody *s = snake; s != NULL; s = s->next) {
        if(s->next == NULL) {
            return s;
        }
    }
    return NULL;
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    nodelay(stdscr, 1);

    Vector2 position;
    position.x = COLS / 2;
    position.y = 0;

    Vector2 move_direction;
    move_direction.x = 1;
    move_direction.y = 0;

    SnakeBody *snake = create_snake_body(position, move_direction, NULL);

    while(1) {
        clear();
        for(SnakeBody *s = snake; s != NULL; s = s->next) {
            Vector2 p = s->position;
            p.x += s->move_direction.x;
            p.y += s->move_direction.y;
            s->position = p;
            if(s->next != NULL) {
                s->move_direction = s->next->move_direction;
            }

            mvprintw(s->position.y, s->position.x, "0");
        }
        refresh();

        int key = getch();
        if(key != ERR) {
            Vector2 new_direction;
            if(key == 'e') {
                snake = create_snake_body(snake->position, move_direction, snake);
            } else if(key == 'd') {
                new_direction.x = 1;
                new_direction.y = 0;
                get_snake_head(snake)->move_direction = new_direction;
            } else if(key == 'a') {
                new_direction.x = -1;
                new_direction.y = 0;
                get_snake_head(snake)->move_direction = new_direction;
            } else if(key == 's') {
                new_direction.x = 0;
                new_direction.y = 1;
                get_snake_head(snake)->move_direction = new_direction;
            } else if(key == 'w') {
                new_direction.x = 0;
                new_direction.y = -1;
                get_snake_head(snake)->move_direction = new_direction;
            }
        }
        usleep(1000000/10);
    }

    endwin();

    return 0;
}
