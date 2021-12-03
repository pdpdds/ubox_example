#include <stdint.h>

#include "ubox.h"

#include "helpers.h"

/**
 * Put a zero terminated string on the screen using tiles.
 *
 * The font starts on the tileset on tile 128 and the fist char in our has
 * ASCII value 31, so it is adjusted so we can use ASCII *uppercase* directly
 * in our C code.
 */
void put_text(uint8_t x, uint8_t y, const uint8_t *text)
{
    while (*text)
        ubox_put_tile(x++, y, *text++ + 128 - 31);
}
