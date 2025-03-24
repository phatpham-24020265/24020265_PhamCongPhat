#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
// Khởi tạo
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 40;

// Hướng di chuyển của rắn của 2 chế độ
enum Direction { UP, DOWN, LEFT, RIGHT };
enum GameMode { CLASSIC, HEALTH_MODE };

// Dùng để quản lý rắn và thức ăn trong game.
struct Snake {
    std::vector<SDL_Point> body;
    Direction dir;
    int health;
};

struct Food {
    SDL_Point position;
};


// Khởi tạo trạng thái ban đầu của rắn khi game bắt đầu hoặc khi chơi lại.
void initSnake(Snake &snake) {
    snake.body.clear();
    snake.body.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    snake.dir = RIGHT;
    snake.health = 100;
}
// Đặt thức ăn vào một vị trí ngẫu nhiên trên màn hình.
void placeFood(Food &food) {
    food.position.x = (rand() % (SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
    food.position.y = (rand() % (SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;
}

// Cập nhật vị trí của rắn sau mỗi lần di chuyển.
void moveSnake(Snake &snake, bool &isAlive, GameMode mode) {
    SDL_Point newHead = snake.body[0];
    switch (snake.dir) {
        case UP:    newHead.y -= GRID_SIZE; break;
        case DOWN:  newHead.y += GRID_SIZE; break;
        case LEFT:  newHead.x -= GRID_SIZE; break;
        case RIGHT: newHead.x += GRID_SIZE; break;
    }
    // Kiểm tra va chạm và thân => Over

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT) {
        isAlive = false;
        return;
    }

    for (size_t i = 1; i < snake.body.size(); i++) {
        if (newHead.x == snake.body[i].x && newHead.y == snake.body[i].y) {
            isAlive = false;
            return;
        }
    }

    snake.body.insert(snake.body.begin(), newHead);
    // Nếu đang chơi chế độ Health Mode, giảm máu
    if (mode == HEALTH_MODE) snake.health -= 1;
}

// Check rắn có ăn không
bool checkCollision(const Snake &snake, const Food &food) {
    return (snake.body[0].x == food.position.x && snake.body[0].y == food.position.y);
}
 // hiển thị text lên màn hình bằng SDL2 và SDL_ttf.

void renderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), white);
    if (!surface) return;

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// GameMode showMenu(SDL_Renderer *renderer, TTF_Font *font) {
//     SDL_Event event;
//     while (true) {
//         SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
//         SDL_RenderClear(renderer);

//         renderText(renderer, font, "Select Mode:", 220, 150);
//         renderText(renderer, font, "1. Classic Mode", 200, 200);
//         renderText(renderer, font, "2. Health Mode", 200, 250);
//         SDL_RenderPresent(renderer);

//         while (SDL_WaitEvent(&event)) {
//             if (event.type == SDL_QUIT) exit(0);
//             if (event.type == SDL_KEYDOWN) {
//                 if (event.key.keysym.sym == SDLK_1) return CLASSIC;
//                 if (event.key.keysym.sym == SDLK_2) return HEALTH_MODE;
//             }
//         }
//     }
// }

// Chọn chế độ , button trò chơi
GameMode showMenu(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    while (true) {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255); // Màu nền cam
        SDL_RenderClear(renderer);

        renderText(renderer, font, "Select Mode:", 220, 150);

        // Vẽ nền xanh dương cho Classic Mode
        SDL_Rect classicBox = {190, 195, 260, 30};
        SDL_SetRenderDrawColor(renderer, 255, 102, 102, 102); // Màu xanh dương
        SDL_RenderFillRect(renderer, &classicBox);

        // Vẽ nền xanh dương cho Health Mode
        SDL_Rect healthBox = {190, 245, 260, 30};
        SDL_RenderFillRect(renderer, &healthBox);

        // Vẽ viền trắng cho các khung
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &classicBox);
        SDL_RenderDrawRect(renderer, &healthBox);

        // Hiển thị chữ màu trắng bên trong khung
        renderText(renderer, font, "1. Classic Mode", 200, 200);
        renderText(renderer, font, "2. Health Mode", 200, 250);

        SDL_RenderPresent(renderer);

        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) return CLASSIC;
                if (event.key.keysym.sym == SDLK_2) return HEALTH_MODE;
            }
        }
    }
}


