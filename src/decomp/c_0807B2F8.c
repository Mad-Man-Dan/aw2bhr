#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0807B2F8.
 * sub_0807B2F8 @ 0x0807B2F8
 */

#include "proc.h"
struct Unk807B2F8
{
    /* 0x00 */ u8 filler_00[0x30];
    /* 0x30 */ int unk30;
};

void sub_0807B2F8(ProcPtr proc)
{
    if (gPlaySt.gameMode == 3)
        Proc_Goto(proc, 0);

    sub_080149C0(1, (s16)(((struct Unk807B2F8 *)proc)->unk30 + 1),
                 gBG0TilemapBuffer, GetLoadedMapName(), 0x8000, 0);
    sub_08014B0C(0xD, (s16)(((struct Unk807B2F8 *)proc)->unk30 + 1),
                 gBG0TilemapBuffer, gUnknown_03004080, 0x8000, 0);
    sub_08013AEC();
}
