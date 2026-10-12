# AGENT.md — Game Boy Emulator Project

Notes for future pi agents. This project is run in **learning/tutor mode**
(`/learn`), not autopilot mode. Read this before teaching or reviewing.

---

## Project

Building a Game Boy emulator in C, from scratch, as a learning vehicle.

- **Plan:** see `ROADMAP.md` — 12 numbered milestones, each with an observable
  "done when" condition. Follow that order unless Tyler says otherwise.
- **v1.0 target:** Tetris playable, sound off (milestones 0–11). APU and
  cycle-accurate mid-scanline PPU timing are explicitly out of scope for v1.
- **Current position:** milestone 0 (multi-file project setup). `main.c` exists
  but is empty. Nothing is built yet.

### Prior work — `/Users/Tyler/Documents/romheader`

A completed, working cartridge-header reader (~150 lines, single file). This was
the intro project and is where all the C learning below happened. It contains
`parse_header`, a `RomHeader` struct, and three `decode_*` lookup tables that
carry over to milestone 1 essentially unchanged. Worth reading for context on
Tyler's actual code style and level.

Known remaining nits in that file (discussed, deliberately left): `fread`'s
return value `n` isn't validated against `HEADER_END`, `main` has no `return 0;`,
and the "Raw Values" title line prints only the first byte of a 16-byte field.

---

## Learner profile

**Background:** comfortable in Python and Java. Has the general programming
fundamentals — control flow, functions, data structures, decomposition. The gap
is C-specific semantics, not programming ability.

**Learning style, observed:**
- Dives in and writes code before being asked. Often arrives with a half-working
  attempt rather than a question. Review the attempt first.
- Applies corrections thoroughly and often goes beyond what was asked (in one
  pass: fixed the requested bug, hoisted `#define`s to file scope, added an
  `argc` guard, deleted dead variables, renamed a badly-named macro).
- Asks good meta questions ("is this a good stopping point?") at genuinely
  appropriate moments. Take those seriously rather than deflecting.
- Will say plainly when something hasn't landed ("I'm honestly not sure on the
  types", "I don't think I know the syntax for that"). Trust these signals.

---

## C concepts covered (with evidence)

Confirmed through code written, bugs found, or questions answered correctly:

| Concept | Evidence |
|---|---|
| Pointers as addresses, not objects | `const char *`, `char *filename` |
| Storage duration: static vs. automatic | Correctly diagnosed a dangling-pointer scenario unprompted |
| Why `char[17]` beats `char *` for an owned string | Reasoned about it after the ROM buffer is freed |
| Fixed-width types / `stdint.h` | `uint8_t` throughout |
| Signedness traps | Explained why `int c` not `char c` with `fgetc`/`EOF`/`0xFF` |
| Sentinel error handling | `fopen` → `NULL`, `perror`, `argc` guard |
| Hidden stream state (file position) | **Found the `fread`/`fgetc` offset bug independently** |
| `switch` fall-through | Debugged 32 cases missing `return` |
| Format specifiers as interpretation | `%02X` vs `%c` vs `%s` |
| Structs: declare, fill, return by value, consume | Wrote `parse_header` end to end |
| File-scope rule (no nested functions) | Corrected after hitting it |
| Block scope vs. function scope | Corrected after hitting it (see below) |
| `typedef struct` and anonymous struct tags | Applied correctly |
| Undefined behavior as a *category* | Shift-count UB; still worth reinforcing |

### Known shaky areas

- **Scope vs. lifetime.** Got confused about needing to "pass data out of" a loop
  when the destination struct was already declared in the enclosing function.
  Resolved, but likely to recur in new forms. Diagnose by asking "which `{` was
  this declared after?"
- **Undefined behavior.** Initially explained out-of-range shift UB as "the value
  is too big." Understands it now, but the instinct to treat UB as merely "wrong
  value" rather than "compiler may do anything" isn't automatic yet.
- **Uninitialized memory.** Hit the "`break` leaves the rest of the array
  indeterminate" bug. Worth re-raising whenever a struct or buffer is partially
  written.

### Not yet covered — teach when reached

- `malloc` / `free`, heap ownership, leaks, double-free (**milestone 1**)
- `.h` / `.c` split, include guards, `extern`, Makefiles (**milestone 0**)
- `union`, and the byte-order catch for register pairs (**milestone 3**)
- Bitwise ops `& | ^ ~ << >>` used for flags and masking (**milestone 3**)
- Function pointers, `static` for internal linkage, `enum`
- Passing a pointer *and* a length together as a safety habit
- Debuggers (`lldb`) and sanitizers (`-fsanitize=address`)

---

## How to work with Tyler

**Do:**
- Walk code line by line when asked. He wants to understand every line, not just
  have working code.
- Review his actual attempt before teaching anything new. Read the file; don't
  give generic advice.
- Let him find bugs. He found the biggest one in the last project himself when
  given the right framing. Hint, then wait.
- Name *why* something is wrong, including the mechanism. "This is UB because the
  shift count exceeds the type width" beats "don't do that."
- Verify output against the spec. Checking that offset `0x104` held the Nintendo
  logo bytes was how he proved a fix worked. Keep that habit alive — it matters
  enormously once the PPU exists.
- Connect each C mechanic to where it reappears in the emulator.

**Don't:**
- Write milestone code for him. This is a learning project; the code is the point.
- Hand over a complete function when one illustrative line would unblock him.
- Skip prerequisites. Define terms before relying on them.
- Let him start milestone N+1 before N's "done when" is observably true — the
  roadmap's core principle is one unverified subsystem at a time.

---

## Conventions

- Tabs for indentation. Brace on the same line for functions in the existing code
  (`const char *decode_ram_size(uint8_t code) {`), though `main` uses the next
  line. Not strict about it.
- `#define` for hardware addresses, uppercase, at file scope.
- Hex as `0x13`. Note that Pandocs writes `$13` — he's hit that translation
  problem once already.
- Compile with `gcc -Wall -Wextra`. Reinforce that warnings are not errors and
  must still be read.
- Lookup tables as `switch` returning `const char *` string literals.
- Structs hold **raw data** (codes, bytes); decoding to display strings happens at
  print time. This was a deliberate design decision he made and understands.

---

## Open questions he's been asked but hasn't answered yet

- **Milestone 3:** `union` vs. two `uint8_t`s with shift/mask for `AF`/`BC`/`DE`/`HL`?
  The `union` approach has an endianness catch that's worth working through rather
  than being told.
