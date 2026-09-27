#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801D390.
 * RunSpriteScript @ 0x0801D390
 *
 * Not a Xenesis-documented name. The old sub_0801D390 symbol is kept as a
 * linker alias below so every other unit keeps resolving it unchanged.
 */

/* Steps the animation script of sprite entry `id` in gUnknown_0200E438, the
 * full sibling of RunSimpleSpriteScript (sub_0801DCD4). Each u16 command's
 * top nibble is the opcode and its low 12 bits the argument:
 *   0x0 wait (argument = frames)       0x1 show frame (and draw unless `tick`)
 *   0x2 jump within the script         0x5 one-shot effect, one s16 operand
 *   0x6 one-shot effect, two s16 percentages converted to 8.8 fixed point
 *   0xd set unk26                      0xe set unk2c and save the position
 *   0xf sub-ops: 0xf00 draw and stop, 0xe00 finish, 0xa00 set unk1e,
 *       0x900 save the position past this command
 * Each 0x5/0x6 effect fires once per entry: unk02 bits 2-5 latch it, and a
 * latched effect just skips its operands. `tick` selects the tick pass (waits
 * count down) over the draw pass.
 *
 * Measured spelling notes:
 * - The loop-top and AND spellings are RunSimpleSpriteScript's: the opcode
 *   is `(u16)((s16)*p & ~0xfff)` (a HImode AND with pool word 0xFFFFF000 and
 *   the ROM's two subreg copies), and the command is re-read through `*p` at
 *   every use instead of being held in a local. A command local makes regmove
 *   AND into the dying variable instead of into a copy of the constant.
 * - This draft's shape is what puts `e` in r8 and `p` in r7, as in the ROM:
 *   `arg` and `first` as named int locals, `frame` and the jump target each
 *   built in two statements, and the latched-effect skips written as
 *   `p++; break;` in each arm. A fresh draft with the same mask spelling
 *   compiled to 800 bytes with `e` in r7 (docs/agbcc-codegen.md, "sub_0801D390's
 *   remaining residual").
 * - `end` is the never-set exit flag from W56-H.
 */
void RunSpriteScript(int id, int tick)
{
    struct Unk0200E438 *e;
    u16 *p;
    int arg;
    int end;
    int first;
    u32 *frame;

    gUnknown_03003040 = id;
    e = &gUnknown_0200E438[id];
    p = (u16 *)e->unk08;

    while (p != NULL)
    {
        end = 0;
        switch ((u16)((s16)*p & ~0xFFF))
        {
        case 0x0000:
            if (tick == 0)
                return;
            if ((s16)e->unk1c != 0)
                e->unk1c = e->unk1c - 1;
            else
                e->unk1c = (*p & 0xFFF) - 1;
            if ((s16)e->unk1c != 0)
                return;
            e->unk08 = (u32)(p + 1);
            e->unk02 = 0;
            return;
        case 0x2000:
            arg = *p & 0xFFF;
            p = (u16 *)e->unk04;
            p += arg;
            break;
        case 0x1000:
            e->unk24 = *p & 0xFF;
            p++;
            if (tick == 0)
            {
                frame = (u32 *)e->unk20;
                frame += e->unk24;
                if (((struct SpriteScriptFlags *)&e->unk30)->hidden == 0)
                    sub_0801D348(e->unk26, (e->unk14 + e->unk0c) / 256,
                                 (e->unk18 + e->unk10) / 256, *frame,
                                 e->unk1e, e->unk28,
                                 *(long long *)&e->unk30, e->unk38);
            }
            break;
        case 0x5000:
            arg = *p & 0xFFF;
            p++;
            if (arg != 0)
            {
                if (e->unk02 & 8)
                {
                    p++;
                    break;
                }
                e->unk02 |= 8;
                sub_0801E294(e->unk28, arg, *(s16 *)p++);
            }
            else
            {
                if (e->unk02 & 4)
                {
                    p++;
                    break;
                }
                e->unk02 |= 4;
                sub_0801E264(e->unk28, *(s16 *)p++);
            }
            break;
        case 0x6000:
            arg = *p & 0xFFF;
            p++;
            first = *(s16 *)p++;
            if (arg != 0)
            {
                if (e->unk02 & 0x20)
                {
                    p++;
                    break;
                }
                e->unk02 |= 0x20;
                sub_0801E27C(e->unk28, arg, first * 0x100 / 100,
                             *(s16 *)p++ * 0x100 / 100);
            }
            else
            {
                if (e->unk02 & 0x10)
                {
                    p++;
                    break;
                }
                e->unk02 |= 0x10;
                sub_0801E248(e->unk28, first * 0x100 / 100,
                             *(s16 *)p++ * 0x100 / 100);
            }
            break;
        case 0xE000:
            e->unk2c = *p & 0xFFF;
            p++;
            e->unk08 = (u32)p;
            break;
        case 0xD000:
            e->unk26 = *p & 0xFFF;
            p++;
            break;
        case 0xF000:
            switch (*p & 0xF00)
            {
            case 0xF00:
                e->unk08 = (u32)p;
                if (tick != 0)
                    return;
                frame = (u32 *)e->unk20;
                frame += e->unk24;
                if (((struct SpriteScriptFlags *)&e->unk30)->hidden == 0)
                    sub_0801D348(e->unk26, (e->unk14 + e->unk0c) / 256,
                                 (e->unk18 + e->unk10) / 256, *frame,
                                 e->unk1e, e->unk28,
                                 *(long long *)&e->unk30, e->unk38);
                return;
            case 0xE00:
                if (tick != 0)
                    sub_0801D81C(id);
                return;
            case 0xA00:
                e->unk1e = *p;
                p++;
                break;
            case 0x900:
                e->unk08 = (u32)p++;
                e->unk08 += 2;
                break;
            }
            break;
        }
        if (end != 0)
            break;
    }
}

asm(".global sub_0801D390\n.thumb_set sub_0801D390, RunSpriteScript\n");
