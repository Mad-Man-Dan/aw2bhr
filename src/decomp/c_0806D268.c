#include "global.h"

/* Promoted from assembly; each function below is byte-for-byte
 * identical to the original. Order is address order and must
 * stay that way -- the linker places this file's .text as one
 * contiguous block at 0x0806D268.
 * sub_0806D268 @ 0x0806D268, sub_0806D34C @ 0x0806D34C
 */

/* The install half of RulesScreenDismissRuleOptions's teardown: seven sub_080152EC(3) objects
 * into gUnknown_08580934->unk54[], each wired up from five parallel
 * seven-entry ROM tables. See the gUnknown_085809B4 comment in
 * include/unknown-globals.h for the tables and for why two of them come
 * through agbcc's own -fforce-addr words (0x0816E18C / 0x0816E190) while the
 * other three get a plain literal-pool `ldr`.
 *
 * `a` and `b` are SOURCE locals, not strength-reduced induction variables, and
 * the preheader is what says so: `movs r6,#7; movs r5,#0x54; movs r4,#0` sit
 * BEFORE the LICM-hoisted pool addresses, and the source's own initialisations
 * are the only thing that goes there (docs/agbcc-codegen.md, "The preheader is
 * written by three passes in a fixed order"). Spelled the arithmetic way --
 * `unk2c = i * 0x20 + 7; unk2a = 0x54 - i * 8;` -- agbcc recomputes both from
 * `i` inside the loop and, with the two registers it frees, force-addrs three
 * more tables that the ROM reaches directly. Probed both ways. */
void RulesScreenSpawnRuleOptions(void)
{
    struct Unk08580934 *g;
    struct Unk08580934_Obj *o;
    u8 *p;
    int i;
    int a;
    int b;

    a = 7;
    b = 0x54;

    for (i = 0; i <= 6; i++)
    {
        o = (struct Unk08580934_Obj *)sub_080152EC(gUnknown_08581E94, 3);
        gUnknown_08580934->unk54[i] = o;
        o->unk46 = 0;
        o->unk2c = a;
        o->unk2a = b;
        a += 0x20;
        b -= 8;
        o->unk24 = 6 - i;
        o->unk3e = 0x6000 + gUnknown_0816E0D0[i];
        o->unk40 = 0x6000 + gUnknown_0816E0DE[i];
        o->unk4b = gUnknown_085809A4[i];
        p = &gUnknown_08580934->unk84;
        o->unk48 = p[i];
        o->unk4c = gUnknown_085809B4[i];
        o->unk47 = gUnknown_085809D0[i];
    }

    g = gUnknown_08580934;
    g->unk54[4]->unk4b = g->unk16 - g->unk15 + 2;
}
asm(".global sub_0806D268\n.thumb_set sub_0806D268, RulesScreenSpawnRuleOptions\n");

/* MatchSetupDismissRuleOptions's twin -- byte-exact prologue, gate and tail, and the same
 * seven-iteration walk over unk54[]. The only differences are the blob
 * (0x08581E94, reached through the force-addr pool word at 0x0816E194) and the
 * last two stores: a constant 5 into the WORD unk34 and 0 into the word unk30,
 * where MatchSetupDismissRuleOptions writes its parameter into the HALFWORD unk3c and 0 into
 * unk3a. Different members, not the same field spelled differently, and this
 * one takes no parameter. */
void RulesScreenDismissRuleOptions(void)
{
    struct Unk08580934 *g;
    struct Unk08580934_Obj *o;
    u8 *p;
    int i;

    if (FindSlotScript((s32)gUnknown_08581E94) != -1)
    {
        for (i = 0; i <= 6; i++)
        {
            g = gUnknown_08580934;
            o = g->unk54[i];
            p = &g->unk84;
            p[i] = o->unk48;
            o->unk24 = 6 - i;
            o->unk34 = 5;
            o->unk30 = 0;
        }
        ClearCallbackOfSlotsRunningScript(gUnknown_08581E94);
    }
}
asm(".global sub_0806D34C\n.thumb_set sub_0806D34C, RulesScreenDismissRuleOptions\n");
