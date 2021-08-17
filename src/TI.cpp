#include "TI.h"

GFXglyph * pgm_read_glyph_ptr(const GFXfont *gfxFont, uint8_t c)
{
    #ifdef __AVR__
    return &(((GFXglyph *)pgm_read_ptr(&gfxFont->glyph))[c]);
    #else
    return gfxFont->glyph + c;
    #endif //__AVR__
}

//UI-----------------------------------------------------------------------------------------------
UI::UI(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc,
  const char *des, const GFXfont *f, ILI9341 *lcdPointer) : 
  x1(x1), y1(y1), dx(width), dy(height), fgc(fgc), bgc(bgc), 
  font((GFXfont*)f), lcd(lcdPointer)
{
  x2=x1+dx;
  y2=y1+dy;
  this->setString(des);
}

void UI::setString(const char *des) 
{
  string = des;
  centerWriteX=0;
  centerWriteY=0;
  size = 0;
  while(*(des+size)!=NULL)
  {
    ++size;
  }
  int sizeBuffer = size;
  char charBuffer[size];
  while(sizeBuffer)
  {
    sizeBuffer--;
    charBuffer[sizeBuffer] = *(des+sizeBuffer);
  }
  sizeBuffer = size;
  while(sizeBuffer)
  {
    sizeBuffer--;
    *(charBuffer+sizeBuffer) -= (uint8_t)pgm_read_byte(&font->first);
    GFXglyph *glyph  = pgm_read_glyph_ptr(font, *(charBuffer+sizeBuffer));
    centerWriteX +=pgm_read_byte(&glyph->xAdvance);
  }
  centerWriteX = -centerWriteX/2 + (x1+dx/2) - 2;
  centerWriteY =  pgm_read_byte(&font->yAdvance)/5 + (y1+dy/2) + 1;
}

void UI::setColor(uint16_t bg)
{
  bgc = bg;
}

void UI::newLayout(uint16_t nx, uint16_t ny)
{
  centerWriteX+=(nx-x1);
  centerWriteY+=(ny-y1);
  x1=nx;
  x2=nx+dx;
  y1=ny;
  y2=ny+dy;
}

void UI::moveLayout(uint16_t nx, uint16_t ny)
{
  centerWriteX+=nx;
  centerWriteY+=ny;
  x1+=nx;
  x2+=nx;
  y1+=ny;
  y2+=ny;
}

bool UI::alive(int xt, int yt)
{
  return ((xt>x1) && (xt<x2) && (yt>y1) && (yt<y2));
}

//Button-------------------------------------------------------------------------------------------
Button::Button(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc, 
  const char *des, const GFXfont *f, ILI9341 *lcdPointer, 
  shape s) :
  UI(x1, y1, width, height, fgc, bgc, des, f, lcdPointer), 
  edge(10), Shape(s)
{

}

void Button::draw()
{
  int i=0;

  switch (Shape)
  {
    case (edgeRoundRect0): //0 means buttom edge
    lcd->fillRoundRect(x1, y1, dx, dy+edge, fillet, bgc);
    lcd->setFont(font);
    lcd->setCursor(centerWriteX, centerWriteY);
    lcd->setTextColor(fgc);
    break;

    case (roundRect):
    lcd->fillRoundRect(x1, y1, dx, dy, fillet, bgc);
    lcd->setFont(font);
    lcd->setCursor(centerWriteX, centerWriteY);
    lcd->setTextColor(fgc);
    break;        
  }
  while(i<size)
  {
    lcd->write(*(string+i));
    ++i;
  }
}

void Button::draw(uint32_t tc, uint32_t bc)
{
  int i=0;

  switch (Shape)
  {
    case (edgeRoundRect0): //0 means buttom edge
    lcd->fillRoundRect(x1, y1, dx, dy+edge, fillet, bc);
    lcd->setFont(font);
    lcd->setCursor(centerWriteX, centerWriteY);
    lcd->setTextColor(tc);
    break;

    case (roundRect):
    lcd->fillRoundRect(x1, y1, dx, dy, fillet, bc);
    lcd->setFont(font);
    lcd->setCursor(centerWriteX, centerWriteY);
    lcd->setTextColor(tc);
    break;        
  }
  while(i<size)
  {
    lcd->write(*(string+i));
    ++i;
  }
}


