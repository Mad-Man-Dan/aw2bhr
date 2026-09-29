#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806E740.
 * sub_0806E740 @ 0x0806E740, sub_0806E780 @ 0x0806E780, sub_0806E7C0 @ 0x0806E7C0, sub_0806E7FC @ 0x0806E7FC
 */

#include "proc.h"
/* A 16-frame scroll step. Interpolate's result feeds SetBgScrollShadow directly --
 * `adds r1, r0, #0` then `lsls #0x10; lsrs #0x10` is copy-then-narrow into that
 * callee's u16 second parameter, not evidence about Interpolate, which is
 * declared s32.
 *
 * `bls` and not `ble` is the one type fact here: the frame counter is compared
 * UNSIGNED, so unk38 is u32 where its neighbours unk2c and unk30 are signed
 * words. Both tail arms join, so the out-of-line arm is the ELSE and the
 * condition is `> 0x10`. */
struct Unk6E740Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ STRUCT_PAD(0x34, 0x38);
    /* 38 */ u32 unk38;
};
/* Redraws a 16x16 window whose palette row is 0x10 when the proc's flag is set
 * and 0 when it is clear. The branchless `rsbs; orrs; asrs #0x1f` is `x != 0`
 * widened to a mask -- see docs/agbcc-codegen.md -- and the `ands` applies it.
 *
 * THE CONSTANT MUST BE ITS OWN STATEMENT. `movs r1, #0x10` lands BEFORE the
 * field load in the ROM, and no single-expression spelling puts it there:
 * `0x10 & -(x != 0)`, `(x != 0) ? 0x10 : 0`, `(x != 0) * 0x10` and the explicit
 * `((x | -x) >> 31) & 0x10` all fold to the same tree with the constant as the
 * second operand, so it is materialised last, into the register the field load
 * just freed. Initialising a local to the constant and then compound-assigning
 * creates the constant's pseudo first, and the whole 64 bytes fall out. The
 * local does not survive -- gcc still folds the value -- but the RTL creation
 * ORDER is what the emitted code follows. */
struct Unk6E780Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ s32 unk34;
};
/* Starts the scroll proc SoundRoomTitleScroll_Loop steps -- same four word fields -- and
 * seeds its start and end positions from two caller arguments.
 *
 * `b & 1` is computed ONCE and reused three times (stored to unk34, shifted by
 * 7 for the offset, shifted by 5 for SoundRoomDrawTrackTitle's argument), which is what
 * keeps it in r1 across the whole tail. unk30 is derived from unk2c rather than
 * recomputed: the ROM subtracts `a << 7` from the value still in r2, so the two
 * fields are one expression apart and the source says so. */
struct Unk6E7C0Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x2c);
    /* 2c */ s32 unk2c;
    /* 30 */ s32 unk30;
    /* 34 */ s32 unk34;
    /* 38 */ s32 unk38;
};
#include "hardware.h"

void SoundRoomTitleScroll_Loop(struct Unk6E740Proc *proc)
{
    SetBgScrollShadow(0, Interpolate(4, proc->unk2c, proc->unk30, proc->unk38, 0x10), 0);

    if (proc->unk38 > 0x10)
        Proc_Break(proc);
    else
        proc->unk38++;
}
asm(".global sub_0806E740\n.thumb_set sub_0806E740, SoundRoomTitleScroll_Loop\n");

void SoundRoomTitleScroll_OnScrollEnd(struct Unk6E780Proc *proc)
{
    int pal = 0x10;

    pal &= -(proc->unk34 != 0);

    PlayMusicOrSfx2(0x67);
    FillTilemapRect(gBG0TilemapBuffer, pal, 0x10, 0x10, 2, 0);
    BG_EnableSyncBG0();
}
asm(".global sub_0806E780\n.thumb_set sub_0806E780, SoundRoomTitleScroll_OnScrollEnd\n");

void StartSoundRoomTitleScroll(int a, int b, ProcPtr parent)
{
    struct Unk6E7C0Proc *proc = Proc_StartBlocking(gUnknown_08582BFC, parent);

    proc->unk34 = b & 1;
    proc->unk2c = -0x38 - ((b & 1) << 7);
    proc->unk30 = proc->unk2c - (a << 7);
    proc->unk38 = 0;
    SoundRoomDrawTrackTitle(b, ((b & 1) << 5) + 0x40);
}
asm(".global sub_0806E7C0\n.thumb_set sub_0806E7C0, StartSoundRoomTitleScroll\n");

/* Pokes one palette entry from a ROM table keyed on the frame counter, then
 * flushes.
 *
 * `lsrs #1; lsls #1` is NOT a mask clearing bit 0 -- counted as
 * `(u32)x >> 1 << 1` it is the /2 fused with the u16 array's own scale, i.e.
 * the byte offset of element x/2, so the table is indexed at half rate and the
 * animation advances every other frame. The `(u32)` cast is load-bearing:
 * gGameClock is `s32`, and without it the divide is `asrs`.
 *
 * THE TABLE BASE MUST BE BOUND TO A LOCAL, and this was the last two bytes.
 * The ROM's literal pool is ordered (gUnknown_081A47E4, gPal,
 * gGameClock); the natural one-statement spelling emits (gPal,
 * gUnknown_081A47E4, gGameClock) instead, because agbcc's
 * expand_assignment computes the DESTINATION address before the source, so the
 * LHS array's pool word is always created first. Binding the source table's
 * BASE -- not its value, and not the element's address -- moves that one
 * reference into an earlier statement and leaves the rest of the second
 * statement's order untouched, which is the only arrangement that puts gPal in
 * the middle slot. */
void SoundRoomCycleArrowPalette(void)
{
    const u16 *tbl = gUnknown_081A47E4;

    gPal[0x1EC] = tbl[((u32)gGameClock & 0x1F) / 2];
    EnablePaletteSync();
}
asm(".global sub_0806E7FC\n.thumb_set sub_0806E7FC, SoundRoomCycleArrowPalette\n");
