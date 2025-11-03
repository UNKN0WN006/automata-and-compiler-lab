n = int(input("Enter number of productions: "))
p = {}
for _ in range(n):
    s = input().replace(" ", "")
    l, r = s.split("->")
    p[l] = r.split('|')

nt = set(p.keys())
t = set()
for v in p.values():
    for x in v:
        for c in x:
            if not c.isupper() and c != 'ε':
                t.add(c)

f = {x: set() for x in nt}
fl = {x: set() for x in nt}
start = list(p.keys())[0]

def first(x):
    if x in t: return {x}
    r = set()
    for y in p.get(x, []):
        if y == 'ε': r.add('ε')
        else:
            for s in y:
                r |= first(s) - {'ε'}
                if 'ε' not in first(s): break
            else: r.add('ε')
    return r

def follow(x):
    r = set()
    if x == start: r.add('$')
    for a, b in p.items():
        for y in b:
            if x in y:
                i = y.index(x)
                beta = y[i+1:]
                if beta:
                    fb = first(beta[0])
                    r |= fb - {'ε'}
                    if 'ε' in fb: r |= follow(a)
                else: r |= follow(a)
    return r

for k in nt:
    f[k] = first(k)
for k in nt:
    fl[k] = follow(k)

print("\nFIRST sets:")
for k in nt: print(f"FIRST({k}) = {f[k]}")
print("\nFOLLOW sets:")
for k in nt: print(f"FOLLOW({k}) = {fl[k]}")
