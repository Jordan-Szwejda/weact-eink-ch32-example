#include "Epaper/epaper.h"
#include "Epaper/bmp.h"
#include <RREFont.h>
#include "RREFont-CH32/rrefontCh32.h"
#include <rre_nadine_16x16.h>

static void clearScreen()
{
    uint8_t fillByte = 0xFF;
    epd_write_pattern(&fillByte, 1, 1);
}

RREFont rreFont;
RRFontCH32 rreFontCh32(rreFont);

void printTextOnEInk()
{
    rreFont.setFont(&rre_nadine_16x16);
    clearScreen();
    
    rreFontCh32.printLine(0,0,  "Text example in RED colour",
                                "                RED       ");
    rreFontCh32.printLine(0,2,  "Line only in black colour", nullptr);

    rreFontCh32.printLine(50,4,  "The whole RED line", 
                                "The whole RED line");

    rreFontCh32.printLine(0,13,  "Some text at the bottom", 
                                  "Some text at the       ");    
    epd_update();
}

void initEPaper()
{
    epd_io_init();
    epd_init();
}

int main() {
    initEPaper();
    printTextOnEInk();
    epd_enter_deepsleepmode(EPD_DEEPSLEEP_MODE1);
    while(true);
}