//Slider-------------------------------------------------------------------------------------------
Slider::Slider(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint16_t fgc, uint16_t bgc,
  const char *des, const GFXfont *f, ILI9341 *lcdPointer,
  bool& tg) :
  UI(x1, y1, width, height, fgc, bgc, des, f, lcdPointer), 
  tg(tg)
{
  x2+=
  midx = x1+dx;
  rad = dy/2;
}

void Slider::draw()
{
  lcd->fillRoundRect(x1, y1, dx, dy+1, fillet, bgc);
  lcd->fillCircle(midx, y1+rad, rad, bgc);

  if(tg)
  {
    lcd->fillCircle(midx,    y1+rad, rad-dec, fgc);
    lcd->fillCircle(midx-sw, y1+rad, rad-dec, fgc);
    lcd->fillRect(midx-sw, y1+dec, sw, dy-2*dec, fgc);

    lcd->fillCircle(midx, y1+rad, rad-decx+4, bgc);
    lcd->fillCircle(midx, y1+rad, rad-decx, fgc);
  }
  else
  {
    lcd->fillCircle(midx,    y1+rad, rad-dec, fgc);
    lcd->fillCircle(midx-sw, y1+rad, rad-dec, fgc);
    lcd->fillRect(midx-sw, y1+dec, sw, dy-2*dec, fgc);
    lcd->fillCircle(midx,    y1+rad, rad-dec-2, bgc);
    lcd->fillCircle(midx-sw, y1+rad, rad-dec-2, bgc);
    lcd->fillRect(midx-sw, y1+dec+2, sw, dy-2*dec-4, bgc);

    lcd->fillCircle(midx-sw, y1+rad, rad-decx, fgc);
    }
  lcd->setFont(font);
  lcd->setCursor(x1+3, centerWriteY);
  lcd->setTextColor(fgc);
  int i=0;
  while(i<size)
  {
    lcd->write(*(string+i));
    ++i;
  }
}

void Slider::toggle()
{
  int start = midx-sw;
  int end   = midx;
  uint32_t t=micros();
  tg=!tg;
  if(tg)
  {
    // lcd->fillCircle(midx,    y1+rad, rad-dec, fgc);
    // lcd->fillCircle(midx-sw, y1+rad, rad-dec, fgc);
    // lcd->fillRect(midx-sw, y1+dec, sw, dy-2*dec, fgc);
    for(int i=start,j=-1; i<=end; i+=2)
    {
      if(j!=-1)
        lcd->fillCircle(j, y1+rad, rad-decx+4, fgc);
      lcd->fillCircle(i, y1+rad, rad-decx+4, bgc);
      lcd->fillCircle(i, y1+rad, rad-decx, fgc);
      delay(10);
      j=i;
    }    
  }
  else
  {
    // lcd->fillCircle(midx,    y1+rad, rad-dec, fgc);
    // lcd->fillCircle(midx-sw, y1+rad, rad-dec, fgc);
    // lcd->fillRect(midx-sw, y1+dec, sw, dy-2*dec, fgc);
    // lcd->fillCircle(midx,    y1+rad, rad-dec-2, bgc);
    // lcd->fillCircle(midx-sw, y1+rad, rad-dec-2, bgc);
    // lcd->fillRect(midx-sw, y1+dec+2, sw, dy-2*dec-4, bgc);

    for(int i=end, j=-1; i>=start; i-=2)
    {
      if(j!=-1)
        lcd->fillCircle(j, y1+rad, rad-decx, bgc);
      lcd->fillCircle(i, y1+rad, rad-decx+4, bgc);
      lcd->fillCircle(i, y1+rad, rad-decx, fgc);
      delay(10);
      j=i;
    }  
  }
  t=micros()-t;
  double tt = t;
  Serial.println(tt/1000); 
}