#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080411A0.
 * sub_080411A0 @ 0x080411A0
 */

#include "proc.h"
/* The proc the gUnknown_0849FD44 script runs. +0x34 is a AP_Create handle
 * (it goes straight to AP_SwitchAnimation's first parameter) and +0x42 / +0x46 are
 * halfwords the 0x080412A4 / 0x080412F4 / 0x0804134C handlers drive. */
struct Unk411A0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ struct Unk0801C210 *unk34;
};

/* Three statements, not two: Proc_StartBlocking's result is dead (r0 is
 * reloaded by the pool `ldr` for the next call) and PlayMusicOrSfx2 takes no
 * argument from it. */
void CaptureAnim_StartShake(ProcPtr parent)
{
    AP_SwitchAnimation(((struct Unk411A0Proc *)Proc_Find(gUnknown_0849FD44))->unk34, 1);
    Proc_StartBlocking(gUnknown_0849FE0C, parent);
    PlayMusicOrSfx2(0x6D);
}
asm(".global sub_080411A0\n.thumb_set sub_080411A0, CaptureAnim_StartShake\n");
