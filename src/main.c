#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 850

#define BGColor   (Color){214,214,177,255}
#define CHBrown   (Color){63,63,55,255}
#define DrkKhaki  (Color){73,67,49,255}
#define GryOlive  (Color){135,132,114,255}
#define SpciPapka (Color){222,84,49,255}
#define ScndBrown (Color){134, 54, 20, 255}


//------------------------------------------------
// Variables
//------------------------------------------------

typedef enum {
  Triangle,
  Square,
  Rect, 
  Circle
}Shape;

typedef struct {
  Shape shape;
  Vector2 position;
  Color color;

  union {
    struct {
      Vector2 a;
      Vector2 b;
      Vector2 c;
    } triangle;

    struct {
      float size; 
    } square;

    struct {
      float width;
      float height;
    } rect;

    struct {
      float radius;
    } circle;
  };

}Object;

typedef struct {
  Object playerObj;
  Vector2 dir;
  float velocity;
  int speed;
}Player;

int randInt(int min, int max) {
   return rand() % (max - min + 1) + min; 
}

Player player = {
    .playerObj = {
        .shape = Circle,
        .position = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2},
        .color = {134, 54, 20, 255},
        .circle.radius = 50.0f
    },

    .dir = {0, 0},
    .speed = 500,
    .velocity = 0.0f
};

void drawObject(Object *obj)
{
    switch (obj->shape)
    {
        case Circle:
            DrawCircleV(
                obj->position,
                obj->circle.radius,
                obj->color
            );
            break;

        case Rect:
            DrawRectangleV(
                obj->position,
                (Vector2){
                    obj->rect.width,
                    obj->rect.height
                },
                obj->color
            );
            break;

        case Square:
            DrawRectangleV(
                obj->position,
                (Vector2){
                    obj->square.size,
                    obj->square.size
                },
                obj->color
            );
            break;

        case Triangle:
            DrawTriangle(
                obj->triangle.a,
                obj->triangle.b,
                obj->triangle.c,
                obj->color
            );
            break;
    }
}

int main() {
  
  //Init random number
  srand(time(NULL));

  InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "Speedrunner");
  
  
  while (!WindowShouldClose())
  {

    BeginDrawing();
    ClearBackground(BGColor);
    
    drawObject(&player.playerObj);

    EndDrawing();
  }

  CloseWindow();

  return 0;

}