// Hiển thị màn hình Game Over và chờ người chơi nhập lệnh để chơi lại hoặc thoát.
bool showGameOver(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_Event event;
    while (true) {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderClear(renderer);

        renderText(renderer, font, "Game Over!", 250, 150);
        renderText(renderer, font, "Press R to Retry or Q to Quit", 150, 200);
        SDL_RenderPresent(renderer);

        while (SDL_WaitEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_r) return true;
                if (event.key.keysym.sym == SDLK_q) return false;
            }
        }
    }
}
// Khởi tạo game random vị trí , SDL2 âm thanh , đồ hoạ game

int main(int argc, char* argv[])  {
    srand(time(0));
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return 1;
    if (TTF_Init() == -1) return 1;
    if (IMG_Init(IMG_INIT_PNG) == 0) return 1;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return 1;
    // Tạo màn hình game
    SDL_Window *window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Render đồ hoạ game phù hợp
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // Dùng Macos
    //TTF_Font *font = TTF_OpenFont("/System/Library/Fonts/Supplemental/Arial.ttf", 24);
    //Dùng Win
    TTF_Font *font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 24);

    SDL_Texture *headTexture = IMG_LoadTexture(renderer, "upmouth.png");
    SDL_Texture *bodyTexture = IMG_LoadTexture(renderer, "bodydi.png");
    SDL_Texture *foodTexture = IMG_LoadTexture(renderer, "apple.png");
    SDL_Texture *backgroundTexture = IMG_LoadTexture(renderer, "SnakeBG.png");

    Mix_Chunk *eatSound = Mix_LoadWAV("Untitled 1.wav");
    Mix_Chunk *loseSound = Mix_LoadWAV("Untitled 2.wav");
    // Vòng lặp , khởi tạo trò chơi
    while (true) {
        GameMode mode = showMenu(renderer, font);
        Snake snake;
        Food food;
        initSnake(snake);
        placeFood(food);
        bool isAlive = true;
        // Vòng lặp chính
        while (isAlive) {
            SDL_Event event;
            // Sử lý event khi user nhập lệnh bàn phím
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) return 0;
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:    if (snake.dir != DOWN) snake.dir = UP; break;
                        case SDLK_DOWN:  if (snake.dir != UP) snake.dir = DOWN; break;
                        case SDLK_LEFT:  if (snake.dir != RIGHT) snake.dir = LEFT; break;
                        case SDLK_RIGHT: if (snake.dir != LEFT) snake.dir = RIGHT; break;
                    }
                }
            }

            moveSnake(snake, isAlive, mode);
            // Kiểm tra va chạm thức ăn
            if (checkCollision(snake, food)) {
                snake.health += 10;
                placeFood(food);
                Mix_PlayChannel(-1, eatSound, 0);
            } else {
                snake.body.pop_back();
            }
            // Hiển thị lên màn hình
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            // vẽ nền
            SDL_Rect foodRect = {food.position.x, food.position.y, GRID_SIZE, GRID_SIZE};
            SDL_RenderCopy(renderer, foodTexture, NULL, &foodRect);
            // Vẽ thức ăn
            for (size_t i = 0; i < snake.body.size(); i++) {
                SDL_Rect rect = {snake.body[i].x, snake.body[i].y, GRID_SIZE, GRID_SIZE};
                SDL_RenderCopy(renderer, (i == 0) ? headTexture : bodyTexture, NULL, &rect);
            }
            // Vẽ rắn
            renderText(renderer, font, "Score: " + std::to_string(snake.body.size()), 10, 10);
            if (mode == HEALTH_MODE) renderText(renderer, font, "Health: " + std::to_string(snake.health), 10, 40);
            // Kiểm tra rắn hết máu
            if (snake.health <= 0) isAlive = false;
            // Cập nhật màn hình
            SDL_RenderPresent(renderer);
            SDL_Delay(200);
        }
        // Xử lý âm thanh khi rắn chết
        Mix_PlayChannel(-1, loseSound, 0);
        SDL_Delay(1000);
        if (!showGameOver(renderer, font)) break;
    }

    SDL_Quit();
    return 0;
}
