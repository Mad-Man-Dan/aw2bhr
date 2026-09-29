#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080145E8.
 * sub_080145E8 @ 0x080145E8
 */

/*
 * TextBox_Init -- copy the text writer's parameters out of gUnknown_0200C020
 * into another record of the same type.
 *
 * Exactly the fields InitTextWriter fills, in the order InitTextWriter writes them.
 *
 * Why the C looks odd: this spelling does not change what the code does, but
 * the original compiler only produces identical output with it.
 *   - `d->unk34 = d->unk36 = <value>` is one chained assignment. One load feeds
 *     both stores and unk36 is written first, as in the original.
 */
void TextBox_Init(struct Unk08014074 *d)
{
    d->unk20 = gUnknown_0200C020.unk20;
    d->unk24 = gUnknown_0200C020.unk24;
    d->unk28 = gUnknown_0200C020.unk28;
    d->unk2c = gUnknown_0200C020.unk2c;
    d->unk2e = gUnknown_0200C020.unk2e;
    d->unk34 = d->unk36 = gUnknown_0200C020.unk34;
    d->unk38 = gUnknown_0200C020.unk38;
    d->unk30 = gUnknown_0200C020.unk30;
    d->unk31 = gUnknown_0200C020.unk31;
    d->unk32 = gUnknown_0200C020.unk32;
    d->unk33 = gUnknown_0200C020.unk33;
    d->unk39 = gUnknown_0200C020.unk39;
    d->unk3a = gUnknown_0200C020.unk3a;
    d->unk3c = gUnknown_0200C020.unk3c;
    d->unk40 = gUnknown_0200C020.unk40;
}
asm(".global sub_080145E8\n.thumb_set sub_080145E8, TextBox_Init\n");
