#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08066580.
 * sub_08066580 @ 0x08066580
 */

/* Installs a gUnknown_03001470 slot and registers it in the two parallel
 * per-slot tables. The `((u16 *)&p->unk28)[0..1]` spelling is forced: the ROM
 * writes +0x28 and +0x2a as two `strh`, while Unk03001470.unk28 is the u32 that
 * five already-promoted files write whole. That word/halfword duality wants a
 * union in the struct; until one exists this is the honest way to say it.
 *
 * All three parameters are `int` -- the prologue is three bare
 * `adds rN, rM, #0` copies with no PROMOTE_MODE narrowing. */
void StartReadyMarker(int a1, int a2, int a3)
{
    struct Unk03001470 *p;

    p = sub_080152EC(gUnknown_08580D0C, 1);
    ((u16 *)&p->unk28)[0] = a2;
    ((u16 *)&p->unk28)[1] = a3;
    p->unk1c = a1;
    gUnknown_08580934->unk74[a1] = p;
    gUnknown_08580934->unk70[a1] = 0xFF;
}
asm(".global sub_08066580\n.thumb_set sub_08066580, StartReadyMarker\n");
