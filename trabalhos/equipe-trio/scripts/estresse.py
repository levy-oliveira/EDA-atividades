#!/usr/bin/env python3
"""Teste de estresse: compara o programa com um gabarito ingenuo.

O gabarito guarda uma lista inteira por versao (O(n) por versao), exatamente
o que o trabalho proibe -- por isso mesmo serve de referencia confiavel.

Uso (dentro de trabalhos/equipe-trio):
    python scripts/estresse.py                    # 300 casos, usa ./programa
    python scripts/estresse.py -n 1000 -e programa_debug
    python scripts/estresse.py --salvar-falhas    # grava casos que falharem em tests/
"""
import argparse
import os
import random
import subprocess
import sys
import tempfile


def referencia(linhas):
    versoes = [[]]
    saida = []

    def norm(m):
        return len(versoes) - 1 if (m < 0 or m >= len(versoes)) else m

    for linha in linhas:
        p = linha.split()
        if not p:
            continue
        if p[0] == "INC":
            versoes.append(sorted(versoes[-1] + [int(p[1])]))
        elif p[0] == "REM":
            v = list(versoes[-1])
            if int(p[1]) in v:
                v.remove(int(p[1]))
            versoes.append(v)
        elif p[0] == "SUC":
            saida.append(linha)
            v = versoes[norm(int(p[2]))]
            s = next((x for x in v if x > int(p[1])), None)
            saida.append("inf" if s is None else str(s))
        elif p[0] == "IMP":
            saida.append(linha)
            saida.append(", ".join(map(str, versoes[norm(int(p[1]))])))
    return "\n".join(saida) + "\n"


def gerar(rng, n_ops, faixa):
    """Mistura modificacoes e consultas (inclusive a versoes antigas e inexistentes)."""
    linhas, mods = [], 0
    while mods < n_ops:
        r = rng.random()
        if r < 0.55:
            linhas.append(f"INC {rng.randint(0, faixa)}"); mods += 1
        elif r < 0.85:
            linhas.append(f"REM {rng.randint(0, faixa)}"); mods += 1
        elif r < 0.93:
            linhas.append(f"SUC {rng.randint(0, faixa)} {rng.randint(0, mods + 2)}")
        else:
            linhas.append(f"IMP {rng.randint(0, mods + 2)}")
    # consulta final em todas as versoes, para pegar qualquer erro em versoes antigas
    for v in range(0, mods + 1):
        linhas.append(f"IMP {v}")
    return linhas


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("-n", "--casos", type=int, default=300)
    ap.add_argument("-e", "--exe", default="./programa")
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument("--salvar-falhas", action="store_true")
    args = ap.parse_args()

    falhas = 0
    with tempfile.TemporaryDirectory() as tmp:
        for caso in range(args.casos):
            rng = random.Random(args.seed * 100003 + caso)
            n_ops = rng.choice([5, 20, 60, 200, 1000])
            faixa = rng.choice([2, 5, 10, 40, 1000, 65535])
            linhas = gerar(rng, n_ops, faixa)
            entrada = "\n".join(linhas) + "\n"
            esperado = referencia(linhas)
            arq = os.path.join(tmp, "entrada.in")
            with open(arq, "w", newline="\n") as f:
                f.write(entrada)
            r = subprocess.run([args.exe, arq], capture_output=True, text=True)
            if r.returncode != 0 or r.stdout != esperado:
                falhas += 1
                print(f"FALHOU caso {caso} (ops={n_ops}, faixa={faixa}, rc={r.returncode})")
                if r.stderr:
                    print(r.stderr[:600])
                if args.salvar_falhas:
                    base = os.path.join("tests", f"estresse_falha_{args.seed}_{caso}")
                    with open(base + ".in", "w", newline="\n") as f:
                        f.write(entrada)
                    with open(base + ".out", "w", newline="\n") as f:
                        f.write(esperado)
                    print(f"  salvo em {base}.in/.out")
    print(f"{args.casos - falhas}/{args.casos} casos ok")
    sys.exit(1 if falhas else 0)


if __name__ == "__main__":
    main()
