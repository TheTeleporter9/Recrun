#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 850

#include "player.c"


int randInt(int min, int max) {
   return rand() % (max - min + 1) + min; 
}

int main() {
  
  //Init random number
  srand(time(NULL));

  InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Speedrunner");
  
  initPlayer();

  while (!WindowShouldClose())
  {
    
    
    updatePlayer(GetFrameTime());

    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawPlayer(); 

    EndDrawing();
  }

  CloseWindow();

  return 0;

}
