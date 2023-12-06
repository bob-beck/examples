from typing import List
import hashlib

# We've been talking about a Merkle Tree. Here's a very very basic example
# constructed with the inspiration of several internet sources.
# This example uses sha256 for the hashes in the tree. 

class Node:
    def __init__(self, left, right, value: str, cert:str)-> None:
        self.left: Node = left
        self.right: Node = right
        self.value = value  # The hash value of this node of the tree.
        self.cert = cert    # If this node is a leaf, this is the 'certificate' data that was hashed.

    @staticmethod
    def sha256(val: str)-> str:
        return hashlib.sha256(val.encode('utf-8')).hexdigest()

class MerkleTree:
    def __init__(self, values: List[str])-> None:
        self.__buildTreeFromValues(values)

    def __buildTreeFromValues(self, values: List[str])-> None:
        leaves: List[Node] = [Node(None, None, Node.sha256(e), e) for e in values]
        # Add a special element "PADDING" if we have an odd number of leaves.
        if len(leaves) % 2 == 1:
            leaves.append(Node(None, None, Node.sha256("PADDING"), "PADDING"))
        self.root: Node = self.__buildTree(leaves)

    def __buildTree(self, nodes: List[Node])-> Node:
        if len(nodes) % 2 == 1:
            nodes.append(Node(None, None, Node.sha256("PADDING"), "PADDING"))
        split: int = len(nodes) // 2
        if len(nodes) == 2:
            return Node(nodes[0], nodes[1], Node.sha256(nodes[0].value + nodes[1].value), "")
        left: Node = self.__buildTree(nodes[:split])
        right: Node = self.__buildTree(nodes[split:])
        value: str = Node.sha256(left.value + right.value)
        return Node(left, right, value, "")

    def printTree(self)-> None:
        self.__printTreePreorder(self.root)

    def __printTreePreorder(self, node)-> None:
        if node != None:
            print("Node Value: "+str(node.value))
            if node.left != None:
                print("     Left:  "+str(node.left.value))
                print("     Right: "+str(node.right.value))
            else:
                print ("     Cert:  "+str(node.cert))
            self.__printTreePreorder(node.left)
            self.__printTreePreorder(node.right)

    def getRootHash(self)-> str:
        return self.root.value

class MerkleTreeProof:
    def __init__(self, root: str) -> None:
        self.root = root

    def isValueInTree(self, cert: str, intermediateHashes: List[str]) -> bool:
        # This is unimplemented and always returns false.  You should try to
        # implement this method. Don't forget to hash "cert"  to get the
        # starting hash value.
        return False;

    def getRootHash(self)-> str:
        return self.root

elems = [ "Bob signed cert for google.com", "Alice signed cert for microsoft.com", "Paul signed cert for ualberta.ca", "Mallory signed cert for google.com" ]
mtree = MerkleTree(elems)
print("Root of tree : " +str(mtree.getRootHash()))
print("------")
mtree.printTree()

print("If we change anything in the tree, the root changes")
elems = [ "Bob signed cert for gmail.com", "Alice signed cert for microsoft.com", "Paul signed cert for ualberta.ca", "Mallory signed cert for google.com" ]
mtree2 = MerkleTree(elems)
print("New Root of tree : " +str(mtree2.getRootHash()))
print("------")
mtree2.printTree()

print("------")
intermediates = [ "16bffac11115e3bd11a9ae95832edcabfa9d967ddf9524243b388ae87574c09b", "05a80af53889c59121a94054804df1320606b1c48baac7cce69473cda448b192", "9d2ef4878fa1e4d60b52a1a1bac7ae4bdfd9ef44dc931ddde7a09a7ff256764f" ]
proof1 = MerkleTreeProof(str(mtree.getRootHash()))
cert = "Bob signed cert for google.com"
# The result below will be wrong until you fix isValueInTree above.
if proof1.isValueInTree(cert, intermediates):
    print("\"" + cert + "\"" + " is in the tree with root " + str(proof1.getRootHash()))
else:
    print("\"" + cert + "\"" + " is not in the tree with root " + str(proof1.getRootHash()))
