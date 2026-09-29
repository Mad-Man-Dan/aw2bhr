#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0802C390.
 * sub_0802C390 @ 0x0802C390
 */

#include "hardware.h"
/* Not a Proc: +0x1e lands inside PROC_HEADER's proc_next (0x1c..0x1f) and
 * src/proc.c pins that layout, so the prefix is filler -- the same reading
 * src/decomp/c_0802C480.c makes of its +0x20.
 *
 * unk1e is SIGNED: the publish is `asrs #0x11` off the `lsls #0x10`, an
 * arithmetic shift, so the source is `>> 1` on an s16 and not `/ 2` (which
 * would carry a rounding correction) nor a u16 (`lsrs`).
 *
 * There is NO local holding the loaded halfword. Binding one is 4 bytes and the
 * wrong allocation: with a local, agbcc keeps the sign-extended value and the
 * pointer in scratch registers and never touches r4, and the `+ 1` is computed
 * off the sign-extended copy. The ROM instead parks the POINTER in the
 * callee-saved r4 across three references to proc->unk1e and adds 1 to the raw
 * `ldrh`, which is the shape of three separate reads of the member -- the
 * "binding locals are punctuation" rule of docs/agbcc-codegen.md read
 * backwards. It also fixes the pool-word order: without the local the address
 * of gUnknown_03001FFC is the first pseudo created, so its `ldr` precedes the
 * `ldrh`, which is exactly the ROM's order.
 *
 * gDispIo byte 1 bit 0 is DISPCNT bit 8 = disp_ct.bg0_enable; OpenMinimap in
 * this block is the clearing half, which is what discriminates the bitfield
 * from a byte-wide `|= 1` on an opaque u8.
 *
 * gUnknown_03001FBC is s16 and ClearSlotScriptCallback takes u8, so the `ldrb` on it is
 * the parameter narrowing and not evidence about the global.
 */

struct Unk2C390Proc
{
    /* 0x00 */ u8 filler_00[0x1e];
    /* 0x1e */ s16 unk1e;
};

void MinimapScreen_FadeIn(struct Unk2C390Proc *proc)
{
    gUnknown_03001FFC = proc->unk1e >> 1;

    if (proc->unk1e > 0xb)
    {
        gDispIo.disp_ct.bg0_enable = 1;
        ClearSlotScriptCallback(gUnknown_03001FBC);
    }
    else
    {
        proc->unk1e = proc->unk1e + 1;
    }
}
asm(".global sub_0802C390\n.thumb_set sub_0802C390, MinimapScreen_FadeIn\n");
