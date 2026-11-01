#!/usr/bin/env python3
import argparse
import os
import random
import subprocess
import sys
import tempfile


def reference(lines):
    versions = [[]]
    output = []

    def norm(m):
        return len(versions) - 1 if (m < 0 or m >= len(versions)) else m

    for line in lines:
        p = line.split()
        if not p:
            continue
        if p[0] == "INC":
            versions.append(sorted(versions[-1] + [int(p[1])]))
        elif p[0] == "REM":
            v = list(versions[-1])
            if int(p[1]) in v:
                v.remove(int(p[1]))
            versions.append(v)
        elif p[0] == "SUC":
            output.append(line)
            v = versions[norm(int(p[2]))]
            s = next((x for x in v if x > int(p[1])), None)
            output.append("inf" if s is None else str(s))
        elif p[0] == "IMP":
            output.append(line)
            output.append(", ".join(map(str, versions[norm(int(p[1]))])))
    return "\n".join(output) + "\n"


def generate(rng, n_ops, key_range):
    lines, mods = [], 0
    while mods < n_ops:
        r = rng.random()
        if r < 0.55:
            lines.append(f"INC {rng.randint(0, key_range)}")
            mods += 1
        elif r < 0.85:
            lines.append(f"REM {rng.randint(0, key_range)}")
            mods += 1
        elif r < 0.93:
            lines.append(f"SUC {rng.randint(0, key_range)} {rng.randint(0, mods + 2)}")
        else:
            lines.append(f"IMP {rng.randint(0, mods + 2)}")
    for v in range(0, mods + 1):
        lines.append(f"IMP {v}")
    return lines


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", "--cases", type=int, default=300)
    ap.add_argument("-e", "--exe", default="./programa")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--save-failures", action="store_true")
    args = ap.parse_args()

    failures = 0
    with tempfile.TemporaryDirectory() as tmp:
        for case in range(args.cases):
            rng = random.Random(args.seed * 100003 + case)
            n_ops = rng.choice([5, 20, 60, 200, 1000])
            key_range = rng.choice([2, 5, 10, 40, 1000, 65535])
            lines = generate(rng, n_ops, key_range)
            input_text = "\n".join(lines) + "\n"
            expected = reference(lines)
            path = os.path.join(tmp, "input.in")
            with open(path, "w", newline="\n") as f:
                f.write(input_text)
            r = subprocess.run([args.exe, path], capture_output=True, text=True)
            if r.returncode != 0 or r.stdout != expected:
                failures += 1
                print(f"FAILED case {case} (ops={n_ops}, range={key_range}, rc={r.returncode})")
                if r.stderr:
                    print(r.stderr[:600])
                if args.save_failures:
                    base = os.path.join("tests", f"stress_failure_{args.seed}_{case}")
                    with open(base + ".in", "w", newline="\n") as f:
                        f.write(input_text)
                    with open(base + ".out", "w", newline="\n") as f:
                        f.write(expected)
                    print(f"  saved to {base}.in/.out")
    print(f"{args.cases - failures}/{args.cases} cases ok")
    sys.exit(1 if failures else 0)


if __name__ == "__main__":
    main()
