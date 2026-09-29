#include "global.h"
#include "map.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08023274.
 * sub_08023274 @ 0x08023274, sub_080232CC @ 0x080232CC
 */

void StepMapCursorAndDraw(int a1)
{
    s16 x;
    s16 y;

    StepMapCursorDisplayToward(gUnknown_030033E4.unk00 << 4, gUnknown_030033E4.unk02 << 4, a1, &x, &y);
    DrawMapCursorSprite(x - gMap->scrollX, y - gMap->scrollY, (s16)a1);
}
asm(".global sub_08023274\n.thumb_set sub_08023274, StepMapCursorAndDraw\n");

void StepMapCursorAndDrawTwo(int a1, int a2)
{
    s16 x;
    s16 y;

    StepMapCursorDisplayToward(gUnknown_030033E4.unk00 << 4, gUnknown_030033E4.unk02 << 4, a1, &x, &y);
    DrawMapCursorSprite(x - gMap->scrollX, y - gMap->scrollY, a1);
    DrawMapCursorSprite(x - gMap->scrollX, y - gMap->scrollY, a2);
}
asm(".global sub_080232CC\n.thumb_set sub_080232CC, StepMapCursorAndDrawTwo\n");
