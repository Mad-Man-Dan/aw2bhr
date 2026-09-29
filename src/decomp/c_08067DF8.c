#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08067DF8.
 * sub_08067DF8 @ 0x08067DF8
 */

#include "proc.h"
struct Unk67DF8Proc
{
    /* 00 */ PROC_HEADER;
    /* 29 */ u8 unk29;
    /* 2a */ u8 unk2a;
};

/* +0x29 and +0x2a are the first two user bytes after PROC_HEADER (which ends
 * at 0x29), both read with `ldrb`. This loads the OBJ graphics for one entry:
 * +0x29 picks the sprite set and +0x2a indexes gUnknown_08581104 for the tile
 * id. IntroCoSlide_HoldLoop reads the same +0x2a through the same table. */
void IntroCoSlide_Init(struct Unk67DF8Proc *proc)
{
    LoadCoFullBodyPart1(proc->unk29, gUnknown_08581104[proc->unk2a]);
}
asm(".global sub_08067DF8\n.thumb_set sub_08067DF8, IntroCoSlide_Init\n");
