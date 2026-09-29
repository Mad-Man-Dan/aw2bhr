#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080879B0.
 * sub_080879B0 @ 0x080879B0, sub_080879D8 @ 0x080879D8
 *
 * Named per src/aw2e-names.s (proc-table labels auto-generated from
 * AW2E.lua). The old sub_XXXXXXXX symbols are kept as linker aliases
 * below so every other unit keeps resolving them unchanged.
 */

struct Unk080879D8
{
    /* 0x00 */ u8 filler_00[0x54];
    /* 0x54 */ int unk54;
};

void PreviewMapRecords_CB_080879B1(void)
{
    sub_08012BC8(gBG2TilemapBuffer, 0xD, 4, 0x12, 0xC, 0);
    sub_08013B0C();
}

void PreviewMapRecords_080879D9(struct Unk080879D8 *proc)
{
    DrawWindowBackground(0xD, 4, 0x12, 0xC, gBG2TilemapBuffer, 0);
    sub_08013B0C();
    MapSelect_SetBlend();
    LoadMapRecordMinimugs(proc->unk54);
}

asm(".global sub_080879B0\n.thumb_set sub_080879B0, PreviewMapRecords_CB_080879B1\n"
    ".global sub_080879D8\n.thumb_set sub_080879D8, PreviewMapRecords_080879D9\n");
