/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include "circle.h"
#include <iostream>
#include <random>

//define window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//define SDL Window related variables
SDL_Window* window = NULL;
SDL_Renderer* windowRenderer = NULL;
SDL_Event currentEvent;

SDL_Rect paleta;
SDL_Rect wall1;
SDL_Rect wall2; 
SDL_Rect wall3;

bool quit = false;

Circle cerc;
bool animationRunning = false;

glm::vec2 movementDirection;
float dt = 0.1f;

float elapsedTime = 0.0f;

float speed = 2000.0f;
bool goingDown;
bool goingRight;

bool initWindow() {

    bool success = true;

    //Try to initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;

    }
    else {

        //Try to create the window
        window = SDL_CreateWindow(
            "SDL Hello Window Example",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if (window == NULL) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;

        }
        else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

            if (windowRenderer == NULL) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;

            }
            else {

                //Set background color
                SDL_SetRenderDrawColor(windowRenderer, 255, 255, 255, 255);

                //Apply background color
                SDL_RenderClear(windowRenderer);
            }
        }
    }

    return success;
}

bool fRand()
{
    std::vector<bool> list{ false, true };
    int index = rand() % list.size(); // pick a random index
    bool value = list[index];
    return value;
}
void reset() {
    goingDown = fRand();
    goingRight = fRand();
    cerc.center.x = 400.0f;
    cerc.center.y = 300.0f;
    speed = 2000.0f;
    paleta.x = 0;
    paleta.y = 225;
}
void processEvents() {

    //Check for events in queue
    SDL_PollEvent(&currentEvent);

    //User requests quit
    if (currentEvent.type == SDL_QUIT) {

        quit = true;
    }

    //Keyboard event
    if (currentEvent.type == SDL_KEYDOWN) {

        switch (currentEvent.key.keysym.sym) {

        case SDLK_UP:
            if (paleta.y > wall2.y+wall2.h && animationRunning)
                paleta.y -= speed /250;
            break;

        case SDLK_DOWN:
            if (paleta.y < wall3.y-150 && animationRunning)
                paleta.y += speed /250;
            break;

        case SDLK_LEFT:
            break;

        case SDLK_RIGHT:
            break;

        case SDLK_r:
            reset();
            break;

        case SDLK_s:
            animationRunning = !animationRunning;
            break;

        case SDLK_ESCAPE:
            quit = true;
            break;

        default:
            break;
        }
    }
}


void animationStep() {
    static int bounce = 0;



    if (goingRight) {
        cerc.center.x += speed *  dt * elapsedTime;
        if (cerc.center.x + cerc.radius > wall1.x) {
            goingRight = false;
        }
    }
           else {
               cerc.center.x -= speed *  dt * elapsedTime;
               if (cerc.center.x - cerc.radius < paleta.w && (cerc.center.y>paleta.y && cerc.center.y<paleta.y + paleta.h)){
                   goingRight = true;
                   bounce += 1;
                   if (bounce == 3)
                   {
                       speed *= 2;
                       bounce = 0;
                   }
               }
               else if (cerc.center.x - cerc.radius < 0.0f) {        
                   reset();
               }
           }


        if (goingDown) {
               cerc.center.y += speed * dt * elapsedTime;
               if (cerc.center.y + cerc.radius > wall3.y) {
                   goingDown = false;
               }

           }
        else {
            cerc.center.y -= speed * dt * elapsedTime;
            if (cerc.center.y - cerc.radius < wall2.y + wall2.h) {
                goingDown = true;
            }
        }
    SDL_SetRenderDrawColor(windowRenderer, 0, 0, 0, 255);

}

void drawFrame() {
    Uint32 start = SDL_GetTicks();

    SDL_SetRenderDrawColor(windowRenderer, 20, 20, 60, 255);
    SDL_RenderClear(windowRenderer);

    SDL_SetRenderDrawColor(windowRenderer, 100, 130, 180, 255);
    SDL_RenderFillRect(windowRenderer, &wall1);

    SDL_SetRenderDrawColor(windowRenderer, 100, 130, 180, 255);
    SDL_RenderFillRect(windowRenderer, &wall2);

    SDL_SetRenderDrawColor(windowRenderer, 100, 130, 180, 255);
    SDL_RenderFillRect(windowRenderer, &wall3);

    SDL_SetRenderDrawColor(windowRenderer, 130, 130, 230, 255);
    SDL_RenderFillRect(windowRenderer, &paleta);


    SDL_FillCircle(windowRenderer, cerc);

    if (animationRunning) {
        animationStep();
    }

    SDL_RenderPresent(windowRenderer);


    Uint32 end = SDL_GetTicks();
    elapsedTime = (end - start) / 1000.0f;
}

void cleanup() {

    //Destroy renderer
    if (windowRenderer) {

        SDL_DestroyRenderer(windowRenderer);
        windowRenderer = NULL;
    }

    //Destroy window
    if (window) {

        SDL_DestroyWindow(window);
        window = NULL;
    }

    //Quit SDL
    SDL_Quit();
}

void initObjects() {
    goingDown = fRand();
    goingRight = fRand();

    //init pallet
    paleta.x = 0;
    paleta.y = 225;
    paleta.w = 10;
    paleta.h = 150;

    //init walls
    wall1.x = 780;
    wall1.y = 0;
    wall1.w = 20;
    wall1.h = 600;

    wall2.x = 0;
    wall2.y = 0;
    wall2.w = 800;
    wall2.h = 20;

    wall3.x = 0;
    wall3.y = 580;
    wall3.w = 800;
    wall3.h = 20;

    //init circle
    cerc.radius = 30.0f;
    cerc.center.x = 400.0f;
    cerc.center.y = 300.0f;
    cerc.color = glm::vec4(230.0f, 100.0f, 100.0f, 255.0f);
}

int main(int argc, char* argv[]) {
    //Initialize window
    if (!initWindow()) {

        std::cout << "Failed to initialize" << std::endl;
        return -1;
    }

    initObjects();

    //Game loop
    while (!quit) {
        processEvents();
        drawFrame(); 
    }

    cleanup();
    return 0;
}