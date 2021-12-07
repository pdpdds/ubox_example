#include "util.h"
#include "ubox.h"

void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}

void RenderTile(int x, int y, int tileNum)
{

    ubox_put_tile(x, y, tileNum);
}

