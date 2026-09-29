#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080145BC.
 * sub_080145BC @ 0x080145BC
 */

void TextBox_OnEnd(void)
{
    if (gUnknown_03002514 == 2)
        gUnknown_03002514 = 0;

    sub_0801537C(gUnknown_0848A398);
    sub_0801537C(gUnknown_0848A3C4);
}
asm(".global sub_080145BC\n.thumb_set sub_080145BC, TextBox_OnEnd\n");
