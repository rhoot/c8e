c8e
===

[CHIP-8] emulator. Written for kicks and giggles.

Currently builds and runs on macOS and Windows.

Building
--------

Uses [GENie] for project generation.

### macOS

```bash
c8e$ genie gmake
c8e$ make -j -C .build/prj config=release
c8e$ .build/out/c8e programs/bc_test.c8
```

### Windows

```bash
c8e$ genie vs2017
c8e$ msbuild /p:Configuration=Release /p:Platform=x64 .build/prj/c8e.sln
c8e$ .build/out/c8e programs/bc_test.c8
```

Usage
-----

```
c8e: Yet another CHIP-8 emulator.

Usage:

  c8e --help
  c8e [--log] [--step] <c8_path>

Arguments:

  --help    Show this help and exit.
  --log     Print every instruction executed to stdout.
  --step    Only cycle the CPU when space is pressed.
  c8_path   Path to the CHIP-8 ROM to run.

```

License
-------

Copyright (c) 2018, Johan Sköld  
  
Permission to use, copy, modify, and/or distribute this software for any  
purpose with or without fee is hereby granted, provided that the above  
copyright notice and this permission notice appear in all copies.  
  
THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES  
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF  
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR  
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES  
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN  
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF  
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

[CHIP-8]: https://en.wikipedia.org/wiki/CHIP-8
