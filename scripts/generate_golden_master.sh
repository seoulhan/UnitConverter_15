#!/usr/bin/env bash
# Generates tests/golden_master_expected.txt by capturing UnitConverter stdout.
# Usage:
#   ./scripts/generate_golden_master.sh [path/to/unit_converter]

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXE="${1:-${ROOT}/build/unit_converter}"
EXPECTED="${ROOT}/tests/golden_master_expected.txt"
PROMPT_PREFIX="Insert value for converting"

if [[ ! -x "${EXE}" && ! -f "${EXE}" ]]; then
  if [[ -x "${EXE}.exe" ]]; then
    EXE="${EXE}.exe"
  else
    echo "Executable not found: ${EXE}" >&2
    echo "Build first: cmake -B build -S . && cmake --build build" >&2
    exit 1
  fi
fi

scenarios=( "meter:2.5" "feet:1.0" "yard:1.0" "meter:0.0" )

capture_scenario() {
  local input_line="$1"
  local tmp_out
  tmp_out="$(mktemp)"
  printf '%s\n' "${input_line}" | "${EXE}" >"${tmp_out}" 2>/dev/null || true
  grep -v "^${PROMPT_PREFIX}" "${tmp_out}" || true
  rm -f "${tmp_out}"
}

if [[ -f "${EXPECTED}" && "${FORCE:-0}" != "1" ]]; then
  echo "Baseline exists: ${EXPECTED}" >&2
  echo "Set FORCE=1 to overwrite after intentional output change." >&2
  exit 1
fi

{
  for i in "${!scenarios[@]}"; do
    scenario="${scenarios[$i]}"
    printf '[%s]\n' "${scenario}"
    body="$(capture_scenario "${scenario}")"
    if [[ -n "${body}" ]]; then
      printf '%s\n' "${body}"
    fi
    printf '%s' "---"
    if [[ "${i}" -lt $((${#scenarios[@]} - 1)) ]]; then
      printf '\n'
    fi
  done
  printf '\n'
} >"${EXPECTED}"

echo "Wrote ${EXPECTED}"
echo "Next: git add tests/golden_master_expected.txt"
