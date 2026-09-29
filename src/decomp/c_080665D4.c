#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080665D4.
 * sub_080665D4 @ 0x080665D4, sub_0806666C @ 0x0806666C, sub_08066710 @ 0x08066710
 */

#include "hardware.h"
/* Arms the "wait then dismiss" countdown at +0x26 (the same field
 * src/decomp/c_0806675C.c decrements) and resets the per-slot state: every live
 * gUnknown_08580934->unk70[] mark back to 0, the +0x31 sub-mode flag cleared,
 * and the +0x2d counter bumped -- the same `unk2d++` opener five other 0x0806D
 * handlers use.
 *
 * The ROM reaches gUnknown_08580934 through a ROM word at 0x0816E14C rather
 * than naming it, i.e. `ldr =0x0816E14C; ldr; ldr` where MatchSetupHandleColumnReadyKeys next door
 * has a plain `ldr =gUnknown_08580934; ldr`. That is agbcc's own -fforce-addr
 * address-constant pool, not a global: this function reads the symbol four
 * times, which is the threshold at which the address gets parked instead of
 * rematerialised. Written the honest way -- name the global -- and the build
 * places the .rodata word itself. No gUnknown_0816E14C declaration is needed or
 * wanted. */

struct Unk66710Proc
{
    /* 0x00 */ u8 filler_00[0x26];
    /* 0x26 */ s16 unk26;
};

/* Scans the live slots for one the player has just acted on: on A (0x1) over a
 * slot whose unk70[] mark is 0 it installs a cursor at the slot object's
 * +0x28/+0x2a position offset by (8, 0x10); on B (0x2) over a mark of 1 it
 * tears the slot's node down. Either way it plays a sound and RETURNS -- the
 * loop only continues while nothing has happened. MatchSetupHandleBadgeReadyKeys is the same
 * sweep over unk44[] instead of unk34[].
 *
 * unk70[] is declared `u8` in include/unknown-globals.h but is read here with
 * `ldrb; lsls #24; asrs #24`, i.e. SIGNED, and the writers agree it is: 0xFF
 * (src/decomp/c_08066580.c), 1 (c_08066374.c) and 0 (MatchSetupConfirm_Init) are 0, 1 and
 * -1 as s8. The cast is at the use rather than on the member: two other
 * promoted files read the same member zero-extended and retyping it would
 * change their bytes. Recorded in the header, not acted on.
 *
 * The key-state base is bound INSIDE the loop body, which is what puts its pool
 * `ldr` in the preheader after the guard where the ROM has it. A function-scope
 * binding is a source statement and lands BEFORE the `i = 0` init, which also
 * swaps the two pool words; the cast-subscript spelling with no binding at all
 * emits the pool word TWICE (once per if-arm) and an `add` per use, because
 * with only two references in two basic blocks CSE will not hold the address.
 * Block scope makes it an LICM hoist instead, and LICM hoists land exactly
 * there. Same lever as src/decomp/c_08064410.c's, read the other way round. */
void MatchSetupHandleColumnReadyKeys(void)
{
    int i;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        struct KeySt *keys = (struct KeySt *)&gUnknown_03002040;

        if ((s8)gUnknown_08580934->unk70[i] == 0 && (keys[i].pressed & 1))
        {
            struct Unk08580934_Obj *obj = gUnknown_08580934->unk34[i];
            int x = obj->unk28;
            int y = obj->unk2a;

            StartReadyMarker(i, x + 8, y + 0x10);
            PlayMusicOrSfx2(0x71);
            return;
        }

        if ((s8)gUnknown_08580934->unk70[i] == 1 && (keys[i].pressed & 2))
        {
            DismissReadyMarker(i);
            PlayMusicOrSfx2(0x66);
            return;
        }
    }
}
asm(".global sub_080665D4\n.thumb_set sub_080665D4, MatchSetupHandleColumnReadyKeys\n");

/* MatchSetupHandleColumnReadyKeys's twin, and every note on that function applies here. The two
 * differences are real: the slot object comes from unk44[] rather than unk34[],
 * and the cursor position is offset by (1, 8) plus `unk11[i] * 4` on the Y --
 * unk11[] being the per-slot byte table that unk44[] itself is indexed by, so
 * the extra term is "one row per sub-entry of this slot".
 *
 * The two position reads are bound to locals before the call. Written inline as
 * `StartReadyMarker(i, obj->unk28 + 1, ...)` agbcc interleaves each load with its
 * own constant add; the ROM does both `ldrsh`s first and then both adds, which
 * is what separate bindings produce. The `+ 8` on Y is likewise outside the
 * binding, because the ROM's `adds r2, #8` comes after `adds r1, #1`. */
void MatchSetupHandleBadgeReadyKeys(void)
{
    int i;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        struct KeySt *keys = (struct KeySt *)&gUnknown_03002040;

        if ((s8)gUnknown_08580934->unk70[i] == 0 && (keys[i].pressed & 1))
        {
            struct Unk08580934_Obj *obj = gUnknown_08580934->unk44[i];
            int x = obj->unk28;
            int y = obj->unk2a + (gUnknown_08580934->unk11[i] << 2);

            StartReadyMarker(i, x + 1, y + 8);
            PlayMusicOrSfx2(0x71);
            return;
        }

        if ((s8)gUnknown_08580934->unk70[i] == 1 && (keys[i].pressed & 2))
        {
            DismissReadyMarker(i);
            PlayMusicOrSfx2(0x66);
            return;
        }
    }
}
asm(".global sub_0806666C\n.thumb_set sub_0806666C, MatchSetupHandleBadgeReadyKeys\n");

void MatchSetupConfirm_Init(struct Unk66710Proc *proc)
{
    int i;

    for (i = 0; i < gUnknown_08580934->unk08; i++)
    {
        gUnknown_08580934->unk70[i] = 0;
    }

    proc->unk26 = 0x24;
    gUnknown_08580934->unk31 = 0;
    gUnknown_08580934->unk2d++;
}
asm(".global sub_08066710\n.thumb_set sub_08066710, MatchSetupConfirm_Init\n");
