
## wave 95

Base: the old draft (kept as `sub_08046914.w95-start.c`, 14.95%, size -12, first difference +0xe). Draft left as that file.

Pre-registered step (split the 0x8000 / 0 constants into their own locals): tried. `hi = 0x8000; zero = 0;` used in calls 1-3 only and literals after: size -12 -> -8, 12.5%, first difference +0x12 (kept as `sub_08046914.w95-hi.c`). The ROM does not create the constants at the top; it makes them inside the first call's argument setup (after the `.rodata`-word load) and keeps them in sb/sl for calls 1-3, and the compiler hoists the locals above the divide instead. Constants used through ALL calls: size -20 (worse). `(u32)` casts on the two `(a + 0x50)/8` and `(a + 0x60)/8` divides: -20. A `static inline u16 *TextBuf(void)` helper for the calls after the first `if`: -20, 7.9%. `u16 **pp = &gUnknown_08499578` bound before the else arms and `(*pp)` there: -24.

What the diff shows the ROM doing: calls 1-3 reach the pointer variable through the `.rodata` word (held in r8, reloaded per call), but the else arms and the later calls use a PLAIN `ldr =gUnknown_08499578`. The draft (and every variant) unifies the later uses with the word-derived value. Same unexplained split as sub_08046030's block A; none of the levers tried creates it in either function.

Permuter (two chained runs from the `hi` variant): reports 11.41 -> 68.21 -> 68.75%, size-exact. THESE ARE WRONG C AND MUST NOT BE USED: the kept file (`sub_08046914.w95-perm1.c`) turns `/ 2` into `x = 2; x = f(gfx) / x` which compiles to a `bl __divsi3` call instead of `lsrs; adds; asrs`, drops the `(s16)` cast, adds a `volatile int pad` local (second run) and reuses `zero = t` in the loop. The size-exact score is an accident of the call replacing the shift sequence. Read the diff, not the banner: the first difference is still at +0x12.

Proposed summary:
- does: lays out one unit's info panel: the portrait, two text rows, an optional third, and a per-terrain icon
- status: 14.95% at -12 bytes
- left: the ROM reaches the text-buffer pointer through a `.rodata` word for the first three calls and through a plain literal for the calls after the first `if`; the draft reuses one value for all and so never spills the way the ROM does
- tried: constant locals for the first three calls (-8), for all calls (-20), inline text-buffer helper (-20), `&gUnknown_08499578` bind (-24), unsigned casts on the divides (-20); permuter's 68% result is wrong C (calls __divsi3)
