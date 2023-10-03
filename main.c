#include <curses.h>
#include <stdlib.h>
#include <unistd.h>

#define INFO_WIDTH 10
#define INFO_HEIGHT LINES
#define GAME_WIDTH (COLS - INFO_WIDTH)
#define GAME_HEIGHT LINES

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

Vector2 generate_apple() {
    Vector2 apple;
    apple.x = rand() % GAME_WIDTH;
    apple.y = rand() % GAME_HEIGHT;

    return apple;
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

    WINDOW *left_window = newwin(INFO_HEIGHT, INFO_WIDTH, 0, 0);
    WINDOW *game_window = newwin(GAME_HEIGHT, GAME_WIDTH, 0, INFO_WIDTH);

    int score = 0;

    
    refresh();

    wclear(left_window);
    mvwprintw(left_window, 0, 0, "Score: %d", score);
    wrefresh(left_window);

    Vector2 apple = generate_apple();

    int playing = 1;

    while(playing) {
        wclear(game_window);

        for(SnakeBody *s = snake; s != NULL; s = s->next) {
            Vector2 p = s->position;
            p.x += s->move_direction.x;
            p.y += s->move_direction.y;
            s->position = p;
            if(s->next != NULL) {
                s->move_direction = s->next->move_direction;
            }

            mvwprintw(game_window, s->position.y, s->position.x, "0");
        }

        Vector2 head_position = get_snake_head(snake)->position;
        if(head_position.x == apple.x && head_position.y == apple.y) {
            apple = generate_apple();
            snake = create_snake_body(snake->position, move_direction, snake);
            
            score++;
            wclear(left_window);
            mvwprintw(left_window, 0, 0, "Score: %d", score);
            wrefresh(left_window);
        }

        for(SnakeBody *s = snake; s->next != NULL; s = s->next) {
            if(s->position.x == head_position.x && s->position.y == head_position.y && score > 1)
                playing = false;
        }

        mvwprintw(game_window, apple.y, apple.x, "A");
        
        wrefresh(game_window);

        int key = getch();
        if(key != ERR) {
            Vector2 new_direction;
            if(key == 'd') {
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
