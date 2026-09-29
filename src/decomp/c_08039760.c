#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08039760.
 * sub_08039760 @ 0x08039760
 */

#include "proc.h"
#include "hardware.h"
/* CoPowerIntro_FadeInLoop's mirror image: the fade-OUT step, every ninth frame moving one
 * unit back from gUnknown_03002020 into gUnknown_03002B28, ending the group at
 * zero instead of breaking it at eight.
 *
 * The 0x08090F68 pool word is this function's own private -fforce-addr .rodata
 * copy of &gUnknown_03002020 -- the same address CoPowerIntro_FadeInLoop reaches through
 * 0x08090F64. Two adjacent words holding one address look like two globals in
 * the split index and are not; the global is named honestly here. */
struct Unk39760Proc
{
    /* 0x00 */ PROC_HEADER;
    /* 0x29 */ STRUCT_PAD(0x29, 0x64);
    /* 0x64 */ s16 unk64;
};

void CoPowerIntro_FadeOutLoop(struct Unk39760Proc *proc)
{
    if (proc->unk64++ > 8)
    {
        gUnknown_03002020--;
        gUnknown_03002B28++;
        proc->unk64 = 0;
    }

    if (gUnknown_03002020 == 0)
    {
        Proc_Break(proc);
        Proc_EndEach(gUnknown_0849D7FC);
        ClearBg0Tilemap();
        BG_EnableSyncBG0();
    }
}
asm(".global sub_08039760\n.thumb_set sub_08039760, CoPowerIntro_FadeOutLoop\n");
