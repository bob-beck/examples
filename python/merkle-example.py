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

elems = [ "Bob signed cert for google.com", "Alice signed cert for microsoft.com", "Paul signed cert for ualberta.ca", "Mallory signed cert for google.com" ]
mtree = MerkleTree(elems)
print("Root of tree : " +str(mtree.getRootHash()))
print("------")
mtree.printTree()
print("If we change anything in the tree, the root changes")
elems = [ "Bob signed cert for gmail.com", "Alice signed cert for microsoft.com", "Paul signed cert for ualberta.ca", "Mallory signed cert for google.com" ]
mtree = MerkleTree(elems)
print("New Root of tree : " +str(mtree.getRootHash()))
print("------")
mtree.printTree()


