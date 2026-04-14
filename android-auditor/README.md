# Android Privacy Auditor

A small, open-source, non-stealth auditing tool that uses `adb` (Android Debug Bridge) to collect device information useful for privacy audits.

WARNING: This tool is for device owners and defenders only. Do not run against devices you do not own or have explicit permission to inspect.

## Requirements
- Python 3.8+
- Android Platform Tools (`adb`) installed and in PATH

## Installation

No packages required. Optionally create a virtualenv and install dependencies from `requirements.txt` (empty by default).

## Usage

Run from the repository root:

```bash
python3 android-auditor/auditor.py --devices
python3 android-auditor/auditor.py --list-packages --third-party
python3 android-auditor/auditor.py --permissions com.example.app
python3 android-auditor/auditor.py --net
```

The tool calls `adb` under the hood; ensure a device is connected and `adb devices` shows it.

## Notes
- This tool intentionally only reads diagnostics; it does not modify device state.
- Use results to harden settings (revoke unneeded permissions, uninstall suspicious apps, review network connections).
