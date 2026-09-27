#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801DCD4.
 * RunSimpleSpriteScript @ 0x0801DCD4
 *
 * Not a Xenesis-documented name. The old sub_0801DCD4 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Steps the animation script of sprite entry `id` in gUnknown_0200E438, the
 * smaller sibling of sub_0801D390 (no 0x5/0x6/0xd effect opcodes). Each u16
 * command's top nibble is the opcode and its low 12 bits the argument:
 *   0x0 wait (argument = frames)       0x1 show frame (and draw unless `tick`)
 *   0x2 jump within the script         0xe set unk2c and save the position
 *   0xf sub-ops: 0xf00 draw and stop, 0xe00 finish, 0xa00 set unk1e,
 *       0x900 save the position past this command
 * `tick` selects the tick pass (waits count down) over the draw pass. A frame
 * is skipped while `hidden` is set, and on even gGameClock frames while
 * `flicker` is set.
 *
 * Measured spelling notes. The first closes the loop-top residual parked in
 * W56-H/W71 for both this function and sub_0801D390:
 * - The command is re-read through `*p` at every use, never held in a local.
 *   A `cmd` local is a user variable that dies at each `& 0xfff`, so regmove
 *   rewrites every AND to clobber it in place (`ands rCmd, rMask`). The ROM
 *   always ANDs into a copy of the constant (`mov r0, sl; ands r0, r2`).
 * - The opcode is `(u16)((s16)*p & ~0xfff)`: the (s16) makes the AND HImode
 *   with the pool constant 0xFFFFF000. Storing it in an `int` rather than a
 *   `u16` local drops a second copy of the opcode (`adds r3, r2, #0`) that
 *   the inner compare chain would otherwise use.
 * - `end` is the never-set exit flag from W56-H.
 * - The jump target and the frame pointer are each built in two statements
 *   (base, then offset). The one-expression forms load the base late.
 */
void RunSimpleSpriteScript(int id, int tick)
{
    struct Unk0200E438 *e;
    u16 *p;
    int op;
    int offset;
    int end;
    u32 *frame;

    gUnknown_03003040 = id;
    e = &gUnknown_0200E438[id];
    p = (u16 *)e->unk08;
    while (p != NULL)
    {
        end = 0;
        op = (u16)((s16)*p & ~0xfff);
        switch (op)
        {
        case 0x0000:
            if (tick == 0)
                return;
            if ((s16)e->unk1c != 0)
                e->unk1c--;
            else
                e->unk1c = (*p & 0xfff) - 1;
            if ((s16)e->unk1c != 0)
                return;
            e->unk08 = (u32)(p + 1);
            return;
        case 0x2000:
            offset = *p & 0xfff;
            p = (u16 *)e->unk04;
            p += offset;
            break;
        case 0x1000:
            e->unk24 = *p & 0xff;
            p++;
            if (tick != 0)
                break;
            frame = (u32 *)e->unk20;
            frame += e->unk24;
            if (((struct SpriteScriptFlags *)&e->unk30)->hidden)
                break;
            if (((struct SpriteScriptFlags *)&e->unk30)->flicker && !(gGameClock & 1))
                break;
            sub_0801ECE8(e->unk1e, (e->unk14 + e->unk0c) / 256, (e->unk18 + e->unk10) / 256,
                         *frame, *(long long *)&e->unk30, e->unk38);
            break;
        case 0xe000:
            e->unk2c = *p & 0xfff;
            p++;
            e->unk08 = (u32)p;
            break;
        case 0xf000:
            switch (*p & 0xf00)
            {
            case 0xf00:
                e->unk08 = (u32)p;
                if (tick != 0)
                    return;
                frame = (u32 *)e->unk20;
                frame += e->unk24;
                if (((struct SpriteScriptFlags *)&e->unk30)->hidden)
                    return;
                if (((struct SpriteScriptFlags *)&e->unk30)->flicker && !(gGameClock & 1))
                    return;
                sub_0801ECE8(e->unk1e, (e->unk14 + e->unk0c) / 256, (e->unk18 + e->unk10) / 256,
                             *frame, *(long long *)&e->unk30, e->unk38);
                return;
            case 0xe00:
                if (tick != 0)
                    sub_0801D81C(id);
                return;
            case 0xa00:
                e->unk1e = *p;
                p++;
                break;
            case 0x900:
                e->unk08 = (u32)p;
                p++;
                e->unk08 += 2;
                break;
            }
            break;
        }
        if (end != 0)
            break;
    }
}

asm(".global sub_0801DCD4\n.thumb_set sub_0801DCD4, RunSimpleSpriteScript\n");
