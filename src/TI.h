#ifndef _Tuch_Buttons_h
#define _Tuch_Buttons_h

#include "Adafruit_GFX.h"

#if defined(__AVR__)
  #include "Adafruit_ILI9341.h"
  typedef Adafruit_ILI9341 ILI9341;  
#elif defined(__arm__)
  #include "Adafruit_ILI9341_STM.h"
  typedef Adafruit_ILI9341_STM ILI9341; 
#endif


enum shape{circle, roundRect, rect, edgeRect=10, edgeRoundRect0, edgeRoundRect1, edgeCircle=30};

class UI
{
public:
  UI(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc,
   const char *des, const GFXfont *f, ILI9341 *lcdPointer);

  ~UI(){}
  // virtual void draw();

  void setColor(uint16_t bg);
  void setString(const char *des);
  void newLayout(uint16_t nx, uint16_t ny);
  void moveLayout(uint16_t nx, uint16_t ny);
  bool alive(int xt, int yt);

protected:  
  ILI9341* lcd;
  GFXfont* font;

  const char* string;
  uint8_t  size;

  static const uint8_t fillet = 7;
  uint16_t x1,x2,dx,y1,y2,dy;
  uint16_t centerWriteX,centerWriteY;
  uint16_t fgc,bgc;

};


class Button : public UI
{
public:
  Button(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc,
  const char *des, const GFXfont *f, ILI9341 *lcdPointer, 
  shape s);

  void draw();
  void draw(uint32_t bg, uint32_t fg);

protected:
  shape    Shape;
  uint16_t edge;
};


class Slider : public UI
{

public:
  Slider(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc,
  const char *des, const GFXfont *f, ILI9341 *lcdPointer,
  bool& tg);
  
  void draw();
  void toggle();

protected:
  static const uint8_t sw=20; //distance betwen too slider circles (slider width)
  static const uint8_t dec=4; // distance to inner part of slider (decreaser)
  static const uint8_t decx=10;
  bool& tg; //toggle  
  uint16_t midx;
  uint16_t rad;
};


#endif
