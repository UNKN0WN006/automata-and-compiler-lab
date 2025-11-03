n = int(input("Enter number of productions: "))
p = {}
for _ in range(n):
    s = input().replace(" ", "")
    l, r = s.split("->")
    p[l] = r.split("|")
nt = set(p.keys())
for rhs_list in p.values():
    for rhs in rhs_list:
        for c in rhs:
            if c.isupper(): nt.add(c)
for v in nt:
    if v not in p: p[v] = []
FIRST = {x:set() for x in nt}
FOLLOW = {x:set() for x in nt}
start = list(p.keys())[0]
FOLLOW[start].add('$')
changed = True
while changed:
    changed = False
    for A, alts in p.items():
        for rhs in alts:
            if rhs == 'ε' or rhs == '':
                if 'ε' not in FIRST[A]:
                    FIRST[A].add('ε'); changed = True
                continue
            nullable = True
            for i, sym in enumerate(rhs):
                if sym.isupper():
                    add = FIRST[sym] - {'ε'}
                    before = len(FIRST[A])
                    FIRST[A] |= add
                    if len(FIRST[A]) != before: changed = True
                    if 'ε' in FIRST[sym]:
                        nullable = True
                    else:
                        nullable = False
                        break
                else:
                    if sym not in FIRST[A]:
                        FIRST[A].add(sym); changed = True
                    nullable = False
                    break
            if nullable:
                if 'ε' not in FIRST[A]:
                    FIRST[A].add('ε'); changed = True
changed = True
while changed:
    changed = False
    for A, alts in p.items():
        for rhs in alts:
            for i, B in enumerate(rhs):
                if not B.isupper(): continue
                beta = rhs[i+1:]
                first_beta = set()
                if not beta:
                    first_beta.add('ε')
                else:
                    all_e = True
                    for sym in beta:
                        if sym.isupper():
                            first_beta |= (FIRST[sym] - {'ε'})
                            if 'ε' in FIRST[sym]:
                                all_e = True
                            else:
                                all_e = False
                                break
                        else:
                            first_beta.add(sym)
                            all_e = False
                            break
                    if all_e: first_beta.add('ε')
                before = len(FOLLOW[B])
                FOLLOW[B] |= (first_beta - {'ε'})
                if 'ε' in first_beta or not beta:
                    FOLLOW[B] |= FOLLOW[A]
                if len(FOLLOW[B]) != before: changed = True
print("\nFIRST sets:")
for k in sorted(nt):
    print(f"FIRST({k}) = {FIRST[k]}")
print("\nFOLLOW sets:")
for k in sorted(nt):
    print(f"FOLLOW({k}) = {FOLLOW[k]}")
