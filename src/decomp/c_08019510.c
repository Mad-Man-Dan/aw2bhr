#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08019510.
 * sub_08019510 @ 0x08019510
 */

#include "hardware.h"
/* A proc handler: on A or Start it optionally arms the skip flag, then tears
 * the proc down -- restore its saved palette, run its stored callback and clear
 * the +8 word -- and finally ticks sub_0803670C either way.
 *
 * gpKeySt is reached through a DIRECT pool word here, not the .rodata address
 * constant its siblings use: `ldr r5, =gpKeySt` is loaded once and the pointer
 * re-read after the IsTextSkipAllowed call, which is CSE of the symbol address with
 * the call as the barrier on the value.
 *
 * `->last`, i.e. +0x0c, is the key field, and 9 is A|Start tested as one mask
 * before 8 (Start) is tested alone. */
struct Unk8019510
{
    /* 0x00 */ u8 filler_00[0x08];
    /* 0x08 */ int unk08;
    /* 0x0c */ u8 filler_0c[0x1c];
    /* 0x28 */ u16 *unk28;
    /* 0x2c */ void (*unk2c)(void);
};

void TextBoxConfirm_Loop(void *proc)
{
    struct Unk8019510 *p = (struct Unk8019510 *)proc;

    if (gpKeySt->unk0c & 9)
    {
        if (IsTextSkipAllowed() && (gpKeySt->unk0c & 8))
            gUnknown_03002514 = 1;

        ClearTilemapRect23x4(p->unk28);
        p->unk2c();
        p->unk08 = 0;
    }

    sub_0803670C();
}
asm(".global sub_08019510\n.thumb_set sub_08019510, TextBoxConfirm_Loop\n");
