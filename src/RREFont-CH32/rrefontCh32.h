#ifndef RREFONT_CH32_H
#define RREFONT_CH32_H

#define DISPLAY_WIDTH 250
#define DISPLAY_HEIGHT 122
#define FONT_WIDTH 16
#define FONT_HEIGHT 16
#define SINGLE_LINE_HEIGHT 8

class RREFont;

/**
 *  It is an experimental implementation for RREFont that allows to use the library on ch32v003 device that has quite limited amount of RAM.
 */
class RRFontCH32 {

    RREFont &rreFont_;

public:

    RRFontCH32(RREFont &rreFont);

    /**
     * posX - in pixels
     * lineNumber - it is in the device units. 1 device unit = 8 real lines. E.g. lineNumbe 3 gives starting Y position equals to 24 (3 * 8)
     * str - black string (null terminated) to write.
     * redColorMap - Optional. It specifies which chars should be painted in RED color. If specified it must have exactly same length as str.
     * If a char in the str buffer is the same as a char in redColorMap buffer at the same index then this char is painted in RED colour; otherwise it is painted in black colour.
     * 
     * The current printLine implementation clears the whole physical device line.
     */
    void printLine(int posX, int lineNumber, const char *str, const char *redColorMap);

private:

    int printStr(int xpos, int ypos, const char *str, const char *redColMap, bool redColorFlow);
};

#endif