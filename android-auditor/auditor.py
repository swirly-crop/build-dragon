#!/usr/bin/env python3
"""
Android Privacy Auditor - uses adb to collect basic device diagnostics for privacy audits.

This script intentionally reads information only; it does not change device state.
"""

import argparse
import subprocess
import sys

def run_adb(cmd):
    try:
        out = subprocess.check_output(['adb'] + cmd, stderr=subprocess.STDOUT, text=True)
        return out
    except FileNotFoundError:
        print('adb not found. Install Android Platform Tools and ensure adb is in PATH.', file=sys.stderr)
        sys.exit(2)
    except subprocess.CalledProcessError as e:
        return e.output

def devices():
    out = run_adb(['devices', '-l'])
    lines = [l for l in out.splitlines() if l.strip() and 'List of devices' not in l]
    devs = []
    for l in lines:
        if 'device' in l:
            devs.append(l.strip())
    return devs

def list_packages(third_party=False):
    if third_party:
        cmd = ['shell', 'pm', 'list', 'packages', '-3']
    else:
        cmd = ['shell', 'pm', 'list', 'packages']
    out = run_adb(cmd)
    pkgs = [line.split(':',1)[1].strip() for line in out.splitlines() if line.startswith('package:')]
    return pkgs

def get_permissions(pkg):
    out = run_adb(['shell', 'dumpsys', 'package', pkg])
    perms = []
    collect = False
    for line in out.splitlines():
        if 'grantedPermissions:' in line:
            collect = True
            continue
        if collect:
            if not line.strip():
                break
            perms.append(line.strip())
    return perms

def network_connections():
    # try ss, fallback to netstat
    for cmd in (['shell','ss','-tunp'], ['shell','netstat','-anp']):
        out = run_adb(cmd)
        if out and 'not found' not in out.lower():
            return out
    return ''

def main():
    parser = argparse.ArgumentParser(description='Android Privacy Auditor')
    parser.add_argument('--list-packages', action='store_true', help='List installed packages')
    parser.add_argument('--third-party', action='store_true', help='Only list third-party packages')
    parser.add_argument('--permissions', metavar='PKG', help='Show granted permissions for a package')
    parser.add_argument('--devices', action='store_true', help='List connected adb devices')
    parser.add_argument('--net', action='store_true', help='Show network connections')
    args = parser.parse_args()

    if args.devices:
        for d in devices():
            print(d)
    if args.list_packages:
        for p in list_packages(third_party=args.third_party):
            print(p)
    if args.permissions:
        perms = get_permissions(args.permissions)
        if perms:
            print('\n'.join(perms))
        else:
            print(f'No grantedPermissions block found for {args.permissions}', file=sys.stderr)
    if args.net:
        out = network_connections()
        if out:
            print(out)
        else:
            print('Could not obtain network connections (ss/netstat missing).', file=sys.stderr)

if __name__ == '__main__':
    main()
