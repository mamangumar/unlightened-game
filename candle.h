#ifndef candle_h
#define candle_h

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Lilin
{
public:
  Lilin() {}

  // todo: setters
  void setIsOn(bool value)
  {
    isOn = value;
  }
  void setTexture(Texture &texture)
  {
    spriteLilin.setTexture(texture);

    float scaleFactor = 100.0f / 400.0f;
    spriteLilin.setScale(scaleFactor, scaleFactor);
  }
  void setPosition(Vector2f pos)
  {
    spriteLilin.setPosition(pos);
  }

  // todo: getters
  Sprite *getSprite()
  {
    return &spriteLilin;
  }
  bool getIsOn()
  {
    return isOn;
  }

  void draw(RenderWindow &window)
  {
    window.draw(spriteLilin);
  }

private:
  Sprite spriteLilin{};
  bool isOn = false;
};

#endif