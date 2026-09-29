#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0801F0AC.
 * sub_0801F0AC @ 0x0801F0AC, sub_0801F0C8 @ 0x0801F0C8, sub_0801F0E0 @ 0x0801F0E0, sub_0801F0FC @ 0x0801F0FC
 */

/* Family F070, second member -- see BeginOamFrameForMode. */

void SyncLoOamForMode(void)
{
    if (gUnknown_03001FE0)
        SyncLoOam();
    else
        CopyLoOamShadowToOam();
}
asm(".global sub_0801F0AC\n.thumb_set sub_0801F0AC, SyncLoOamForMode\n");

/* Family F071, second member -- see BeginOamFrameForMode. Its callee ClearLoOamShadow is
 * the F069 member matched in the same batch. */

void ClearLoOamForMode(void)
{
    if (!gUnknown_03001FE0)
        ClearLoOamShadow();
}
asm(".global sub_0801F0C8\n.thumb_set sub_0801F0C8, ClearLoOamForMode\n");

/* Family F070, third member -- see BeginOamFrameForMode. */

void SyncHiOamForMode(void)
{
    if (gUnknown_03001FE0)
        SyncHiOam();
    else
        CopyHiOamShadowToOam();
}
asm(".global sub_0801F0E0\n.thumb_set sub_0801F0E0, SyncHiOamForMode\n");

/* Family F071, third member -- see BeginOamFrameForMode. */

void TickSimpleSpriteScriptsForMode(void)
{
    if (!gUnknown_03001FE0)
        TickSimpleSpriteScripts2();
}
asm(".global sub_0801F0FC\n.thumb_set sub_0801F0FC, TickSimpleSpriteScriptsForMode\n");
