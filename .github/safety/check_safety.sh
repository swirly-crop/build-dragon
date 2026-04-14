#!/usr/bin/env bash
set -euo pipefail

ROOT=$(git rev-parse --show-toplevel || echo "$(pwd)")
cd "$ROOT"

echo "Running repository safety scan in $ROOT"

# Directories to exclude from scan
EXCLUDES=(--exclude-dir=.git --exclude-dir=.github --exclude-dir=build --exclude-dir=bin --exclude-dir=android-auditor/perf/build)

# Files or paths to exclude by default (docs, READMEs)
PATH_EXCLUDES=("README.md" "*.md" "android-auditor/*.md" "ios_auditor/*.md")

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

# Optional allowlist file: lines are literal filename patterns or full file paths to ignore
ALLOWFILE=.github/safety/allowlist.txt
ALLOW_PATTERNS=()
if [ -f "$ALLOWFILE" ]; then
  mapfile -t ALLOW_PATTERNS < "$ALLOWFILE"
fi

TOTAL=0
for p in "${PATTERNS[@]}"; do
  echo "Checking pattern: $p"
  # Use -e to allow patterns with spaces/special chars; quote variables
  # Build grep exclude args for path excludes
  GREP_EXCLUDES=()
  for ex in "${EXCLUDES[@]}"; do GREP_EXCLUDES+=("$ex"); done
  for ex in "${PATH_EXCLUDES[@]}"; do GREP_EXCLUDES+=(--exclude="$ex"); done

  # Run grep and capture matches
  if matches=$(grep -RInE -e "$p" . "${GREP_EXCLUDES[@]}" || true); then
    if [ -n "$matches" ]; then
      # Filter allowlist: remove matches that are in allow patterns
      filtered="$matches"
      for a in "${ALLOW_PATTERNS[@]}"; do
        if [ -n "$a" ]; then
          filtered=$(echo "$filtered" | grep -v -- "$a" || true)
        fi
      done
      if [ -n "$filtered" ]; then
        echo "$filtered"
        TOTAL=$((TOTAL+1))
      fi
    fi
  fi
done

if [ "$TOTAL" -gt 0 ]; then
  echo
  echo "Safety scan detected potentially risky patterns ($TOTAL). Please review the findings above."
  echo "If these are false positives, add allowed files to $ALLOWFILE or refine PATTERNS in .github/safety/check_safety.sh."
  exit 1
else
  echo "Safety scan passed: no high-risk patterns found."
  exit 0
fi
