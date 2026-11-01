# build instructions
```bash
/bin/g++ momoi.cpp -o momoi
```

> [!TIP]
> if compiling for embedded/older environments, you could swap out a few to make this a valid C code.
> what to replace: new[], delete[] to malloc(), free() ; pass-by-reference to pass-by-pointer ; C++ wrappers to C headers ; nullptr to NULL (if standard below C23)

# how to
use it with common sense
there's no bug to fix in here
