#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A444.
 * sub_0801A444 @ 0x0801A444
 */

/* Draws a box into BG map 2 -- gBG2TilemapBuffer is the third of the four
 * buffers DrawWindowBackground recognises, and it is DEREFERENCED here (`ldr r4,[r4]`),
 * so what goes on the stack is the u16 * it holds. The four `lsls #0x10;
 * asrs #0x10` pairs are PROMOTE_MODE on the declared s16 parameters. */
void DrawWindowBackgroundOnBg2(s16 a1, s16 a2, s16 a3, s16 a4)
{
    DrawWindowBackground(a1, a2, a3, a4, gBG2TilemapBuffer, 8);
}
asm(".global sub_0801A444\n.thumb_set sub_0801A444, DrawWindowBackgroundOnBg2\n");
