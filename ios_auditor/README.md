# iOS Privacy Auditor (Swift)

Non-destructive, owner-only diagnostics using `libimobiledevice` tools.

Requirements
- macOS or Linux with `libimobiledevice` installed (`idevice_id`, `ideviceinfo`).
- Swift toolchain to build the CLI.

Build & run (example):

```bash
swiftc android-auditor/ios_auditor/main.swift -o android-auditor/ios_auditor/ios_auditor
./android-auditor/ios_auditor/ios_auditor --devices
./android-auditor/ios_auditor/ios_auditor --info <UDID>
```

Notes
- This tool only queries devices you connect and does not modify device state.
- Use results to check for unknown device profiles, provisioning, or unexpected installed apps (when available).