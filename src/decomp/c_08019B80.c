#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019B80.
 * sub_08019B80 @ 0x08019B80
 */

#include "hardware.h"
/* The same 0x48-byte object src/decomp/c_08019B50.c walks -- this is the
 * D-pad handler that moves its cursor and then re-dispatches through
 * Menu_CallCursorHook.
 *
 * Neither `p->unk41` nor `p->unk42` may be bound to a local: the stores to
 * unk42 invalidate every memory value cse.c is holding, so the ROM re-reads
 * unk41 in the second arm rather than keeping it in a callee-saved register,
 * which is exactly what the inline spelling produces. The single `u8` read at
 * the top does get CSEd for both the `bls` and the `strb` in the first arm,
 * because nothing is stored in between. */
struct Unk08019B80Cmd /* 0x20 */
{
    /* 0x00 */ u8 filler_00[0x0c];
    /* 0x0c */ void (*unk0c)(u8, u8, u8);
    /* 0x10 */ u8 filler_10[0x10];
};
struct Unk08019B80 /* 0x48 */
{
    /* 0x00 */ u8 filler_00[0x20];
    /* 0x20 */ struct Unk08019B80Cmd *unk20;
    /* 0x24 */ u8 unk24[0x0d];
    /* 0x31 */ u8 unk31[0x10];
    /* 0x41 */ u8 unk41;
    /* 0x42 */ u8 unk42;
};
#define Q ((struct Unk08019B80 *)arg)

u8 Menu_MoveCursorFromDpad(void *arg)
{
    int hit = 0;

    if (Q->unk41 <= 1)
        return 0;

    if (gpKeySt->repeated & DPAD_UP)
    {
        if (Q->unk42 == 0)
        {
            if (gpKeySt->repeated != gpKeySt->pressed)
                return 0;
            Q->unk42 = Q->unk41;
        }
        Q->unk42--;
        hit = 1;
        PlayMusicOrSfx2(0x67);
    }

    if (gpKeySt->repeated & DPAD_DOWN)
    {
        if (Q->unk42 == Q->unk41 - 1)
        {
            if (gpKeySt->repeated != gpKeySt->pressed)
                return 0;
            Q->unk42 = 0xff;
        }
        Q->unk42++;
        hit = 1;
        PlayMusicOrSfx2(0x67);
    }

    if (hit == 1)
    {
        Menu_CallCursorHook(arg);
        return 1;
    }

    return 0;
}
asm(".global sub_08019B80\n.thumb_set sub_08019B80, Menu_MoveCursorFromDpad\n");
