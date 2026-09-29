#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0805C0AC.
 * sub_0805C0AC @ 0x0805C0AC
 */

/* Scans the army's 0x40 unit records for a type-0x14 unit standing on a tile
 * whose gUnknown_03003340 entry is not -1, and writes that unit's cell column
 * and row back through the out-parameter as two halfwords. The prototype in
 * include/unknown-functions.h records the argument as the address of a 4-byte
 * (u16, u16) stack object shared with AiAdvanceToward, hence the local view here.
 *
 * `(s8)` on the u8 the row pointer yields really is a CAST and it really does
 * come out as `lsls #0x18; asrs #0x18`. That does NOT contradict wave 37's
 * finding that `ldrh; lsls; asrs` is not an `(s16)` cast: THUMB has no
 * immediate-offset `ldrsb`, so sign_extend (mem:QI) has nothing to fold into
 * here and the shift pair survives, where sign_extend (mem:HI) always folds to
 * `ldrsh`. Probed both ways -- the cast is the only spelling that produces it.
 *
 * The loop bound is bound to a LOCAL, and the store is what forces it. The
 * body writes through a pointer parameter, which sets loop.c's
 * unknown_address_altered and makes any global re-read variant; the ROM
 * nevertheless reads gUnknown_03003F2C exactly once and check_dbra_loop turns
 * the exit test into a countdown, which it only does on an invariant bound. The
 * sibling builders that re-read the global in the loop (src/decomp/c_0805CA60.c,
 * AiStartSupplyPass) are the contrast case. */

struct Unk0805C0ACOut
{
    /* 0x00 */ u16 unk00;
    /* 0x02 */ u16 unk02;
};

void AiFindEmptyTCopter(void *a1)
{
    struct Unk0805C0ACOut *out = a1;
    int i;
    int end;

    i = gUnknown_03003F2C;
    end = i + 0x40;

    for (; i < end; i++)
    {
        if (gUnits[i].type == 0x14
            && gUnits[i].unk07 == 0
            && (s8)gUnknown_03003340[gUnits[i].y][gUnits[i].x] != -1)
        {
            out->unk00 = gUnits[i].x;
            out->unk02 = gUnits[i].y;
        }
    }
}
asm(".global sub_0805C0AC\n.thumb_set sub_0805C0AC, AiFindEmptyTCopter\n");
