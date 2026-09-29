#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08014074.
 * sub_08014074 @ 0x08014074
 */

/*
 * TextWriterDisableDelay -- switch the text writer's per-character delay off.
 *
 * unk3a is the delay in frames and unk39 the counter that runs it. With unk3a at
 * 0 and unk39 at -2, TextBox_Loop's loop never waits and the rest of the text
 * appears in one frame. struct Unk08014074 is declared in
 * include/unknown-globals.h; gUnknown_0200C020 is one instance of it and
 * InitTextWriter the routine that fills one in.
 */

void TextWriterDisableDelay(struct Unk08014074 *s)
{
    s->unk3a = 0;
    s->unk39 = -2;
}
asm(".global sub_08014074\n.thumb_set sub_08014074, TextWriterDisableDelay\n");
