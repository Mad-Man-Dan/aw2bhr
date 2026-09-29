#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08073304.
 * sub_08073304 @ 0x08073304
 */

/* MATCHED, wave 77 (W77-L). 156/156 bytes, relocs match, trymatch exit 0.
 *
 * Parked at 97.4% since wave 46 on FOUR bytes that were one fact: `src` and
 * `a6` held each other's high register (ROM src->r9, dst->r8, a6->sl; the
 * draft had src->sl, a6->r9). Wave 46 built the right model and then looked
 * for the lever in the wrong place.
 *
 * THE MODEL, which was correct: agbcc's allocno_compare ranks roughly
 * floor_log2(n_refs) * n_refs / live_length. dst has 3 refs, src 2, a6 2 with
 * a longer range, giving the ROM's order dst, src, a6. The draft got
 * dst, a6, src because agbcc COALESCES the plain copy `v = a6;`, fusing v and
 * a6 into one 4-reference allocno whose extra references outweigh a6's longer
 * range and lift it above src. Wave 46 concluded the lever must "lower a6's
 * reference count" and could not find one, because a6 is a parameter read
 * once and 2 is its floor.
 *
 * THE ACTUAL LEVER IS TO DELETE THE COPY, NOT TO SHORTEN OR RE-COUNT IT.
 * There is no `v` at all:
 *
 *     proc->unk64 = a6 != 0 ? -1 : a6;
 *
 * The false arm returns a6 itself -- which is 0 there, so this is the same
 * value the old `v = a6; if (v) v = -1;` stored -- and it is what makes the
 * ROM's fall-through shape (a6 already in r0, conditionally overwritten with
 * -1) come out of an expression instead of a copy. With no copy there is
 * nothing for regmove to coalesce, a6 keeps exactly 2 references, and the
 * allocation falls out in the ROM's order.
 *
 * Wave 46 had measured `proc->unk64 = a6 ? -1 : 0;` at 76.9% and treated the
 * `?:` axis as closed. The difference between that and the matching line is
 * ONLY the false operand: `0` is a fresh constant needing its own register,
 * `a6` reuses the value already in r0. `v = a6; proc->unk64 = v != 0 ? -1 : v;`
 * also matches, so it is the deleted COPY that matters, not deleting `v`.
 *
 * The `do { } while (0)` around the head block is still load-bearing (wave 46
 * measured it at 2 bytes; it puts dst in r8). Everything else is as wave 46
 * left it: the four entry narrowings, the `(u16)((a4 + 0x10) * 0x20)` palette
 * index as a MULTIPLY, the walking +0x5c/+0x5e/+0x5f/+0x68 store run, and the
 * literal pool at the END.
 *
 * The last parameter stays `int` to agree with include/unknown-functions.h;
 * five promoted callers pass it and the cast below is byte-neutral.
 */

#include "global.h"
#include "proc.h"
struct Unk73304Proc
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ u8 unk54;
    /* 0x55 */ u8 filler_55[0x5c - 0x55];
    /* 0x5c */ u16 unk5c;
    /* 0x5e */ u8 unk5e;
    /* 0x5f */ u8 unk5f;
    /* 0x60 */ u8 filler_60[0x64 - 0x60];
    /* 0x64 */ int unk64;
    /* 0x68 */ u8 unk68;
};

ProcPtr StartHeaderBanner(const void * src, void * dst, u16 a3, u16 a4, u16 a5,
                     u8 a6, int parent)
{
    struct Unk73304Proc *proc;

    do
    {
        Decompress(gUnknown_081CC038, dst);
        ApplyPaletteExt(gUnknown_081D2224, (u16)((a4 + 0x10) * 0x20), 0x20);

        proc = Proc_Start(ProcScr_MainMenuPutSelectModeSprite, (ProcPtr)parent);

        sub_08073228(src, dst, a3, proc);

        proc->unk54 = 0;
        proc->unk5c = a3;
        proc->unk5e = a4;
        proc->unk5f = a5;
        proc->unk68 = 0;
    } while (0);

    proc->unk64 = a6 != 0 ? -1 : a6;

    return proc;
}
asm(".global sub_08073304\n.thumb_set sub_08073304, StartHeaderBanner\n");
