#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08063BBC.
 * sub_08063BBC @ 0x08063BBC
 */

/* Seeds the counter sub_08063BE0 decrements and runs the first frame at once.
 * `ldrb` off the s16 gUnknown_03001FBC is agbcc narrowing the load to
 * ClearSlotScriptCallback's `u8` parameter, exactly as src/decomp/c_08063BE0.c does. */
void sub_08063BBC(struct Unk8063BE0 *p)
{
    ClearSlotScriptCallback(gUnknown_03001FBC);
    p->unk44 = 8;
    sub_08063BE0(p);
}
