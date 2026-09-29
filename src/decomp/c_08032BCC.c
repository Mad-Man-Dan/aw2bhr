#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x08032BCC.
 * sub_08032BCC @ 0x08032BCC
 */

#include "proc.h"
/* The link-lobby screen's init proc step. Reached only through a ProcCmd
 * table, so its proc type is local to this unit.
 *
 * Wave 50 (W50-D) -- THE PREHEADER ORDER IS THE WHOLE FUNCTION. The honest
 * spelling (`&gUnknown_02027C2C[i * 0x13]` and `gUnknown_08613CDC[0]` written
 * out at their uses) puts gUnknown_08613CDC's address in a pseudo that the
 * allocator then refuses a callee-saved register, so reload rematerialises the
 * `ldr` at the use and the frame is one high register short. Binding BOTH bases
 * to locals AFTER a separate `i = 0;` is what reproduces
 *   movs r5,#0 / ldr r6,=gUnknown_02027C2C / ldr r0,=gUnknown_08613CDC
 * in that order -- a source binding is emitted before every optimiser insn, so
 * the only way to get an address load AFTER the counter init is for the counter
 * init to be its own statement and both bases to be source bindings too. */

struct Unk08032BCCProc
{
    /* 0x00 */ u8 filler_00[0x58];
    /* 0x58 */ int unk58;
};

void LinkMapPick_Init(struct Unk08032BCCProc *proc)
{
    int i;
    u8 *p;
    const char **names;
    u8 buf[0x34];

    for (i = 0; i < 4; i++)
        gUnknown_0300449C[i] = 0;

    gUnknown_0849B060->unk0c = 0;

    DrawWindowBackgroundOnBg2(0x10, 1, 0xe, 4);
    BG_EnableSyncBG0();
    BG_EnableSyncBG2();
    LoadTilePoolGraphic(0x45);
    LoadTilePoolGraphic(0x46);
    ApplyPaletteExt(gUnknown_081D2224, 0xc0, 0x20);
    Decompress(gUnknown_081D2554, (void *)0x06006000);

    i = 0;
    p = gUnknown_02027C2C;
    names = gUnknown_08613CDC;

    for (; i < 3; i++)
    {
        if (LoadDesignRoomName((u8)i, buf) == 1)
        {
            CopyNameString16(p, buf);
            gUnknown_0300449C[i]++;
            gUnknown_0849B060->unk0c++;
        }
        else if (gUnknown_0849B060->unk09 != gUnknown_0849B018->unk06)
        {
            sub_0808B678((char *)p, *names);
            gUnknown_0300449C[i]++;
            gUnknown_0849B060->unk0c++;
        }

        p += 0x13;
    }

    for (i = 2; i >= 0; i--)
    {
        if (gUnknown_0300449C[i] != 0)
        {
            gUnknown_0849B060->unk04 = i;
            gUnknown_0849B060->unk06 = i;
        }
    }

    gUnknown_0849B060->unk10 = 1;
    LinkMapPick_Draw();
    gUnknown_0849B060->unk10 = 0;
    gUnknown_0849B060->unk0e = 0;
    LinkMapPick_SetupWindow();
    LinkMapPick_DrawPreview();
    gUnknown_0849B060->unk0a = 0;

    Proc_Start(gUnknown_0849B670, 0);

    SetVCountCompareLine(0);
    SetVCountInterruptHandler((int)LinkMapPick_OnVCountTop);

    proc->unk58 = 0;
}
asm(".global sub_08032BCC\n.thumb_set sub_08032BCC, LinkMapPick_Init\n");
