
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

using namespace std;
struct Point {
    int x, y;
};

class SnakeGame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    vector<Point> snake;
    Point fruit;
    int gridSize;
    int direction;
    bool running;

public:
    SnakeGame() {
        SDL_Init(SDL_INIT_VIDEO);
        IMG_Init(IMG_INIT_PNG);
        window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        gridSize = 20;
        direction = 1;
        running = true;
        snake.push_back({10, 10});
        spawnFruit();

        SDL_Surface* backgroundSurface = IMG_Load("background.jpg");
        if (!backgroundSurface) {
            cout << "Failed to load background image: " << IMG_GetError() << endl;
        } else {
            backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
            SDL_FreeSurface(backgroundSurface);
        }
    }

    ~SnakeGame() {
        SDL_DestroyTexture(backgroundTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }

    void spawnFruit() {
        fruit.x = rand() % (640 / gridSize);
        fruit.y = rand() % (480 / gridSize);
    }

    void handleInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        direction = 0;
                        break;
                    case SDLK_RIGHT:
                        direction = 1;
                        break;
                    case SDLK_DOWN:
                        direction = 2;
                        break;
                    case SDLK_LEFT:
                        direction = 3;
                        break;
                }
            }
        }
    }

    void update() {
        Point newHead = snake.front();
        switch (direction) {
            case 0:
                newHead.y--;
                break;
            case 1:
                newHead.x++;
                break;
            case 2:
                newHead.y++;
                break;
            case 3:
                newHead.x--;
                break;
        }

        if (newHead.x < 0 || newHead.x >= (640 / gridSize) || newHead.y < 0 || newHead.y >= (480 / gridSize)) {
            running = false;
            return;
        }

        snake.insert(snake.begin(), newHead);

        if (newHead.x == fruit.x && newHead.y == fruit.y) {
            spawnFruit();
        } else {
            snake.pop_back();
        }

        for (size_t i = 1; i < snake.size(); ++i) {
            if (newHead.x == snake[i].x && newHead.y == snake[i].y) {
                running = false;
                break;
            }
        }
    }

    void render() {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (const auto& segment : snake) {
            SDL_Rect rect = {segment.x * gridSize, segment.y * gridSize, gridSize, gridSize};
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect fruitRect = {fruit.x * gridSize, fruit.y * gridSize, gridSize, gridSize};
        SDL_RenderFillRect(renderer, &fruitRect);

        SDL_RenderPresent(renderer);
    }

    void run() {
        while (running) {
            handleInput();
            update();
            render();
            SDL_Delay(100);
        }
    }
};

int main(int argc, char* argv[]) {
    SnakeGame game;
    game.run();
    return 0;
}
