#!/usr/bin/env bash
set -euo pipefail

ROOT=$(git rev-parse --show-toplevel || echo "$(pwd)")
cd "$ROOT"

echo "Running repository safety scan in $ROOT"

# Directories to exclude from scan
EXCLUDES=(--exclude-dir=.git --exclude-dir=.github --exclude-dir=build --exclude-dir=bin --exclude-dir=android-auditor/perf/build)

# Patterns that are considered high-risk. This is a conservative scanner and may produce false positives.
PATTERNS=(
  "\brootkit\b"
  "\bbackdoor\b"
  "\bstealth\b"
  "\bundetectable\b"
  "hide_process"
  "hide_file"
  "modify_boot"
  "bootloader"
  "baseband"
  "IMEI"
  "escalat(e|ion)"
  "setuid\("
  "setreuid\("
  "setresuid\("
  "ptrace\("
  "ptrace_attach"
  "AF_PACKET"
  "SOCK_RAW"
  "raw_socket"
  "iptables"
  "netfilter"
  "insmod\b"
  "rmmod\b"
  "load_module\b"
  "open\(\"/dev/"
  "bash -i >& /dev/tcp"
  "\\/dev\\/tcp\\/"
  "nc -e"
  "reverse shell"
  "curl .*http.*-o.*\\.sh"
  "wget .*http"
)

TOTAL=0
for p in "${PATTERNS[@]}"; do
  echo "Checking pattern: $p"
  # shellcheck disable=SC2086
  if grep -RInE $p . ${EXCLUDES[@]} ; then
    FOUND=$?
    # grep exit code 0 means matches found; we counted output displayed already
    TOTAL=$((TOTAL+1))
  fi
done

if [ "$TOTAL" -gt 0 ]; then
  echo
  echo "Safety scan detected potentially risky patterns ($TOTAL). Please review the findings above."
  echo "If these are false positives, update .github/safety/check_safety.sh to refine the pattern list."
  exit 1
else
  echo "Safety scan passed: no high-risk patterns found."
  exit 0
fi
