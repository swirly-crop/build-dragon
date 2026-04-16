# Swift Auditor Bridge

This folder contains small helper bridge components to let Swift call local tools (`libimobiledevice`) via C/C++/Objective-C glue.

Files:
- `bridge.h` - C header exposing functions
- `bridge.cpp` - C++ implementations that call `idevice_id` and `ideviceinfo` via `popen`
- `objc_wrapper.m` - Objective-C wrapper (C-callable) that runs `ideviceinfo`
- `assembly/hello_x86_64.s` - small assembly sample (educational)

Build (macOS/Linux):

```bash
clang -c bridge.cpp -o bridge.o
clang -c objc_wrapper.m -o objc_wrapper.o
clang -c assembly/hello_x86_64.s -o hello.o
ar rcs libbridge.a bridge.o objc_wrapper.o hello.o
```

Link into Swift executable (example):

```bash
swiftc -Xlinker libbridge.a ../swift_auditor/main.swift -o ios_auditor
```

Notes:
- These helpers only run tools you install (`libimobiledevice`).
- They are non-destructive and intended for device owners and defenders.
