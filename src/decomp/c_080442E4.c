#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080442E4.
 * sub_080442E4 @ 0x080442E4
 */

/* Advances one army's state byte through 0 -> 1 -> 2, each step gated on its
 * own predicate, and reports whether it moved.
 *
 * gUnknown_08091388 is NOT a global -- baserom.gba holds 0x08499598 there, so
 * it is agbcc's `-fforce-addr` word for &gPlayers and the double
 * `ldr` is that indirection, not a pointer-to-pointer in the source.
 *
 * IsSuperCoPowerReady and IsCoPowerAvailable are declared `int` but both results are tested
 * as BYTES (`lsls r0,#0x18; cmp r0,#0`), hence the casts. Their own bodies are
 * still asm, so the declarations are left alone rather than retyped. */
bool8 AdvanceCoPowerReadyAnnouncement(int a)
{
    if (gPlayers[a].unk24 <= 1 && (u8)IsSuperCoPowerReady(a))
    {
        gPlayers[a].unk24 = 2;
        return TRUE;
    }

    if (gPlayers[a].unk24 == 0 && (u8)IsCoPowerAvailable(a))
    {
        gPlayers[a].unk24 = 1;
        return TRUE;
    }

    return FALSE;
}
asm(".global sub_080442E4\n.thumb_set sub_080442E4, AdvanceCoPowerReadyAnnouncement\n");
