#pragma once
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <SFML/Graphics.hpp>

class Ball
{
private:
    float x;
    float y;
    float radius;
    float speed;
    float angle;
    sf::Vector2f direction;
    const float friction = 0.80f;
    sf::CircleShape shape;

public:
    Ball();
    Ball(sf::RenderWindow *window, float radius)
    {
        this->radius = radius;
        this->x = (window->getSize().x / 2) - this->radius;
        this->y = (window->getSize().y / 2) - this->radius;
    };

    virtual ~Ball()
    {
        delete &shape;
    }

    virtual void update(float deltaTime)
    {
        direction.x = cos(angle);
        direction.y = sin(angle);

        x += speed * direction.x * deltaTime;
        y += speed * direction.y * deltaTime;
    };

    virtual void draw(sf::RenderWindow *window)
    {
        shape.setPosition(x, y);
        shape.setRadius(radius);
        shape.setFillColor(sf::Color::Red);
        window->draw(shape);
    }

    void applyFriction(float deltaTime)
    {
        speed -= speed * deltaTime * friction;

        if (isless(speed, 10.f))
        {
            speed = 0.0f;
        }
    }

    void setSpeed(float speed)
    {
        this->speed = speed;
    }

    void setAngle(float angle)
    {
        this->angle = angle;
    }

    float getAngle()
    {
        return angle;
    }

    float getX()
    {
        return x;
    }

    float getY()
    {
        return y;
    }

    float getSpeed()
    {
        return speed;
    }
};