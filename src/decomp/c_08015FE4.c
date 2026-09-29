#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08015FE4.
 * sub_08015FE4 @ 0x08015FE4, sub_08016010 @ 0x08016010, sub_0801603C @ 0x0801603C, sub_08016068 @ 0x08016068, sub_08016094 @ 0x08016094
 */

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 SlotOp_StartFadeToBlack(u8 a)
{
    StartFadeToBlack(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08015FE4\n.thumb_set sub_08015FE4, SlotOp_StartFadeToBlack\n");

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 SlotOp_StartFadeFromBlack(u8 a)
{
    StartFadeFromBlack(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}
asm(".global sub_08016010\n.thumb_set sub_08016010, SlotOp_StartFadeFromBlack\n");

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 sub_0801603C(u8 a)
{
    StartFadeToWhite(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}

/* One command of the gUnknown_03001470[a].unk04 script stream: the halfword at
 * +4 of the current 8-byte command is the argument, and the cursor then steps
 * one command forward. Same shape as SlotOp_Wait next door, which reads the
 * same halfword into .unk10.
 */
bool8 sub_08016068(u8 a)
{
    StartFadeFromWhite(((const u16 *)gUnknown_03001470[a].unk04)[2]);
    gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    return FALSE;
}

/* The conditional twin of SlotOp_SetFlag: gUnknown_03002F1C is a one-shot flag
 * that turns the next command into a JUMP -- when it is set, the flag is
 * cleared and the cursor is replaced by the word the current command points
 * at, instead of stepping the usual 8 bytes forward.
 */
bool8 SlotOp_JumpIfFlag(u8 a)
{
    if (gUnknown_03002F1C == 0)
    {
        gUnknown_03001470[a].unk04 = (const u32 *)gUnknown_03001470[a].unk04 + 2;
    }
    else
    {
        gUnknown_03002F1C = 0;
        gUnknown_03001470[a].unk04 = *(const void *const *)gUnknown_03001470[a].unk04;
    }

    return TRUE;
}
asm(".global sub_08016094\n.thumb_set sub_08016094, SlotOp_JumpIfFlag\n");
