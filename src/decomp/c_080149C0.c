#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x080149C0.
 * sub_080149C0 @ 0x080149C0, sub_08014A5C @ 0x08014A5C
 */

/*
 * PutTextScriptImmediate -- draw a whole text string in one go, with no frame delay.
 *
 * FindSlotScript(0) names the slot to work in, and that slot of
 * gUnknown_03001470 is used through struct Unk08014074 -- the same view of the
 * record that InitTextWriter fills. a4 is the script, a3 the tilemap to write
 * into, a5 the tile attribute bits, a1/a2 the left and top margins in tiles, and
 * a6 is handed to sub_0801B998 for each character. The loop runs TextWriterStepCommand
 * until it answers 0, drawing a character with sub_0801B998 on every 1.
 *
 * PutTextTableEntryImmediate below is the same function for a script taken from gTextTable
 * instead of being passed in.
 *
 * The three callee prototypes are declared in this file because each of their
 * definitions describes this same record through its own file-local struct and
 * none of them is in a shared header.
 *
 * Why the C looks odd: these spellings do not change what the code does, but
 * the original compiler only produces identical output with them.
 *   - `new_var` must be a `short` local holding FindSlotScript's result before it
 *     is used as a subscript. With an `s8` local, or with the call written
 *     inside the subscript, the load of the array's address comes one
 *     instruction too early.
 *   - `case 2: break;` must stay although it does nothing. With only two case
 *     labels the compiler emits a chain of two comparisons; three labels give
 *     the balanced three-comparison tree the original has, and case 2's empty
 *     arm then folds away by itself. The order the cases are written in makes no
 *     difference.
 *   - The parameters stay `int` and the narrow values go through the u16 locals
 *     x, y, e and f. Narrowing the parameters instead would make the callers
 *     narrow their arguments, and two already-matched callers would change.
 *   - The slot is cast to `struct Unk08014074 *`. struct Unk03001470 declares
 *     different types at these offsets and must not be reshaped to suit this
 *     function. unk31 is deliberately left unwritten here, although
 *     InitTextWriter writes it.
 */

u16 *sub_08013D4C(struct Unk08014074 *);
s16 TextWriterStepCommand(struct Unk08014074 *, u16 *);
void sub_0801B998(struct Unk08014074 *, u16 *, u16);

void PutTextScriptImmediate(int a1, int a2, u16 *a3, u8 *a4, int a5, int a6)
{
  u16 x = a1;
  u16 y = a2;
  u16 e = a5;
  u16 f = a6;
  struct Unk08014074 *s;
  u16 *v;
  short new_var;
  new_var = FindSlotScript(0);
  s = (struct Unk08014074 *) (&gUnknown_03001470[new_var]);
  s->unk20 = a4;
  s->unk24 = 0;
  s->unk28 = a3;
  s->unk2c = e;
  s->unk2e = 0;
  s->unk30 = x;
  s->unk32 = x;
  s->unk33 = y;
  for (;;)
  {
    v = sub_08013D4C(s);
    switch (TextWriterStepCommand(s, v))
    {
      case 0:
        return;

      case 1:
        sub_0801B998(s, v, f);
        break;

      case 2:
        break;

    }

  }

}
asm(".global sub_080149C0\n.thumb_set sub_080149C0, PutTextScriptImmediate\n");

void PutTextTableEntryImmediate(int a1, int a2, void *a3, int a4, int a5, int a6)
{
  short new_var;
  u16 x = a1;
  u16 y = a2;
  u16 d = a4;
  u16 e = a5;
  u16 f = a6;
  struct Unk08014074 *s;
  u16 *v;
  new_var = FindSlotScript(0);
  s = (struct Unk08014074 *) (&gUnknown_03001470[new_var]);
  s->unk20 = gTextTable[d];
  s->unk24 = 0;
  s->unk28 = a3;
  s->unk2c = e;
  s->unk2e = 0;
  s->unk30 = x;
  s->unk32 = x;
  s->unk33 = y;
  for (;;)
  {
    v = sub_08013D4C(s);
    switch (TextWriterStepCommand(s, v))
    {
      case 0:
        return;

      case 1:
        sub_0801B998(s, v, f);
        break;

      case 2:
        break;

    }

  }

}
asm(".global sub_08014A5C\n.thumb_set sub_08014A5C, PutTextTableEntryImmediate\n");
