#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0804A6A4.
 * sub_0804A6A4 @ 0x0804A6A4, sub_0804A6D8 @ 0x0804A6D8
 */

/* Clears every live entry of gUnknown_030044E0->unk2c. The bound is
 * `unk5f + 1`, an int expression, which is why the compare is SIGNED
 * (`bge`/`blt`) where NameEntry_Confirm's u8-against-u8 bound next door goes
 * through shorten_compare and comes out unsigned. */
void NameEntry_ClearBuffer(void)
{
    u8 i;

    for (i = 0; i < gUnknown_030044E0->unk5f + 1; i++)
        gUnknown_030044E0->unk2c[i] = 0;
}
asm(".global sub_0804A6A4\n.thumb_set sub_0804A6A4, NameEntry_ClearBuffer\n");

/* gUnknown_0812A280 is NOT an object: the word at 0x0812A280 holds 0x030044E0,
 * so it is agbcc's -fforce-addr address constant for gUnknown_030044E0, the
 * same struct NameEntry_ClearBuffer next door reaches with a plain pool word. Reading it
 * as a real global costs one indirection too many -- the trap the
 * gUnknown_0812A2A8 note records.
 *
 * The loop compare is UNSIGNED (`bhs`/`blo`) because both sides are u8 and
 * shorten_compare narrows the comparison back into the shorter unsigned type;
 * NameEntry_ClearBuffer's bound is `unk5f + 1`, an int, and comes out signed.
 *
 * The RETURN TYPE IS NOT void, and the epilogue is the only thing that says so:
 * the ROM pops the return address into r1 (`pop {r1}; bx r1`) where every void
 * neighbour in this block -- NameEntry_SlideIn_Step, LoadBattleBackdropArt, sub_0804BAC4,
 * RedrawNameEntryText -- pops it into r0. agbcc keeps r0 clear at the epilogue when it
 * holds a return value. Both arms end in a void call, so nothing supplies one
 * and the value is whatever PlayMusicOrSfx2 left; that is what the original did. */
int NameEntry_Confirm(void)
{
    u8 count;
    u8 i;

    count = 0;
    RedrawNameEntryText(0);

    for (i = 0; i < gUnknown_030044E0->unk5f; i++)
    {
        if (gUnknown_030044E0->unk2c[i] != gUnknown_084C3D12
         && gUnknown_030044E0->unk2c[i] != 0)
            count++;
    }

    if (count != 0)
    {
        gUnknown_030044E0->unk63 = 2;
        StartEventScript(gUnknown_084C398C);
        PlayMusicOrSfx2(0x71);
    }
    else
    {
        gUnknown_030044E0->unk63 = 1;
        NameEntry_StartPromptMessage();
        PlayMusicOrSfx2(0x68);
    }
}
asm(".global sub_0804A6D8\n.thumb_set sub_0804A6D8, NameEntry_Confirm\n");
