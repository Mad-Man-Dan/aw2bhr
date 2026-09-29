#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067D04.
 * sub_08067D04 @ 0x08067D04
 */

#include "proc.h"
struct Unk67D04Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ STRUCT_PAD(0x29, 0x34);
    /* 34 */ int unk34;
    /* 38 */ int unk38;
    /* 3c */ int unk3c;
};

/* The starter for the gUnknown_085810B8 script that EndIntroSlideSprite tears down.
 * Four parameters: the first indexes the graphics table, the middle two are
 * stashed in the new proc, and the fourth is the parent -- r3 moves to r1 for
 * Proc_Start, which is the pass-through tell.
 *
 * `movs r1,#0x88; lsls r1,#2` is the constant 0x220, i.e. ApplyPalettes(src,
 * 0x11, 1); it is left in the raw ApplyPaletteExt form the ROM emits because
 * the macro would hide which of the two spellings produced the shift. */
void StartIntroSlideSprite(int a, int b, int c, ProcPtr parent)
{
    struct Unk67D04Proc *proc = Proc_Start(gUnknown_085810B8, parent);

    proc->unk34 = b;
    proc->unk38 = c;
    proc->unk3c = 0;

    Decompress(gUnknown_085810C8[a], (void *)0x06010000);
    ApplyPaletteExt(gUnknown_0817D9F8, 0x220, 0x20);
}
asm(".global sub_08067D04\n.thumb_set sub_08067D04, StartIntroSlideSprite\n");
