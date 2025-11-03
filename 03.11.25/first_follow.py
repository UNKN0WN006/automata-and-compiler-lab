import sys
s = sys.stdin.readline().strip()
n = int(sys.stdin.readline().strip())
P = {}
NT = set()
for _ in range(n):
    line = sys.stdin.readline().strip()
    if '->' not in line: continue
    a, rhs = line.split('->',1)
    a = a.strip()
    NT.add(a)
    toks = rhs.strip().split()
    # treat several forms of epsilon as $
    if len(toks)==0 or toks in (['ε'], ['e'], ['$'], ['eps'], ['epsilon']):
        toks = ['$']
    for tok in toks:
        if tok != '$' and tok and tok[0].isupper():
            NT.add(tok)
    P.setdefault(a,[]).append(toks)
FIRST = {A:set() for A in NT}
changed = True
def isNT(x): return x!='$' and x[0].isupper()
while changed:
    changed = False
    for A,alts in P.items():
        for alt in alts:
            allE = True
            for X in alt:
                if X=='$':
                    if '$' not in FIRST[A]:
                        FIRST[A].add('$'); changed = True
                    allE = True
                    break
                if isNT(X):
                    for t in FIRST[X]:
                        if t!='$' and t not in FIRST[A]:
                            FIRST[A].add(t); changed = True
                    if '$' in FIRST[X]:
                        allE = True
                        continue
                    else:
                        allE = False
                        break
                else:
                    if X not in FIRST[A]:
                        FIRST[A].add(X); changed = True
                    allE = False
                    break
            if allE:
                if '$' not in FIRST[A]:
                    FIRST[A].add('$'); changed = True
FOLLOW = {A:set() for A in NT}
FOLLOW[s].add('$')
def first_seq(seq):
    res=set(); allE=True
    for X in seq:
        if X=='$':
            res.add('$'); allE=True; break
        if isNT(X):
            for t in FIRST[X]:
                if t!='$': res.add(t)
            if '$' in FIRST[X]:
                allE=True; continue
            else:
                allE=False; break
        else:
            res.add(X); allE=False; break
    if allE: res.add('$')
    return res
changed = True
while changed:
    changed = False
    for A,alts in P.items():
        for alt in alts:
            m = len(alt)
            for i,X in enumerate(alt):
                if not isNT(X): continue
                beta = alt[i+1:] if i+1<m else []
                fs = first_seq(beta) if beta else set(['$'])
                for t in fs:
                    if t != '$' and t not in FOLLOW[X]:
                        FOLLOW[X].add(t); changed = True
                if ('$' in fs) or not beta:
                    for t in FOLLOW[A]:
                        if t not in FOLLOW[X]:
                            FOLLOW[X].add(t); changed = True
print("FIRST")
for A in sorted(NT):
    print(A,":",sorted(FIRST[A]))
print("FOLLOW")
for A in sorted(NT):
    print(A,":",sorted(FOLLOW[A]))