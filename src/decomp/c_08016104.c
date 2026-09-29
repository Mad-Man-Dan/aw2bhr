#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08016104.
 * sub_08016104 @ 0x08016104, sub_08016130 @ 0x08016130, sub_0801615C @ 0x0801615C, sub_08016188 @ 0x08016188
 */

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 SlotOp_PlayMusic(u8 a)
{
    PlayMusic(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08016104\n.thumb_set sub_08016104, SlotOp_PlayMusic\n");

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 SlotOp_PlaySfx(u8 a)
{
    PlayMusicOrSfx2(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08016130\n.thumb_set sub_08016130, SlotOp_PlaySfx\n");

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 sub_0801615C(u8 a)
{
    StartWipeToBlack(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 sub_08016188(u8 a)
{
    StartWipeFromBlack(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
