#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801A79C.
 * sub_0801A79C @ 0x0801A79C
 */

/* FIVE parameters -- r0-r3 plus `ldr r5, [sp, #0xc]` past a three-register
 * push -- and void. It parks all five in the 0x0200CCxx block and then runs the
 * link scan. The fourth is stored with `strb`, but that is the u8 global's
 * width and not the parameter's: `int` keeps the definition compatible with the
 * prototype-less `void InitSaveSystem();` the header already carries. */
void InitSaveSystem(int a, int b, int c, int d, int e)
{
    gUnknown_0200CC24 = a;
    gUnknown_0200CC28 = b;
    gUnknown_0200CC2C = c;
    gUnknown_0200CC30 = d;
    gUnknown_0200CC34 = e;

    DetectFlash();
    ScanSaveSectors(0);
}
asm(".global sub_0801A79C\n.thumb_set sub_0801A79C, InitSaveSystem\n");
