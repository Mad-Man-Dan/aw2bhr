#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080309AC.
 * sub_080309AC @ 0x080309AC, sub_08030B00 @ 0x08030B00
 */

/* MATCHED (wave 49, W49-B). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CE0"]
 * That word is an agbcc -fforce-addr address constant holding
 * &gUnknown_0849B018, NOT a global. Dereferenced in baserom.gba to be sure:
 * 0x08090CB4/CB8/CBC/CE0/CE4/CE8 hold 0849B01C/0849B018/0849B018/0849B018/
 * 0849B018/0849B018 respectively -- six pool words, two objects, zero globals.
 *
 * LinkReceiveBlock is this function's TWIN: byte-identical but for its own pool
 * word (0x08090CE4) and a 128-byte payload where this one copies 20. The same
 * C matched both with only the loop bound changed.
 *
 * THREE things were load-bearing, and each was measured with compile_probe
 * rather than guessed:
 *
 *  1. The loop is a `goto`, not a `while (1)`. A loop statement emits a
 *     NOTE_INSN_LOOP_BEG, LICM then hoists the pool load, 0x1AAE and
 *     &gUnknown_0202575C into callee-saved registers, and CSE folds the hoisted
 *     -fforce-addr load back into a bare symbol -- so the .rodata word
 *     DISAPPEARS and the three-deep `ldr; ldr; ldr` chain collapses to two.
 *     With a goto there is no loop note, nothing is hoisted, and the ROM's
 *     register allocation falls out exactly. The ROM re-materialises 0x1AAE at
 *     THREE separate sites; that is the tell that no LICM ran. Written up in
 *     docs/agbcc-codegen.md under the goto-loop chapter.
 *
 *  2. `gUnknown_0849B018->unk24[...]` needs unk24 to be a real member ARRAY.
 *     The ROM reaches it as `adds rB,rP,#0; adds rB,#0x24; adds rB,rB,rI` --
 *     agbcc's member-array hoist. `(&p->unk24)[i]` and a cast to
 *     `volatile u16 (*)[4]` both fold the 0x24 into the `ldrh` displacement
 *     instead. See the note on the member in unknown-globals.h; the four
 *     halfwords at 0x24..0x2a were re-expressed as the array they are, with no
 *     change of offset, width or qualifier, and the one existing reader
 *     (c_08033638.c) was re-verified as a match.
 *
 *  3. `army` must be bound before the pointer chain. Written as
 *     `return entry->unk01;` at the end, agbcc reloads the byte after the
 *     volatile stores (+4 bytes, 86.8%); bound first, the one `ldrb` serves
 *     both the unk24 index and the return value, which is what the ROM does.
 *
 * unk1aae was carved off the last byte of the old filler_1aae; it is the read
 * cursor of the send ring and volatile on the usual dead-ldrb tell.
 *
 * The predicate's result is narrowed `lsls r0,#0x18` before the test, which
 * argues for a u8/bool8 return, but unknown-functions.h declares
 * `int (*)(u8 *)` to agree with IsLinkCommandIdValid and RemoteTurn_WaitForCommand. The declaration
 * is left alone and the `(u8)` cast carries the narrowing -- see the existing
 * W43-C note recording the same disagreement. */

