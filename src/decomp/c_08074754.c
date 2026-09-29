#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08074754.
 * sub_08074754 @ 0x08074754, sub_080747FC @ 0x080747FC, sub_08074834 @ 0x08074834
 */

#include "proc.h"

/* Appends one marker to the 12-byte record list at &gUnknown_0202FDFC.unk3c:
 * scan to the -1 terminator, fill that slot from gUnknown_08615194[id], and
 * re-terminate the slot after it. See gUnknown_0202FE38 in
 * include/unknown-globals.h for the record type and for why the list and the
 * struct are one symbol.
 * The zero-trip `do { } while (0)` is the wave-17 register-allocation lever and
 * it is load-bearing here: without it the cursor and gUnknown_08615194[id]
 * swap r5 and r6, an otherwise instruction-identical 91.1%. Binding the base
 * to `new_var` alone does NOT do it -- that was tested separately and still
 * scored 91.1%, so the wrapper is the whole effect. Found by the permuter.
 */
void AddWorldMapMarker(s16 id)
{
  const struct Unk08615194 *r;
  struct Unk0202FE38 *p;
  s16 *new_var;
  struct Unk0801C210 *sprite;
  int mode;
  new_var = &gUnknown_0202FDFC.unk3c;
  for (p = (struct Unk0202FE38 *) new_var; p->unk00 != (-1); p++)
  {
    ;
  }

  mode = 1;
  r = &gUnknown_08615194[id];
  if (r->specialProperty & 4)
  {
    mode = 2;
  }
  if (r->specialProperty & 8)
  {
    mode = 3;
  }
  sprite = AP_Create(gUnknown_081D2930, 1, 1);
  AP_SwitchAnimation(sprite, mode);
  p->unk08 = sprite;
 do { p->unk02 = r->flagX; p->unk04 = r->flagY; } while (0);
  p->unk00 = id;
  gUnknown_0202FDFC.unk12[id] |= r->specialProperty;
  p[1].unk00 = -1;
}
asm(".global sub_08074754\n.thumb_set sub_08074754, AddWorldMapMarker\n");

/* Re-announces every already-live marker: one pass over the 42 owner bytes,
 * calling AddWorldMapMarker for each that has bit 0 set. The `|= 1` afterwards is
 * redundant against the test that guards it, but it is what the ROM does --
 * `movs r6,#1` is hoisted out of the loop and reused by the `orrs`.
 * The `lsls #0x10; asrs #0x10` in front of the `bl` is the s16 conversion of
 * the plain `int` counter; see AddWorldMapMarker in include/unknown-functions.h.
 */
void RestoreWorldMapMarkers(void)
{
    int i;

    for (i = 0; i <= 0x29; i++)
    {
        if (gUnknown_0202FDFC.unk12[i] & 1)
        {
            AddWorldMapMarker(i);
            gUnknown_0202FDFC.unk12[i] |= 1;
        }
    }
}
asm(".global sub_080747FC\n.thumb_set sub_080747FC, RestoreWorldMapMarkers\n");

/* The lookup-and-remove half of the 12-byte record list AddWorldMapMarker appends
 * to; see gUnknown_0202FE38 in include/unknown-globals.h. The record base is
 * reached through agbcc's own -fforce-addr word (the ROM's `gUnknown_081CC4C0`
 * is that word, not a table), which is why the second loop RELOADS the base
 * rather than keeping it live -- naming the object at each use is what puts it
 * there. The compaction loop needs its OWN counter: sharing `i` with the
 * search loop makes one long-lived allocno and swaps three registers.
 */
int RemoveWorldMapMarker(s32 id, struct Unk0202FE38 *out)
{
    struct Unk0202FE38 *p;
    int i;
    int j;

    p = (struct Unk0202FE38 *)&gUnknown_0202FDFC.unk3c;

    for (i = 0; i < 16; i++)
    {
        if (p->unk00 == id)
        {
            out->unk00 = p->unk00;
            out->unk02 = p->unk02;
            out->unk04 = p->unk04;
            out->unk08 = p->unk08;
            break;
        }

        p++;
    }

    if (i == 16)
        return 0;

    p = (struct Unk0202FE38 *)&gUnknown_0202FDFC.unk3c;

    for (j = i + 1; j < 16; j++)
        p[j - 1] = p[j];

    return 1;
}
asm(".global sub_08074834\n.thumb_set sub_08074834, RemoveWorldMapMarker\n");
