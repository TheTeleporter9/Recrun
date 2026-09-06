#include "raylib.h"
#include "raymath.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//------------------------------------------------
// Configuration
//------------------------------------------------

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 896

#define MAX_LEVEL_COMPONENTS 100

#define FRICTION 3500.0f

#define BGColor   (Color){214, 214, 177, 255}
#define CHBrown   (Color){63, 63, 55, 255}
#define DrkKhaki  (Color){73, 67, 49, 255}
#define GryOlive  (Color){135, 132, 114, 255}
#define SpciPapka (Color){222, 84, 49, 255}
#define ScndBrown (Color){134, 54, 20, 255}


//------------------------------------------------
// Types
//------------------------------------------------

typedef enum {
    Triangle,
    Square,
    Rect,
    Circle
} Shape;


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

} Object;


typedef struct {
    Object playerObj;

    // Physics
    float velocity;
    float acceleration;
    float maxSpeed;

} Player;


//------------------------------------------------
// Global Variables
//------------------------------------------------

Object level[MAX_LEVEL_COMPONENTS];

Player player = {
    .playerObj = {
        .shape = Circle,
        .position = {
            SCREEN_WIDTH / 2.0f,
            SCREEN_HEIGHT / 2.0f
        },
        .color = ScndBrown,
        .circle.radius = 50.0f
    },

    .velocity = 0.0f,
    .acceleration = 3000.0f,
    .maxSpeed = 400.0f
};


//------------------------------------------------
// Level
//------------------------------------------------

void loadLevel1(void)
{
    level[0] = (Object) {
        .shape = Rect,
        .position = {
            0,
            SCREEN_HEIGHT / 2.0f + 200
        },
        .color = GryOlive,
        .rect.width = SCREEN_WIDTH,
        .rect.height = 500
    };
}


//------------------------------------------------
// Utility
//------------------------------------------------

int randInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}


//------------------------------------------------
// Drawing
//------------------------------------------------

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
                (Vector2) {
                    obj->rect.width,
                    obj->rect.height
                },
                obj->color
            );
            break;


        case Square:
            DrawRectangleV(
                obj->position,
                (Vector2) {
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


void drawLevel(Object (*arr)[MAX_LEVEL_COMPONENTS])
{
    for (int i = 0; i < MAX_LEVEL_COMPONENTS; i++)
    {
        drawObject(&(*arr)[i]);
    }
}


//------------------------------------------------
// Player Physics
//------------------------------------------------

void playerUpdate(float dt)
{
    float direction = 0.0f;


    //------------------------------------------------
    // Input
    //------------------------------------------------

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction += 1.0f;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction -= 1.0f;
    }


    //------------------------------------------------
    // Acceleration
    //------------------------------------------------

    if (direction != 0.0f)
    {
        player.velocity +=
            direction *
            player.acceleration *
            dt;


        //------------------------------------------------
        // Clamp maximum speed
        //------------------------------------------------

        if (player.velocity > player.maxSpeed)
        {
            player.velocity = player.maxSpeed;
        }

        if (player.velocity < -player.maxSpeed)
        {
            player.velocity = -player.maxSpeed;
        }
    }


    //------------------------------------------------
    // Friction
    //------------------------------------------------

    else
    {
        if (player.velocity > 0.0f)
        {
            player.velocity -= FRICTION * dt;

            if (player.velocity < 0.0f)
            {
                player.velocity = 0.0f;
            }
        }


        if (player.velocity < 0.0f)
        {
            player.velocity += FRICTION * dt;

            if (player.velocity > 0.0f)
            {
                player.velocity = 0.0f;
            }
        }
    }


    //------------------------------------------------
    // Apply velocity
    //------------------------------------------------

    player.playerObj.position.x +=
        player.velocity * dt;
}


//------------------------------------------------
// Debug UI
//------------------------------------------------

void drawDebug(float dt)
{
    DrawRectangle(
        10,
        10,
        330,
        190,
        (Color) {0, 0, 0, 180}
    );


    DrawText(
        TextFormat("FPS: %d", GetFPS()),
        20,
        20,
        20,
        WHITE
    );


    DrawText(
        TextFormat("DT: %.5f", dt),
        20,
        45,
        20,
        WHITE
    );


    DrawText(
        TextFormat(
            "Position X: %.2f",
            player.playerObj.position.x
        ),
        20,
        70,
        20,
        WHITE
    );


    DrawText(
        TextFormat(
            "Position Y: %.2f",
            player.playerObj.position.y
        ),
        20,
        95,
        20,
        WHITE
    );


    DrawText(
        TextFormat(
            "Velocity: %.2f",
            player.velocity
        ),
        20,
        120,
        20,
        WHITE
    );


    DrawText(
        TextFormat(
            "Acceleration: %.2f",
            player.acceleration
        ),
        20,
        145,
        20,
        WHITE
    );


    DrawText(
        TextFormat(
            "Max Speed: %.2f",
            player.maxSpeed
        ),
        20,
        170,
        20,
        WHITE
    );
}


//------------------------------------------------
// Main
//------------------------------------------------

int main(void)
{
    // Random seed
    srand(time(NULL));


    // Initialize window
    InitWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Speedrunner"
    );


    SetTargetFPS(60);


    // Load level
    loadLevel1();


    //------------------------------------------------
    // Game Loop
    //------------------------------------------------

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();


        //------------------------------------------------
        // Update
        //------------------------------------------------

        playerUpdate(dt);


        //------------------------------------------------
        // Draw
        //------------------------------------------------

        BeginDrawing();

        ClearBackground(BGColor);


        // Debug information
        drawDebug(dt);


        // Game
        drawObject(&player.playerObj);

        drawLevel(&level);


        EndDrawing();
    }


    //------------------------------------------------
    // Cleanup
    //------------------------------------------------

    CloseWindow();

    return 0;
}
