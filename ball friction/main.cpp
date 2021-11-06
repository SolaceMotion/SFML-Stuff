#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <random>
#include <string.h>
#include <stdio.h>
#include "src/ball.h"

#define PI 3.14159265

sf::RenderWindow *window;
Ball *ball;
sf::Font *font;

sf::Text *txt_Angle;
sf::Text *txt_FPS;

float dt = 0.0f;
float lastTime = 0.0f;

uint32_t width{700}, height{700};

void start()
{
  window = new sf::RenderWindow(sf::VideoMode(width, height), "Ball");

  //Capped framerate
  window->setFramerateLimit(120);

  ball = new Ball(window, 10);
  font = new sf::Font();
  font->loadFromFile("res/fonts/arial.ttf");
  txt_FPS = new sf::Text();
  txt_FPS->setCharacterSize(20);
  txt_FPS->setFillColor(sf::Color::White);
  txt_FPS->setPosition(10, 10);
  txt_FPS->setFont(*font);

  txt_Angle = new sf::Text();
  txt_Angle->setCharacterSize(20);
  txt_Angle->setFillColor(sf::Color::White);
  txt_Angle->setPosition(10, 35);
  txt_Angle->setFont(*font);
}

void destroy()
{
  delete window;
  delete ball;
  delete font;
  delete txt_Angle;
  delete txt_FPS;
}

sf::Vector2f mousePosBegin;
sf::Vector2f mousePosEnd;

//Distance between points
float distanceHyp(sf::Vector2f a, sf::Vector2f b)
{
  return (float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

//Angle with respect to x-axis
float angle(sf::Vector2f a, sf::Vector2f b)
{
  return atan2f((b.y - a.y), (b.x - a.x));
}

void draw()
{
  window->clear();
  window->draw(*txt_Angle);
  window->draw(*txt_FPS);
  ball->draw(window);
  txt_Angle->setString("Ball angle: " + std::to_string(ball->getAngle() * 180 / PI) + " deg");
  txt_FPS->setString("FPS: " + std::to_string(lastTime));
}

void update()
{
  ball->update(dt);
  ball->applyFriction(dt);
}

/* struct AABB
{
  float x, y, w, h;
  AABB(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
  bool intersects(AABB &other)
  {
    return !(x > other.x + other.w || x + w < other.x || y > other.y + other.h || y + h < other.y);
  }
}; */

//Axis align bounding box
/* bool AABBvsAABB(const AABB &a, const AABB &b)
{
  // Exit with no intersection if found separated along an axis
  if (a.max.x < b.min.x or a.min.x > b.max.x)
    return false;
  if (a.max.y < b.min.y or a.min.y > b.max.y)
    return false;

  // No separating axis found, therefore there is at least one overlapping axis
  return true;
} */

void eventHandler(sf::Event *event)
{
  if (event->type == sf::Event::Closed)
  {
    window->close();
  }

  // if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  // {
  //   std::cout << sf::Mouse::getPosition().y << std::endl;
  // }

  if (ball->getSpeed() > 0)
    return;

  if (event->type == sf::Event::MouseButtonPressed)
  {
    if (event->mouseButton.button == sf::Mouse::Left)
    {
      mousePosBegin.x = sf::Mouse::getPosition(*window).x;
      mousePosBegin.y = sf::Mouse::getPosition(*window).y;
    }
  }

  if (event->type == sf::Event::MouseButtonReleased)
  {
    if (event->mouseButton.button == sf::Mouse::Left)
    {
      mousePosEnd.x = sf::Mouse::getPosition(*window).x;
      mousePosEnd.y = sf::Mouse::getPosition(*window).y;

      //Constraints
      if (mousePosEnd.x >= (int)window->getSize().x)
      {
        mousePosEnd.x = window->getSize().x;
      }
      if (mousePosEnd.x <= 0)
      {
        mousePosEnd.x = 0;
      }
      if (mousePosEnd.y >= (int)window->getSize().y)
      {
        mousePosEnd.y = window->getSize().y;
      }
      if (mousePosEnd.y <= 0)
      {
        mousePosEnd.y = 0;
      }

      //Set speed and angle
      ball->setSpeed(distanceHyp(mousePosBegin, mousePosEnd));
      ball->setAngle(angle(mousePosBegin, mousePosEnd));
    }
  }
}

int main(void)
{
  try
  {
    sf::Clock *deltaClock = new sf::Clock;
    sf::Clock *fpsClock = new sf::Clock;
    start();

    while (window->isOpen())
    {
      sf::Event event;

      while (window->pollEvent(event))
      {
        eventHandler(&event);
      }

      // Seconds since previous frame

      dt = deltaClock->getElapsedTime().asMicroseconds() / 1000000.0f;
      deltaClock->restart();

      float currentTime = fpsClock->restart().asSeconds();
      float fps = 1.f / currentTime;
      lastTime = fps;

      draw();
      update();

      window->display();
    }

    destroy();
  }

  catch (const std::exception &e)
  {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}