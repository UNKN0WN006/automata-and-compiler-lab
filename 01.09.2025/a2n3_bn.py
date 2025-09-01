# CFG for a^(2n+3) b^n | n >= 0
# V = {S}
# Σ = {a, b}
# P = {S → aaSb, S → aaa}
# S = S

from graphviz import Digraph

class Tree:
    def __init__(self):
        self.g = Digraph()
        self.n = 0
    def node(self, label):
        i = f"n{self.n}"
        self.g.node(i, label)
        self.n += 1
        return i
    def edge(self, p, c):
        self.g.edge(p, c)
    def save(self, name):
        self.g.render(name, view=False, format='png')

def check(s, cur, t, par=None):
    print("Now:", cur)
    if cur == s:
        return True
    if len(cur.replace('S', '')) > len(s):
        return False
    i = cur.find('S')
    if i == -1:
        return False
    # S -> aaSb
    ns = cur[:i] + 'aaSb' + cur[i+1:]
    na = t.node('aaSb')
    if par: t.edge(par, na)
    if check(s, ns, t, na): return True
    # S -> aaa
    ns = cur[:i] + 'aaa' + cur[i+1:]
    naaa = t.node('aaa')
    if par: t.edge(par, naaa)
    if check(s, ns, t, naaa): return True
    return False

if __name__ == "__main__":
    s = input("Enter string: ")
    t = Tree()
    r = t.node('S')
    if check(s, 'S', t, r):
        print("Accepted")
        t.save("tree_a2n3_bn")
        print("Parse tree saved as tree_a2n3_bn.png")
    else:
        print("Not Accepted")
        