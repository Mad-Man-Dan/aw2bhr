#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08040C88.
 * sub_08040C88 @ 0x08040C88
 */

/* The teardown for the proc the matched CaptureAnimSprites_Init sets up: it frees the
 * three animation handles in the order 0x2c, 0x34, 0x38. The struct is the
 * same one src/decomp/c_08040CA4.c names Unk08040CA4; only the three handles
 * are needed here. */
struct Unk40C88Proc
{
    /* 00 */ u8 filler_00[0x2c];
    /* 2c */ struct Unk0801C210 *unk2c;
    /* 30 */ u8 filler_30[0x04];
    /* 34 */ struct Unk0801C210 *unk34;
    /* 38 */ struct Unk0801C210 *unk38;
};

void CaptureAnimSprites_OnEnd(struct Unk40C88Proc *proc)
{
    AP_Delete(proc->unk2c);
    AP_Delete(proc->unk34);
    AP_Delete(proc->unk38);
}
asm(".global sub_08040C88\n.thumb_set sub_08040C88, CaptureAnimSprites_OnEnd\n");
