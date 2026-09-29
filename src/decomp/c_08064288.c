#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08064288.
 * sub_08064288 @ 0x08064288
 */

/* WAVE 82 MATCHED byte-for-byte (configured profile). History: W50-H/W59-F/W81-C
 * carried it at 28.6% ("one extra live value"); the wave-81 shell permuter round
 * actually reached 97.96% in best.c but was never harvested. W82-B adopted its
 * lever: the four-term sums split through an inlined add helper (Add/Add/Add
 * nesting -- straight left-assoc gives high-register traffic), and the trailing
 * loop's arguments materialised through u16-independent sx/sy temporaries so
 * arg1's load lands between arg2's term and its add exactly as the ROM emits.
 * Snapshot before final form: w82-snapshot.c. */
#include "global.h"
static inline int Add(int a0, int a1)
{
    return a0 + a1;
}

void UnusedDrawCubeFrame(int a1, int a2)
{
    struct Unk0202F110Entry *e;
    struct Unk0202F140Entry *v[4];
    int x;
    int y;
    int a;
    int b;
    int c;
    int d;
    int i;
    int sx;
    int sy;

    for (i = 0; i < 1; i++)
    {
        e = &gUnknown_0202F110[i];
        v[0] = &gUnknown_0202F140[e->unk02[0]];
        v[1] = &gUnknown_0202F140[e->unk02[1]];
        v[2] = &gUnknown_0202F140[e->unk02[2]];
        v[3] = &gUnknown_0202F140[e->unk02[3]];

        x = (Add(Add(v[0]->unk0c[0], v[1]->unk0c[0]), v[2]->unk0c[0]) + v[3]->unk0c[0]) >> 9;
        y = Add(Add(Add(v[0]->unk0c[1], v[1]->unk0c[1]), v[2]->unk0c[1]), v[3]->unk0c[1]) >> 9;

        PutOamHi(a1 + x, a2 + y, gUnknown_0858092C, 0);

        a = 0x10000 / ((v[1]->unk0c[0] - v[0]->unk0c[0]) >> 5);
        b = 0x10000 / ((v[1]->unk0c[1] - v[0]->unk0c[1]) >> 5);
        c = 0x10000 / ((v[2]->unk0c[0] - v[0]->unk0c[0]) >> 5);
        d = 0x10000 / ((v[2]->unk0c[1] - v[0]->unk0c[1]) >> 5);

        SetObjAffine(i + 0x10, a, c, b, d);

        x = Add(a1, x - 0x20);
        y = Add(a2, y - 0x20);
        PutOamHi(x, y, gUnknown_08580914[i], e->unk00);
    }

    for (i = 0; i < 8; i++)
    {
        sx = gUnknown_0202F140[i].unk0c[0] >> 7;
        sy = gUnknown_0202F140[i].unk0c[1] >> 7;
        PutOamHi(a1 + sx, a2 + sy, gUnknown_0858092C, 0);
    }
}
asm(".global sub_08064288\n.thumb_set sub_08064288, UnusedDrawCubeFrame\n");
