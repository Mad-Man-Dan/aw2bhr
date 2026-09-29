#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08078250.
 * sub_08078250 @ 0x08078250, sub_08078260 @ 0x08078260, sub_08078270 @ 0x08078270, sub_080782C0 @ 0x080782C0, sub_0807831C @ 0x0807831C, sub_08078358 @ 0x08078358, sub_080783BC @ 0x080783BC, sub_08078404 @ 0x08078404, sub_08078420 @ 0x08078420
 */

#include "proc.h"
/* The predicate WorldMapReturn_RevealMissions guards its StartWorldMapMarkerReveal call with. It counts how
 * many entries of a -1-terminated SIGNED byte list (`ldrsb` off a zero index
 * register, which is what makes the elements `s8` and not `u8`) have bit 1 set
 * in gUnknown_0202FDFC.unk12, then compares that count against the record's own
 * two bytes at +4 and +5. Hitting the second threshold also writes 3 to the
 * caller's proc at +0x58 -- which is why WorldMapReturn_RevealMissions re-reads that field after
 * the call rather than reusing the 0 it just stored.
 *
 * `gUnknown_0202FE0E` in the disassembly is `&gUnknown_0202FDFC.unk12`; see the
 * comment on that struct in include/unknown-globals.h for why the two are one
 * object. The candidate emits the same address as `gUnknown_0202FDFC+0x12`.
 *
 * The parameter is typed rather than assigned to a local inside the body: a
 * local puts `adds r3, r0, #0` in the function body AFTER `mov ip, r1`, where
 * the ROM has both as prologue parameter copies in register order. Same for
 * `ids` and `count` -- initialising `ids` first is what puts `ldr r2, [r3]`
 * ahead of `movs r4, #0`. */

/* A proc method of the same 0x54/0x58 shape as src/decomp/c_08078480.c's, one
 * field longer. WorldMapReturn_CheckRevealCondition is a u8 predicate -- the ROM tests its result with
 * a BARE `lsls #0x18` (a truth test, not a value kept) -- and it also writes
 * this proc's own +0x58 to 3 on one of its paths, which is why the proc pointer
 * is its second argument and why +0x58 is re-read after the call rather than
 * being a dead store.
 *
 * `lsls #3` on the index is the 8-byte stride of gUnknown_0861500C; see the
 * struct's comment in include/unknown-globals.h for why word 0 is the
 * `const u8 *` id list StartWorldMapMarkerReveal takes. */

struct Unk807831C
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ void *unk_54;
    /* 0x58 */ s32 unk_58;
};

/* Fills four halfwords with a DESCENDING run: p[3] = 0x2c down to p[0] = 0x29.
 *
 * Two facts pin the loop shape, and the obvious indexed spelling reproduces
 * neither. (1) There is no zero-trip guard, so the loop is a `do`/`while`, or
 * an indexed `for` whose bound the compiler can prove statically. (2) The exit
 * test compares the walking POINTER against the incoming one with a SIGNED
 * `bge`, and agbcc compares pointers UNSIGNED (`bcs`) -- a signed compare of
 * two pointers can only come from a signed comparison, which is why the cast
 * is written out.
 *
 * `for (i = 3; i >= 0; i--) p[i] = i + 0x29;` is the spelling that reads like
 * the original and it does not match: agbcc keeps the index alive, compares it
 * against 0, and rematerialises `i + 0x29` with two instructions per iteration
 * (20 bytes against the ROM's 16). It declines both the biv elimination that
 * would move the test onto the pointer and the induction variable for the
 * stored value. Spelling both induction variables out is what produces the
 * ROM. Declaration order is load-bearing too: `v` before `q` puts
 * `movs r2, #0x2c` ahead of `adds r1, r0, #6`, as in the ROM. */

void sub_08078250(u16 *p)
{
    s32 v = 0x2c;
    u16 *q = p + 3;

    do
    {
        *q = v;
        v--;
        q--;
    } while ((s32)q >= (s32)p);
}

/* sub_08078250's twin one run along: the same four halfwords, 0x31 down to
 * 0x2e. See the comment on sub_08078250 for why the loop is written with both
 * induction variables spelled out and a signed pointer comparison. */

void sub_08078260(u16 *p)
{
    s32 v = 0x31;
    u16 *q = p + 3;

    do
    {
        *q = v;
        v--;
        q--;
    } while ((s32)q >= (s32)p);
}

/* A four-way dispatch on gUnknown_0202FDFC.unk0c, the table index named in
 * that struct's comment. The compare tree is agbcc's binary search over a
 * `switch`, not a chain of `if`s: it tests 0xf for equality, then 0xf again
 * with `bgt` to split the range, and only then 7 on the low side and
 * 0x17 / 0x29 on the high side. A written-out `if`/`else if` chain would have
 * tested the four values in source order.
 *
 * StartWorldMapMissionClear takes the same field as its first argument and forwards the
 * proc as its second -- invisible in that callee's body, which never reads r1,
 * but it survives into its `bl Proc_StartBlocking` as the parent. */