s16 LinkReceiveCommand(void *a1, int (*fn)(u8 *))
{
    struct Unk08090CD8Entry *entry;
    u8 army;
    u8 i;

again:
    entry = &gUnknown_0849B018->unk12c[32 + gUnknown_0849B018->unk1aae];
    if (entry->unk00 != 0xAF)
        return -1;
    if (entry->unk01 == gUnknown_0849B018->unk06)
        return -1;

    if (entry->unk02 != gUnknown_0849B018->unk24[entry->unk01])
    {
        gUnknown_0202575C.unk00 = 0xAE;
        gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
        gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[entry->unk01];
        sub_0802F588(&gUnknown_0202575C, 4);
        entry->unk00 = 0;
        gUnknown_0849B018->unk1aae++;
        gUnknown_0849B018->unk1aae &= 0xF;
        goto again;
    }

    for (i = 0; i < 20; i++)
        ((u8 *)a1)[i] = entry->unk06[i];

    if (fn != NULL && (u8)fn(a1) == 0)
    {
        gUnknown_0202575C.unk00 = 0xAE;
        gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
        gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[entry->unk01];
        sub_0802F588(&gUnknown_0202575C, 4);
        entry->unk00 = 0;
        gUnknown_0849B018->unk1aae++;
        gUnknown_0849B018->unk1aae &= 0xF;
        goto again;
    }

    entry->unk00 = 0;
    army = entry->unk01;
    gUnknown_0849B018->unk24[army]++;
    gUnknown_0849B018->unk1aae++;
    gUnknown_0849B018->unk1aae &= 0xF;
    return army;
}
asm(".global sub_080309AC\n.thumb_set sub_080309AC, LinkReceiveCommand\n");

/* MATCHED (wave 49, W49-B). Needs its .rodata pool word placed:
 *   "rodata": ["0x08090CE4"]
 * -- an agbcc -fforce-addr word holding &gUnknown_0849B018, confirmed by
 * dereferencing baserom.gba, not a global.
 *
 * LinkReceiveCommand's TWIN. The two functions are the same 340 bytes of the same
 * shape and differ in exactly two places: their private pool word, and the
 * payload length -- 20 there, 128 here. The whole derivation was done once on
 * LinkReceiveCommand and instantiated; see that file for the three load-bearing
 * findings (the goto loop, the unk24 member array, and binding `army` before
 * the pointer chain).
 *
 * The length shows up only in the copy loop's exit test, and the two forms
 * look unrelated until you read them as the same `u8 i; i < N`:
 *   20  -> `lsrs r2,r0,#0x18; cmp r2,#0x13; bls`
 *   128 -> `lsrs r2,r0,#0x18; cmp r0,#0;    bge`
 * At 128 the bound is the sign bit of (i + 1) << 24, so gcc tests the shifted
 * value directly and leaves the `lsrs` un-coalesced. That `bge` is an ASCENDING
 * loop, exactly as c_08030930.c records for the same idiom. */

s16 LinkReceiveBlock(void *a1, int (*fn)(u8 *))
{
    struct Unk08090CD8Entry *entry;
    u8 army;
    u8 i;

again:
    entry = &gUnknown_0849B018->unk12c[32 + gUnknown_0849B018->unk1aae];
    if (entry->unk00 != 0xAF)
        return -1;
    if (entry->unk01 == gUnknown_0849B018->unk06)
        return -1;

    if (entry->unk02 != gUnknown_0849B018->unk24[entry->unk01])
    {
        gUnknown_0202575C.unk00 = 0xAE;
        gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
        gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[entry->unk01];
        sub_0802F588(&gUnknown_0202575C, 4);
        entry->unk00 = 0;
        gUnknown_0849B018->unk1aae++;
        gUnknown_0849B018->unk1aae &= 0xF;
        goto again;
    }

    for (i = 0; i < 128; i++)
        ((u8 *)a1)[i] = entry->unk06[i];

    if (fn != NULL && (u8)fn(a1) == 0)
    {
        gUnknown_0202575C.unk00 = 0xAE;
        gUnknown_0202575C.unk01 = gUnknown_0849B018->unk06;
        gUnknown_0202575C.unk02 = gUnknown_0849B018->unk24[entry->unk01];
        sub_0802F588(&gUnknown_0202575C, 4);
        entry->unk00 = 0;
        gUnknown_0849B018->unk1aae++;
        gUnknown_0849B018->unk1aae &= 0xF;
        goto again;
    }

    entry->unk00 = 0;
    army = entry->unk01;
    gUnknown_0849B018->unk24[army]++;
    gUnknown_0849B018->unk1aae++;
    gUnknown_0849B018->unk1aae &= 0xF;
    return army;
}
asm(".global sub_08030B00\n.thumb_set sub_08030B00, LinkReceiveBlock\n");
