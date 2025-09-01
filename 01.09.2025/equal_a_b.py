# CFG for n(wa) = n(wb)
# V = {S}
# Σ = {a, b}
# P = {S → aSb, S → bSa, S → ε}
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

def check(s, t):
    na = s.count('a')
    nb = s.count('b')
    root = t.node(s)
    info = t.node(f"a={na}, b={nb}")
    t.edge(root, info)
    return na == nb

if __name__ == "__main__":
    s = input("Enter string: ")
    t = Tree()
    if check(s, t):
        print("Accepted")
        t.save("tree_eq_ab")
        print("Parse tree saved as tree_eq_ab.png")
    else:
        print("Not Accepted")