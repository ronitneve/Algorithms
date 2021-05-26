import sys  # To extract cmd line arguments and for exit func
from functools import total_ordering


@total_ordering
class node:  # This is the data structure for the node.
    def __init__(self, parent, state, g, d, f, uSearch):
        self.parent = parent  # parent node of this node
        self.state = state  # state this node represents
        self.g = g  # Cumulative cost of reaching this node from root node: g(n)
        self.d = d  # depth of node in the search tree
        self.f = f  # Evaluation value of the current node: f(n) = g(n) + h(n) (Only used by informed search)
        self.uSearch = uSearch  # this is set to true for Uninformed search and false for Informed search

    def __str__(
        self,
    ):
        # this is used to convert a node to string format if necessary (for dumping the fringe)
        if self.uSearch:
            return (
                self.state
                + ": g(n) = "
                + str(self.g)
                + ", d = "
                + str(self.d)
                + ", Parent --> {"
                + str(self.parent)
                + "}"
            )
        else:
            return (
                self.state
                + ": g(n) = "
                + str(self.g)
                + ", d = "
                + str(self.d)
                + ", f(n) = "
                + str(self.f)
                + ", Parent --> {"
                + str(self.parent)
                + "}"
            )

    def __eq__(self, other):
        if self.uSearch:
            return self.g == other.g
        else:
            return self.f == other.f

    def __ne__(self, other):
        if self.uSearch:
            return not (self.g == other.g)
        else:
            return not (self.f == other.f)

    def __lt__(self, other):
        if self.uSearch:
            return self.g < other.g
        else:
            return self.f < other.f


def dump(fringe, closed):  # dumps both fringe and closed to std output
    print("Fringe: ")
    for n in fringe:
        print("\t" + str(n))
    print("Closed: \n\t" + str(closed) + "\n")


def expand(
    n, m, h
):  # uses map and heuristic (if needed) to generate list of successor nodes for a given node
    succ = []
    acts = m[n.state]
    for a in acts:
        cCost = n.g + a[1]
        if n.uSearch:
            succ.append(
                node(n, a[0], cCost, n.d + 1, 0, n.uSearch)
            )  # uninformed search does not use it so set f to 0
        else:
            succ.append(node(n, a[0], cCost, n.d + 1, cCost + h[a[0]], n.uSearch))
    return succ


def readInput(
    fName,
):
    # Reads the input file and creates a dictionary. Stops on reaching line END OF INPUT
    f = open(fName, "r")
    m = {}
    for l in f:
        l = l.rstrip(
            "\n"
        )  # remove newline or carriage return symbols that may have been read
        l = l.rstrip("\r")
        if l == "END OF INPUT":
            return m  # return dictionary of possible actions from every state
        else:
            v = l.split(" ")
            if v[0] in m:  # handle city 1 --> city 2
                m[v[0]].append([v[1], float(v[2])])
            else:
                m[v[0]] = [[v[1], float(v[2])]]
            if v[1] in m:  # handle city 2 --> city 1
                m[v[1]].append([v[0], float(v[2])])
            else:
                m[v[1]] = [[v[0], float(v[2])]]
    print(
        "This part is unreachable. If you are seeing this check Input File Formatting"
    )


def readHeuristic(
    fName,
):  # Reads the heuristic file and creates a dictionary. Stops on reaching line END OF INPUT
    f = open(fName, "r")
    h = {}
    for l in f:
        l = l.rstrip(
            "\n"
        )  # remove newline or carriage return symbols that may have been read
        l = l.rstrip("\r")
        if l == "END OF INPUT":
            return h  # return dictionary of heuristic values for every state
        else:
            v = l.split(" ")
            h[v[0]] = float(v[1])
    print(
        "This part is unreachable. If you are seeing this check Heuristic File Formatting"
    )


def genRoute(n, m):  # reconstruct the path from node
    dist = n.g
    steps = []
    while n is not None:
        p = n.parent
        if p is not None:
            act = (
                a for a in m[p.state] if a[0] == n.state
            )  # of all the actions of parent find the one that will obtain current state
            a = next(act)
            steps.append(p.state + " to " + n.state + ", " + str(a[1]) + " km")
        n = p
    steps.reverse()  # since links go from goal to start, reverse the list
    print("distance: " + str(dist) + " km")
    for s in steps:
        print(s)


def main():

    dumpValues = False  # set this to True to dump fringe and closed after every loop.

    # figure out if doing uninf or inf search
    l = len(sys.argv)
    if l == 4:
        if dumpValues:
            print("Uninformed Search selected\n")
        uSearch = True
    elif l == 5:
        if dumpValues:
            print("Informed Search selected\n")
        uSearch = False
    else:
        print("Incorrect number of arguments\n")
        sys.exit()
    # read and store the input file as dictionary
    map = readInput(sys.argv[1])
    h = {}
    if not uSearch:
        # for inf search read and store heuristic as dictionary
        h = readHeuristic(sys.argv[4])
    # initialize fringe with start node and closed as empty
    fringe = []
    if uSearch:
        fringe.append(node(None, sys.argv[2], 0, 0, 0, uSearch))
    else:
        fringe.append(node(None, sys.argv[2], 0, 0, h[sys.argv[2]], uSearch))
    closed = []
    nExp = 0  # number of nodes Expanded. (No nodes expanded yet)
    nGen = (
        1  # number of nodes Generated (Initial node was generated and added to fringe).
    )

    if dumpValues:
        print("Nodes Expanded: " + str(nExp))
        print("Nodes Generated: " + str(nGen))
        dump(fringe, closed)
    # main loop of search
    while len(fringe) > 0:
        nExp = nExp + 1
        if dumpValues:
            print("Expanding Node: " + str(nExp))
        # take node with cheapest g/f value
        n = fringe.pop(0)
        if n.state == sys.argv[3]:  # check if goal state
            if dumpValues:
                print("Goal Found: " + str(n) + ". Generating Output\n")
            print("nodes expanded: " + str(nExp))
            print("nodes generated: " + str(nGen))
            genRoute(n, map)
            sys.exit()
        else:
            if n.state not in closed:  # check if state already in closed
                if dumpValues:
                    print("Generating successors to " + n.state)
                closed.append(n.state)
                succ = expand(n, map, h)  # generate successors
                for s in succ:
                    nGen = nGen + 1
                    fringe.append(s)  # add successors to queue
                fringe.sort()  # sort the queue on the basis of g/f
            else:
                if dumpValues:
                    print(n.state + " is already in closed. No successors")
        if dumpValues:
            print("Nodes Expanded: " + str(nExp))
            print("Nodes Generated: " + str(nGen))
            dump(fringe, closed)
    # following code only runs if Fringe is empty i.e. no path possible.
    if dumpValues:
        print("Fringe Empty. Goal Not Found. Generating Output\n")
    print("nodes expanded: " + str(nExp))
    print("nodes generated:" + str(nGen))
    print("distance: infinity\nroute:\nnone")


if __name__ == "__main__":
    main()
