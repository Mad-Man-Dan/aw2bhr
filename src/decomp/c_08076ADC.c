#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08076ADC.
 * sub_08076ADC @ 0x08076ADC
 */

#include "proc.h"

/* gUnknown_08614280 is a POINTER, not the buffer: every use is `ldr rN, =sym;
 * ldr rN, [rN]`. The ROM keeps the ADDRESS in r4 across the intervening calls
 * and re-`ldr`s the value, which is what a non-const pointer global does.
 * `movs r2, #0x80; lsls r2, #5` is the constant 0x1000. */
void sub_08076ADC(ProcPtr proc)
{
    sub_08076888(proc);
    Decompress(gUnknown_081D0BAC, gUnknown_08614280);
    sub_08076858();
    sub_0807681C();
    RegisterDataMove(gUnknown_08614280, (void *)0x0600F000, 0x1000);
    sub_08076C8C(proc);
    StartWorldMapNationPanel(proc);
}
