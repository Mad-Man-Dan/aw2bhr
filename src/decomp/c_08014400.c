#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014400.
 * sub_08014400 @ 0x08014400
 */

#include "hardware.h"
/*
 * TextBox_Loop -- draw the next part of a text box, one frame's worth at a time.
 *
 * s is the text writer's state (see TextWriterStepCommand in c_08014084.c for the
 * fields). Nothing happens while any of the three helper processes is still
 * there: FindSlotScript answers -1 for a process that is gone and all three must
 * answer -1.
 *
 * If IsTextSkipAllowed allows it and START is down in gpKeySt->unk0c, the rest of the
 * box is skipped: gUnknown_03002514 is set, the text is handed to ClearTilemapRect23x4
 * and EndSlotScriptAt restores the screen named by gUnknown_03001FBC. Otherwise
 * holding A while the delay is above 1 switches the delay off, so the remainder
 * appears at once.
 *
 * unk39 then counts frames towards the delay in unk3a and the function returns
 * until it runs out. After that it pumps TextWriterStepCommand: 0 ends the box and
 * restores the previous screen, 2 stops until the next frame, 3 asks for another
 * command immediately, and 1 (which shares its body with default) draws one
 * character -- DrawGlyphRam renders it into the tile data at the background's
 * character block, TextWriterAdvanceCursor and sub_080143EC move the write position on,
 * sound 0x70 plays when unk38 is set, and the read position steps on by one. The
 * loop repeats while unk39 is negative, which is how a box with the delay off
 * empties in a single frame.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - All four case labels must be present and `case 1:` must share its body
 *     with `default:`. Without `default:` the two unlabelled values go to the end
 *     of the switch instead of into case 1's block.
 *   - The frame counter is tested by two separate `if` statements, not one
 *     nested pair. The original tests `t >= 0` twice; nesting the second test
 *     inside the first emits it once.
 *   - The character base is cast to `u8 *` before the tile offset is added, so
 *     that the compiler cannot lift the 0x06000000 out of the tile term.
 *   - The four callee prototypes are declared in this file: none of them is in a
 *     header, and the two that are already promoted describe this same record
 *     through their own file-local structs.
 */

u16 *sub_08013D4C(struct Unk08014074 *);
u16 *sub_08013D64(struct Unk08014074 *);
s16 TextWriterStepCommand(struct Unk08014074 *, u16 *);
void sub_080143EC(struct Unk08014074 *, u16 *);

void TextBox_Loop(struct Unk08014074 *s)
{
    s8 a;
    s8 b;
    s8 t;
    int w;

    a = FindSlotScript((s32)gUnknown_08489518);
    if (a != -1)
        return;

    b = FindSlotScript((s32)gUnknown_0848A398);
    if (b != a)
        return;

    if ((s8)FindSlotScript((s32)gUnknown_0848A3C4) != b)
        return;

    if (IsTextSkipAllowed() && (gpKeySt->unk0c & 8) && gUnknown_03002514 == 0)
    {
        gUnknown_03002514 = 1;
        ClearTilemapRect23x4(sub_08013D64(s));
        s->unk3c();
        EndSlotScriptAt(gUnknown_03001FBC);
        return;
    }

    if (s->unk3a > 1 && (gpKeySt->unk0c & 1))
        TextWriterDisableDelay(s);

    t = ++s->unk39;
    if (t >= 0 && t < s->unk3a)
        return;
    if (t >= 0)
        s->unk39 = 0;

    s->unk3c();

    do
    {
        switch (TextWriterStepCommand(s, sub_08013D4C(s)))
        {
        case 0:
            EndSlotScriptAt(gUnknown_03001FBC);
            return;

        case 2:
            return;

        case 3:
            break;

        case 1:
        default:
            if (s->unk32 != s->unk30 || s->unk40 != 0)
                TextWriterAdvanceCursor(s, 1);

            w = DrawGlyphRam(*s->unk20,
                             (int)((u8 *)(gUnknown_03002B6C.bits.chr_block * 0x4000)
                                 + (s->unk34 * 32 + 0x06000000)),
                             s->unk40, s->unk2e);

            if (s->unk40 <= 1)
                sub_080143EC(s, sub_08013D4C(s));

            if ((u8)TextWriterAdvanceCursor(s, w))
                sub_080143EC(s, sub_08013D4C(s));

            if (s->unk38 != 0)
                PlayMusicOrSfx2(0x70);

            s->unk20++;
            break;
        }
    } while (s->unk39 < 0);
}
asm(".global sub_08014400\n.thumb_set sub_08014400, TextBox_Loop\n");
