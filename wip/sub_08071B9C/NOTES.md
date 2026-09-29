# sub_08071B9C

## wave 97

Base: draft (18.75%, 240 B, +8). No match.
- Fully inlined (no a/b/ar.. locals, `data->unk00[i] & 0x1f` at each call): 220 B (-12), 21.55%, frame `sub sp,#16` matches. Locals for a and b only: frame grows to 0x14. Commuted masks (`0x1f & a`), u32/u16 type for b, `(b & 0xff) & 0x1f`, `% 32`: no change or worse.
- Mechanism seen in the ROM: all six masks are made before the calls, and 0x1f is rematerialised for the b group (`movs r2,#31`) as well as at the tail (`movs r3,#31`), so regmove can swap the commutative `ands` onto the constant register. cse shares 0x1f across the a and b groups in the draft, which forces the copies. Something must end the constant's life between the groups; nothing tried does.
