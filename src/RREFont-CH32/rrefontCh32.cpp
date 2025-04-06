#include "rrefontCh32.h"
#include <string.h>
#include <RREFont.h>
#include <epaper.h>

static uint8_t charBuffer[DISPLAY_WIDTH] = {0xff};

static uint8_t fontSubpart = 0;
static uint16_t lineYStart = 0;

/**
 * Fonts are painted in parts of 8 lines height (due to eInk display memory layout).
 * If a font is 16 lines height then it is necessary to paint the font two times. 
 * First pass for lines 1-8 and a second pass for lines 9-16.
 * If there are chars in red then another passes are necessary to fill in diplay memory for red pixels.
 * 
 * In the implementation at first we clip rectangle to the appropriate height range.
 * Later we perform filling pixels according to the device memory model. 
 * In this memory model data starts at (0,0) in the right top corner (we need it on the left so we need to invert it). Data are kept by 8 pixels in a single byte for vertical blocks.
 */
static void fillRect(int x, int inputY, int w, int inputH, int c)
{

    int ymin = fontSubpart * SINGLE_LINE_HEIGHT;
    int ymax = ymin + SINGLE_LINE_HEIGHT - 1;

    int y = inputY - lineYStart;
    int h = inputH;

    // Clip the top edge
    if (y < ymin)
    {
        int difference = ymin - y;
        y = ymin;
        h -= difference;
        if (h < 0)
        {
            h = 0;
        }
    }
    if (h)
    {
        // Clip the bottom edge
        int bottomEdge = y + h - 1;
        if (bottomEdge > ymax)
        {
            int difference = bottomEdge - ymax;
            h -= difference;
            if (h < 0)
            {
                h = 0;
            }
        }
    }

    if (!h)
    {
        return;
    }

    uint16_t startY = y % SINGLE_LINE_HEIGHT;
    int realStartX = DISPLAY_WIDTH - 1 - x;
    for (uint16_t posY = startY; posY < (startY + h); ++posY)
    {
        for (int posX = realStartX - w + 1; posX <= realStartX; ++posX)
        {
            uint8_t bit = (7 - (posY % 8));
            if (c)
            {
                charBuffer[posX] |= (c << bit);
            }
            else
            {
                charBuffer[posX] &= (~(((uint8_t)1) << bit));
            }
        }
    }
}


/**
 * Stores current buffer of 8 pixels height into display memory.
 */
static void flushLine(int lineNo, bool redColor)
{
    epd_address_set(lineNo * SINGLE_LINE_HEIGHT, 249, lineNo * SINGLE_LINE_HEIGHT, 0); // rotate
    epd_setpos(lineNo * SINGLE_LINE_HEIGHT, 0);
    if(redColor) {
        epd_write_to_ram_red(charBuffer, sizeof(charBuffer));
    } else {
        epd_write_to_ram_blackWhite(charBuffer, sizeof(charBuffer));  
    }
}

/**
 * Renders single pass of a string line.
 */
int 
RRFontCH32::printStr(int xpos, int ypos, const char *str, const char *redColMap, bool redColorFlow)
{
  int x = xpos;
  int y = ypos;

  memset(charBuffer, redColorFlow ? 0 : 0xFF, sizeof(charBuffer));

  while(*str) {
    char ch = *str++;
    int wd = rreFont_.charWidthFull(ch);
    bool isRedChar = redColMap && ch == *redColMap++;
    if(redColorFlow == isRedChar) {
        rreFont_.drawChar(x,y,ch);
    }
    x+=wd;
  }
  return x;
}


void 
RRFontCH32::printLine(int posX, int lineNumber, const char *str, const char *redColorMap)
{
    lineYStart = lineNumber * SINGLE_LINE_HEIGHT;
    for(fontSubpart = 0; fontSubpart<(FONT_HEIGHT/SINGLE_LINE_HEIGHT); ++fontSubpart) {
        rreFont_.setColor(0, 1);
        printStr(posX, lineYStart, str, redColorMap, false);
        flushLine(fontSubpart + lineNumber, false);        
        if(redColorMap) {
            rreFont_.setColor(1, 0);
            printStr(posX, lineYStart, str, redColorMap, true);
        } else {
            memset(charBuffer, 0, sizeof(charBuffer));
        }
        flushLine(fontSubpart + lineNumber, true);
    }
}

RRFontCH32::RRFontCH32(RREFont &rreFont)
: rreFont_(rreFont)
{
    rreFont_.init(fillRect, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}
