#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08028D28.
 * sub_08028D28 @ 0x08028D28
 */

#include "proc.h"
/* MATCHED (wave 87, W87-C). 240/240 bytes.
 *
 * The park (waves 38/58/59) classified this as a HOLD/REMATERIALISE INVERSION
 * with no source lever, on two claims that are both REFUTED here:
 *
 *  1. W59-B: "the inner bound appears TWICE from ONE source expression and
 *     agbcc gives the two copies different register strategies -- no spelling
 *     of one source expression can request two strategies." True of ONE
 *     expression; the answer is to write TWO. Hand-inverting both loops
 *     (zero-trip `if` + `do/while`) makes the guard and the bottom SEPARATE
 *     source expressions, so the inner guard can read the held map pointer
 *     one-level (`*(u16 *)map`) while the inner bottom re-walks the chain
 *     two-level (`*(u16 *)*ga`). Same lever as the promoted sub_0803E6C4-family
 *     drafts, which read one halfword through two different symbols.
 *
 *  2. W59-B: "the ROM's `mov r8,r0` sits AFTER the zero-trip compare because it
 *     is an LICM hoist; a source binding is a statement and always lands BEFORE
 *     the compare, so there is no source position inside a preheader." Also
 *     false once the loop is hand-inverted: the first statement INSIDE the
 *     zero-trip `if` IS the preheader. `map = gUnknown_08499590;` written there
 *     costs no load at all -- cse folds it into the guard's own `ldr r0,[r1]`
 *     -- and leaves exactly the ROM's bare `mov`.
 *
 * The last two bytes were the ORDER of the two preheader copies. With only
 * `map` bound, agbcc emits `mov r8,r0` (the source bind) before `mov sl,r1`
 * (the address, a real LICM hoist attributed to the loop's first line). Binding
 * the ADDRESS to its own local as well -- `ga = &gUnknown_08499590;` FIRST --
 * makes both copies source binds, and they are then emitted in SOURCE ORDER.
 * `ga` costs nothing: cse folds it into the guard's force-addr `ldr r1,=sym`.
 * See the wave-87 chapter in docs/agbcc-codegen.md.
 *
 * Everything the park called settled stayed settled: proc->unk4c = ...; buf =
 * proc->unk4c; in that order, the s16 x/y with y's shifted form in the single
 * stack word, the (s8)-on-u8 cell test, and all four particle stores.
 */

struct Unk08028D28Sprite
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
    /* 0x04 */ u16 unk04;
    /* 0x06 */ u16 unk06;
};
struct Unk08028D28Proc
{
    /* 0x00 */ PROC_HEADER;
    STRUCT_PAD(0x29, 0x4c);
    /* 0x4C */ struct Unk08028D28Sprite *unk4c;
    STRUCT_PAD(0x50, 0x64);
    /* 0x64 */ s16 unk64;
    /* 0x66 */ s16 unk66;
};

void RangeSpread_Init(struct Unk08028D28Proc *proc)
{
    struct Unk08028D28Sprite *buf;
    u8 **ga;
    u8 *map;
    s16 x, y;

    proc->unk4c = HeapMalloc(0x660);
    buf = proc->unk4c;
    proc->unk64 = 0;
    proc->unk66 = 0;

    y = 0;
    if (y < *(u16 *)(gUnknown_08499590 + 2))
    {
        ga = &gUnknown_08499590;
        map = gUnknown_08499590;
        do
        {
            x = 0;
            if (x < *(u16 *)map)
            {
                do
                {
                    if ((s8)gUnknown_03003340[y][x] >= 0)
                    {
                        buf[proc->unk64].unk00 = gUnknown_03003100.pos.unk00 * 16;
                        buf[proc->unk64].unk02 = gUnknown_03003100.pos.unk02 * 16;
                        buf[proc->unk64].unk04 = (x - gUnknown_03003100.pos.unk00) * 2;
                        buf[proc->unk64].unk06 = (y - gUnknown_03003100.pos.unk02) * 2;
                        proc->unk64++;
                    }
                    x++;
                } while (x < *(u16 *)*ga);
            }
            y++;
        } while (y < *(u16 *)(map + 2));
    }
}
asm(".global sub_08028D28\n.thumb_set sub_08028D28, RangeSpread_Init\n");
