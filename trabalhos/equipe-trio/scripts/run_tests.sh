#!/usr/bin/env bash
root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
exe="${1:-}"
test_dirs=("$root/tests" "$root/../../testes")

if [ -z "$exe" ]; then
    make -C "$root" build >&2 || { echo "Build failed."; exit 1; }
    exe="$root/programa"
fi

ok=0
total=0
for dir in "${test_dirs[@]}"; do
    [ -d "$dir" ] || continue
    for input in $(ls "$dir"/*.in 2>/dev/null | sort); do
        expected="${input%.in}.out"
        [ -f "$expected" ] || continue
        total=$((total + 1))
        if "$exe" "$input" | tr -d '\r' | cmp -s - <(tr -d '\r' < "$expected"); then
            ok=$((ok + 1))
            echo "OK     $(basename "$input")"
        else
            echo "FAILED $(basename "$input")"
        fi
    done
done

echo "$ok/$total tests passed"
[ "$ok" -eq "$total" ]
