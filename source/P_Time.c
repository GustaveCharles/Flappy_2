#include "P_Time.h"
#include "numbers.h"

// void printDigit(u16 *map, int number, int x, int y){
//     	map[1+(17+x)*32 + y] = numbersMap[(10 - number)*2] | TILE_PALETTE(9);
// 		map[1+(17+x)*32+y+1] = numbersMap[(10 - number)*2+1] | TILE_PALETTE(9);
// }

void printDigit(u16 *map, int number, int x, int y)
{
        BG_MAP_RAM_SUB(24)[12 + y * 32 + x] = numbersMap[number*2] | TILE_PALETTE(9);
        BG_MAP_RAM_SUB(24)[12 + (y +1)* 32 +x] = numbersMap[number*2+1] | TILE_PALETTE(9);
}


void updateChronoDisp(u16*map, int min, int sec, int msec)
{

    int x = 2, y = 2;
    int number;

    /*****MINUTES******/
    number = min;
    if (min > 59)
        min = number = -1;
    // First digit
    x = 0;
    if (min >= 0)
        number = min / 10;
    printDigit(map, number, x, y);
    // Second digit
    x = 1;
    if (min >= 0)
        number = min % 10;
    printDigit(map, number, x, y);

    /*****POINTS******/
    x = 3;
    number = 10;
    printDigit(map, number, x, y);

    /*****SECONDS******/
    number = sec;
    if (sec > 59)
        sec = number = -1;
    // First digit
    x = 5;
    if (sec >= 0)
        number = sec / 10;
    printDigit(map, number, x, y);
    // Second digit
    x = 6;
    if (sec >= 0)
        number = sec % 10;
    printDigit(map, number, x, y);
}

void updateChronoDisp_Sub(int min, int sec, int msec)
{
    updateChronoDisp(BG_MAP_RAM_SUB(24), min, sec, msec);
}