void WorldMapReturn_Init(ProcPtr proc)
{
    StartWorldMapMissionClear(gUnknown_0202FDFC.unk0c, proc);

    switch (gUnknown_0202FDFC.unk0c)
    {
    case 7:
    case 0x29:
        Proc_Goto(proc, 0);
        break;

    case 0xf:
    case 0x17:
        Proc_Goto(proc, 1);
        break;

    default:
        Proc_Goto(proc, 2);
        break;
    }
}
asm(".global sub_08078270\n.thumb_set sub_08078270, WorldMapReturn_Init\n");

u8 WorldMapReturn_CheckRevealCondition(struct Unk80782C0 *a, ProcPtr proc)
{
    const s8 *ids = a->unk_00;
    s32 count = 0;

    while (*ids != -1)
    {
        if (gUnknown_0202FDFC.unk12[*ids] & 2)
            count++;

        ids++;
    }

    if (count == a->unk_04)
        return 1;

    if (count == a->unk_05)
    {
        ((struct Unk807831C *)proc)->unk_58 = 3;
        return 1;
    }

    return 0;
}
asm(".global sub_080782C0\n.thumb_set sub_080782C0, WorldMapReturn_CheckRevealCondition\n");

void WorldMapReturn_RevealMissions(struct Unk807831C *proc)
{
    proc->unk_58 = 0;

    if (WorldMapReturn_CheckRevealCondition(proc->unk_54, proc))
        StartWorldMapMarkerReveal(gUnknown_0861500C[gUnknown_0202FDFC.unk0c + proc->unk_58].unk_00, proc);
}
asm(".global sub_0807831C\n.thumb_set sub_0807831C, WorldMapReturn_RevealMissions\n");

/* Walks the four records of gUnknown_08615974 and starts the ones whose two
 * tags pass and whose id is not already flagged. Three guards, all in one
 * `&&` chain: the two `cmp r0, #0` tests after the IsCampaignCompletionFlagSet calls jump to
 * the SAME label as the `& 3` test, which is short-circuiting, not three
 * separate `if`s round one body.
 *
 * The record's last two bytes are both an id (read `ldrsb` to index
 * gUnknown_0202FDFC.unk12) and the 0xFF-terminated list StartWorldMapMarkerReveal takes --
 * which is why the ROM computes that one address TWICE, once as `[r4, #2]` off
 * the walking pointer for the value and once as `r5 + r8` for the call. Two
 * expressions in the source, not one CSE that failed.
 *
 * `i <= 3` rather than `i < 4`: both give `cmp #3; ble`, so this is not
 * evidence either way. */

void WorldMapReturn_RevealBonusMissions(ProcPtr proc)
{
    s32 i;

    for (i = 0; i <= 3; i++)
    {
        if (IsCampaignCompletionFlagSet(gUnknown_08615974[i].unk_00)
         && !IsCampaignCompletionFlagSet(gUnknown_08615974[i].unk_01)
         && !(gUnknown_0202FDFC.unk12[gUnknown_08615974[i].unk_02[0]] & 3))
            StartWorldMapMarkerReveal((const u8 *)gUnknown_08615974[i].unk_02, proc);
    }
}
asm(".global sub_08078358\n.thumb_set sub_08078358, WorldMapReturn_RevealBonusMissions\n");

/* The guarded member of the StartWorldMapScene wrapper trio (the other two are
 * sub_08078404 and sub_08078420, which are unconditional). Both guards test
 * bit 1 of a byte of gUnknown_0202FDFC.unk12[], at +0x21 and +0x29 -- eight
 * bytes apart, which hints at a record stride inside that 42-byte blob but is
 * not enough on its own to give it a type, so the honest spelling is the array
 * index. The separate `adds r0, r2, #0; adds r0, #0x21` address computations
 * are forced: `ldrb`'s immediate offset only reaches 31.
 *
 * `movs r0, #0xb0; lsls r0, r0, #1` is a plain 0x160, NOT wave 23's named
 * constant local: the shift is minimal for that value (0x160 needs 9 bits) and
 * the `movs` and the `lsls` write the same register. */

void WorldMapReturn_ShowSceneIfMissions0F17Cleared(ProcPtr proc)
{
    if ((gUnknown_0202FDFC.unk12[0x0f] & 2) && (gUnknown_0202FDFC.unk12[0x17] & 2))
        StartWorldMapScene(0x160, 0xa0, 2, gUnknown_084BA210, proc);
}
asm(".global sub_080783BC\n.thumb_set sub_080783BC, WorldMapReturn_ShowSceneIfMissions0F17Cleared\n");

/* A five-argument forwarder to StartWorldMapScene; the incoming ProcPtr becomes the
 * fifth argument and so is `str r0, [sp]`'d before r0 is reloaded with the
 * first constant. The bare prologue is what fixes the three leading arguments
 * as wide -- see the prototype's comment in include/unknown-functions.h. */

void sub_08078404(ProcPtr proc)
{
    StartWorldMapScene(0x98, 0xa0, 1, gUnknown_084B9F00, proc);
}

/* sub_08078404's twin with different constants and a different blob.
 * `movs r1, #0x80; lsls r1, r1, #1` then `adds r0, r1, #0` is an ordinary
 * 0x100 immediate synthesis -- minimal shift, and the `movs`/`lsls` pair
 * writes one register -- with the extra copy coming from argument-setup
 * ordering, not from a named constant local. */

void sub_08078420(ProcPtr proc)
{
    StartWorldMapScene(0x100, 0x70, 3, gUnknown_084BA480, proc);
}
