#include <iostream>
#include "raylib.h"
#pragma once

class Node
{
public:
    int posX;
    int posY;
    int width;
    int height;
    int id;

    enum States
    {
        Base,
        Locked,
        Start,
        End,
        Queued,
        Processed
    };

    States _state = Base;

    void setState(States state)
    {
        _state = state;
    }

    States getState()
    {
        return _state;
    }

    void Draw()
    {
        switch (_state)
        {
        case Base:
            DrawRectangle(posX, posY, width, height, BEIGE);
            break;
        case Locked:
            DrawRectangle(posX, posY, width, height, LIGHTGRAY);
            break;
        case Start:
            DrawRectangle(posX, posY, width, height, PINK);
            break;
        case End:
            DrawRectangle(posX, posY, width, height, PURPLE);
            break;
        case Queued:
            DrawRectangle(posX, posY, width, height, YELLOW);
            break;
        case Processed:
            DrawRectangle(posX, posY, width, height, ORANGE);
            break;
        }
    }

    bool isLeftClicked()
    {
        Vector2 mousePos;
        mousePos = GetMousePosition();
        if (mousePos.x > posX && mousePos.x < posX + width)
        {
            if (mousePos.y > posY && mousePos.y < posY + height)
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool isRightClicked()
    {
        Vector2 mousePos;
        mousePos = GetMousePosition();
        if (mousePos.x > posX && mousePos.x < posX + width)
        {
            if (mousePos.y > posY && mousePos.y < posY + height)
            {
                if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                {
                    return true;
                }
            }
        }
        return false;
    }

    Node(int _posX = 0, int _posY = 0, int _width = 0, int _height = 0)
    {
        posX = _posX;
        posY = _posY;
        width = _width;
        height = _height;
    }
};

class Button : public Node
{
public:
    Color colour;
    enum States
    {
        selectingStart,
        selectingEnd,
        selectingLocked,
        selectingAlgo,
        running,
        display
    };

    Button(int _posX = 0, int _posY = 0, int _width = 0, int _height = 0, Color _colour = GREEN)
    {
        posX = _posX;
        posY = _posY;
        width = _width;
        height = _height;
        colour = _colour;
    }

    void Draw()
    {
        DrawRectangle(posX, posY, width, height, colour);
    }
};
