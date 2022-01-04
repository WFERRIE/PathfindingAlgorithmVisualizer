#include "raylib.h"
#include "node.h"
#include "BFS.h"

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

// cd build
// mingw32-make

// add buttons to set states via left click [DONE]
// make the button pretty [lower priority]
// make right click a reset for the node? [DONE]
// implement BFS [done ;)]
// refactor code to not be an unreadable mess [DONE-ish]

enum gameStates
{
    selectingStart,
    selectingEnd,
    selectingLocked,
    selectingAlgo,
    running,
    display
};

main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    gameStates gameState = selectingStart;

    std::unordered_map<int, Node> nodeDict = {};

    Button okButton(700, 100, 50, 50);

    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Pathfinding Algorithm Visualizer");

    const int GRIDSIZE = 4;
    const int RECSIZE = 75;
    const int RECSPACING = 80;
    const int GRIDMARGIN = 10;

    Node nodes[GRIDSIZE * GRIDSIZE];

    for (int i = 0; i < GRIDSIZE + 1; i++)
    {
        for (int j = 0; j < GRIDSIZE + 1; j++)
        {
            nodes[j + i * GRIDSIZE] = Node(GRIDMARGIN + (RECSPACING * j), GRIDMARGIN + (RECSPACING * i), RECSIZE, RECSIZE);
        }
    }

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        int startID = 0;
        int endID = GRIDSIZE - 1;
        for (int i = 0; i < GRIDSIZE; i++)
        {
            for (int j = 0; j < GRIDSIZE; j++)
            {
                if (nodes[j + i * GRIDSIZE].isLeftClicked())
                {
                    switch (gameState)
                    {
                    case selectingStart:
                    {
                        nodes[j + i * GRIDSIZE].setState(Node::Start);
                        startID = j + i * GRIDSIZE;
                        gameState = selectingEnd;
                        break;
                    }
                    case selectingEnd:
                    {
                        nodes[j + i * GRIDSIZE].setState(Node::End);
                        endID = j + i * GRIDSIZE;
                        gameState = selectingLocked;
                        break;
                    }
                    case selectingLocked:
                    {
                        if (nodes[j + i * GRIDSIZE].getState() == Node::Base) // check if node is lockable
                        {
                            nodes[j + i * GRIDSIZE].setState(Node::Locked);
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                    }
                }
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        okButton.Draw();

        switch (gameState)
        {
        case display:
        {
            DrawText("Algorithm Complete", 450, 100, 20, BLACK);
            break;
        }
        case selectingStart:
        {
            DrawText("Set Start node", 450, 100, 20, BLACK);
            break;
        }
        case selectingEnd:
        {
            DrawText("Set End Node", 450, 100, 20, BLACK);
            break;
        }
        case selectingLocked:
        {
            DrawText("Set Locked Nodes", 450, 100, 20, BLACK);
            if (okButton.isLeftClicked())
            {
                gameState = selectingAlgo;
            }
            break;
        }
        case selectingAlgo:
        {
            DrawText("Select Algorithm", 450, 100, 20, BLACK);
            if (okButton.isLeftClicked())
            {
                gameState = running;
            }
            break;
        }
        case running:
        {
            DrawText("Running...", 450, 100, 20, BLACK);
            // do BFS

            Graph g(GRIDSIZE * GRIDSIZE);

            int u;
            int v;

            for (int i = 0; i < GRIDSIZE; i++)
            {
                for (int j = 0; j < GRIDSIZE; j++)
                {
                    if (nodes[j + i * GRIDSIZE].getState() != Node::Locked)
                    {
                        std::cout << "yo" << std::endl;
                        if (i > 0 && (nodes[j + (i - 1) * GRIDSIZE].getState() != Node::Locked))
                        {
                            // add up
                            u = (GRIDSIZE * i) + j;
                            v = (GRIDSIZE * (i - 1)) + j;
                            g.addEdge(u, v);
                        }
                        if (i < (GRIDSIZE - 1) && (nodes[j + (i + 1) * GRIDSIZE].getState() != Node::Locked))
                        {
                            // add down
                            u = (GRIDSIZE * i) + j;
                            v = (GRIDSIZE * (i + 1)) + j;
                            g.addEdge(u, v);
                        }
                        if (j < (GRIDSIZE - 1) && (nodes[(j + 1) + i * GRIDSIZE].getState() != Node::Locked))
                        {
                            // add right
                            u = (GRIDSIZE * i) + j;
                            v = (GRIDSIZE * i) + (j + 1);
                            g.addEdge(u, v);
                        }
                        if (j > 0 && (nodes[(j - 1) + i * GRIDSIZE].getState() != Node::Locked))
                        {
                            // add left
                            u = (GRIDSIZE * i) + j;
                            v = (GRIDSIZE * i) + (j - 1);
                            g.addEdge(u, v);
                        }
                    }
                }
            }
            g.BFS(startID, endID, nodes, GRIDSIZE);
            gameState = display;
            break;
        }
        }
        DrawText("Base", 450, 200, 20, BEIGE);
        DrawText("Locked", 450, 250, 20, LIGHTGRAY);
        DrawText("Start", 450, 300, 20, PINK);
        DrawText("End", 450, 350, 20, PURPLE);

        for (int i = 0; i < GRIDSIZE; i++)
        {
            for (int j = 0; j < GRIDSIZE; j++)
            {
                nodes[j + i * GRIDSIZE].Draw();
            }
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
