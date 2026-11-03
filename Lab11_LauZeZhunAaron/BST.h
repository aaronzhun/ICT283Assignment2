#ifndef BST_H_INCLUDED
#define BST_H_INCLUDED

/**
 * @file BST.h
 * @brief Generic recursive BST reused from Lab 9 for Lab 10d and Lab 11
 */

/**
 * @brief Plain storage for one value and its two child links
 * @note A struct is sufficient because the node only groups data
 * @note The private root and tree methods control access to the actual nodes
 * @tparam T Stored value type
 */
template <class T>
struct BstNode
{
    T data; ///< Value used for comparison
    BstNode<T> * left;
    BstNode<T> * right;
};

/**
 * @class Bst
 * @brief Owns a binary search tree with no duplicate keys and no built-in I/O
 * @tparam T Value type supporting consistent less-than, greater-than and equality
 * @note Left subtree values are smaller and right subtree values are larger
 * @note This is an ordinary unbalanced tree
 */
template <class T>
class Bst
{
public:
    /**
     * @brief Starts with an empty tree
     */
    Bst();
    /**
     * @brief Copies the values and shape into newly allocated nodes
     * @param other Tree to copy
     */
    Bst(const Bst<T> & other);
    /**
     * @brief Deletes every owned node
     */
    ~Bst();
    /**
     * @brief Replaces this tree with an independent copy
     * @param other Source tree
     * @return This tree
     */
    Bst<T> & operator = (const Bst<T> & other);
    /**
     * @brief Adds a value while preserving strict BST order
     * @param value Value to insert
     * @return True if inserted, false if already present
     * @note Ordinary new may throw if memory cannot be allocated
     */
    bool Insert(const T & value);
    /**
     * @brief Searches without modifying the tree
     * @param value Value to find
     * @return True if present
     */
    bool Search(const T & value) const;
    /**
     * @brief Removes a value if present
     * @param value Value to remove
     * @return True if removed, otherwise false
     */
    bool Delete(const T & value);
    /**
     * @brief Deletes all elements and leaves an empty reusable tree
     */
    void Clear();
    /**
     * @brief Reports whether the tree contains no nodes
     * @return True for an empty tree
     */
    bool Empty() const;
    /**
     * @brief Visits left subtree, current value, then right subtree
     * @param visit Client function taking a constant reference to a value
     * @pre visit is not a null function pointer
     */
    void InOrder(void (*visit)(const T &)) const;
    /**
     * @brief Visits current value, left subtree, then right subtree
     * @param visit Client function taking a constant reference to a value
     * @pre visit is not a null function pointer
     */
    void PreOrder(void (*visit)(const T &)) const;
    /**
     * @brief Visits left subtree, right subtree, then current value
     * @param visit Client function taking a constant reference to a value
     * @pre visit is not a null function pointer
     */
    void PostOrder(void (*visit)(const T &)) const;
    /**
     * @brief Checks ancestor ordering bounds during lab development
     * @return True if every value lies strictly within its inherited bounds
     * @note Debug support for Lab 9, not an Assignment 2 production feature
     */
    bool CheckInvariant() const;

private:
    BstNode<T> * m_root; ///< Only this class can access the owned nodes

    /**
     * @brief Inserts recursively into a subtree
     */
    bool Insert(BstNode<T> * & node, const T & value);
    /**
     * @brief Searches recursively in a subtree
     */
    bool Search(const BstNode<T> * node, const T & value) const;
    /**
     * @brief Removes a value recursively from a subtree
     */
    bool Delete(BstNode<T> * & node, const T & value);
    /**
     * @brief Removes the smallest node and returns its value
     */
    void RemoveSmallest(BstNode<T> * & node, T & value);
    /**
     * @brief Releases children before their parent
     */
    void DeleteTree(BstNode<T> * & node);
    /**
     * @brief Copies the source shape into new nodes
     */
    void CopyTree(BstNode<T> * & destination, const BstNode<T> * source);
    /**
     * @brief Performs recursive inorder traversal
     */
    void InOrder(const BstNode<T> * node, void (*visit)(const T &)) const;
    /**
     * @brief Performs recursive preorder traversal
     */
    void PreOrder(const BstNode<T> * node, void (*visit)(const T &)) const;
    /**
     * @brief Performs recursive postorder traversal
     */
    void PostOrder(const BstNode<T> * node, void (*visit)(const T &)) const;
    /**
     * @brief Checks the whole subtree against ancestor bounds
     */
    bool CheckInvariant(const BstNode<T> * node, const BstNode<T> * lower,
                        const BstNode<T> * upper) const;
};

// Implementation remains below the class interface

// Memory management: the constructor starts with a null root and owns no nodes yet
template <class T>
Bst<T>::Bst()
{
    m_root = nullptr; // No root node yet because the tree is empty
}

// Rule of Three: the copy constructor creates a new tree with independently allocated nodes
// CopyTree copies values and shape instead of sharing the source tree's pointers
template <class T>
Bst<T>::Bst(const Bst<T> & other)
{
    m_root = nullptr; // Start with no root before CopyTree creates the copied nodes
    CopyTree(m_root, other.m_root); // Allocates separate nodes, not shared pointers
}

// Rule of Three: the destructor releases every owned node through the private DeleteTree helper
template <class T>
Bst<T>::~Bst()
{
    DeleteTree(m_root); // Private helper releases all nodes owned by this tree
}

// Rule of Three: assignment replaces an existing tree with an independent deep copy
// The self-assignment check prevents deleting the source when both objects are the same
template <class T>
Bst<T> & Bst<T>::operator = (const Bst<T> & other)
{
    if(this != &other) // Self-assignment must not delete its own source
    {
        DeleteTree(m_root);
        CopyTree(m_root, other.m_root);
    }
    return *this; // Return this tree by reference after assignment
}

// Public wrapper: start insertion at the private root without exposing node pointers to the client
template <class T>
bool Bst<T>::Insert(const T & value)
{
    return Insert(m_root, value); // Public wrapper hides node pointers from the client
}

// Recursive insertion: follow the left or right subtree until an empty link is found
// Allocate a node only at that empty link and reject equal values as duplicates
// node is a reference to a pointer, so it can change the actual root or child link
template <class T>
bool Bst<T>::Insert(BstNode<T> * & node, const T & value)
{
    if(node == nullptr) // Base case: this empty link is the insertion position
    {
        node = new BstNode<T>;
        node->data = value;
        node->left = nullptr;
        node->right = nullptr;
        return true;
    }
    if(value < node->data)
    {
        return Insert(node->left, value); // Repeats the same decision in the left subtree
    }
    else if(value > node->data)
    {
        return Insert(node->right, value); // Larger values continue in the right subtree
    }
    return false; // Duplicate: neither smaller nor larger, so no node is allocated
}

// Public wrapper: start the private recursive search at the root
template <class T>
bool Bst<T>::Search(const T & value) const
{
    return Search(m_root, value); // Public wrapper starts the private recursive search at the root
}

// Recursive search: return true for a match or search only the appropriate child subtree
// A null link is the stopping case and means the value is not present
template <class T>
bool Bst<T>::Search(const BstNode<T> * node, const T & value) const
{
    if(node == nullptr)
    {
        return false; // Empty link is the terminating case for an unsuccessful search
    }
    if(value == node->data)
    {
        return true;
    }
    if(value < node->data)
    {
        return Search(node->left, value);
    }
    return Search(node->right, value);
}

// Public single-value deletion: this removes one key, rather than clearing the whole tree
template <class T>
bool Bst<T>::Delete(const T & value)
{
    return Delete(m_root, value);
}

// Recursive single-value deletion: find the key, then handle zero, one or two children
// Reconnect a remaining child or replace the key with its inorder successor, then release the removed node
template <class T>
bool Bst<T>::Delete(BstNode<T> * & node, const T & value)
{
    if(node == nullptr)
    {
        return false;
    }
    if(value < node->data)
    {
        return Delete(node->left, value);
    }
    if(value > node->data)
    {
        return Delete(node->right, value);
    }

    BstNode<T> * oldNode = node;
    if(node->left == nullptr)
    {
        node = node->right; // Also handles a leaf because its right link is null
        delete oldNode;
    }
    else if(node->right == nullptr)
    {
        node = node->left; // The remaining child takes this node's position
        delete oldNode;
    }
    else
    {
        RemoveSmallest(node->right, node->data); // Successor replaces a node with two children
    }
    return true;
}

// Recursive deletion helper: follow left links to find and remove the smallest key
// Its possible right child must remain connected after the smallest node is deleted
template <class T>
void Bst<T>::RemoveSmallest(BstNode<T> * & node, T & value)
{
    if(node->left != nullptr)
    {
        RemoveSmallest(node->left, value);
    }
    else
    {
        value = node->data;
        BstNode<T> * oldNode = node;
        node = node->right; // Preserves the successor's possible right child
        delete oldNode;
    }
}

// Public Clear lets the client empty the tree without accessing internal node pointers
// Clear calls the private DeleteTree helper and leaves the tree reusable
template <class T>
void Bst<T>::Clear()
{
    DeleteTree(m_root);
}

// Reports whether the root is null, which means there are no stored nodes
template <class T>
bool Bst<T>::Empty() const
{
    return m_root == nullptr;
}

// PRIVATE recursive memory helper used by the destructor, Clear and assignment
// Delete the left subtree, then the right subtree, then the current node
// Children are released before their parent so their links can still be accessed
// The client calls public Clear, not this internal DeleteTree method
template <class T>
void Bst<T>::DeleteTree(BstNode<T> * & node)
{
    if(node != nullptr)
    {
        DeleteTree(node->left);
        DeleteTree(node->right);
        delete node; // Postorder deletion releases children before their parent
        node = nullptr; // Updates the actual root or child link because node is a pointer reference
    }
}

// Recursive deep-copy helper: allocate a new node, copy its value, then copy both child subtrees
// Copies keep the same values and shape but own separate nodes
// Changing or destroying one tree therefore does not damage the other
template <class T>
void Bst<T>::CopyTree(BstNode<T> * & destination, const BstNode<T> * source)
{
    if(source == nullptr)
    {
        destination = nullptr;
    }
    else
    {
        destination = new BstNode<T>;
        destination->data = source->data;
        destination->left = nullptr;
        destination->right = nullptr;
        CopyTree(destination->left, source->left); // Each subtree gets fresh nodes with the same shape
        CopyTree(destination->right, source->right);
    }
}

// Public traversal wrapper: begin at the root and pass the client's processing function
// visit must point to a function returning void and accepting one const T & parameter
// With Bst<DataRecType> the caller passes AddRecord, another client can pass a different matching function
// A function pointer lets the caller choose the processing without adding I/O or weather logic to the BST
template <class T>
void Bst<T>::InOrder(void (*visit)(const T &)) const // InOrder cannot directly change the tree object's member variables
{
    InOrder(m_root, visit);
}

// Recursive inorder traversal: LEFT subtree, CURRENT value, RIGHT subtree
// A valid BST produces ascending values in this order
// The visit callback handles processing outside the tree class
// Left current right gives ascending keys, visit(node->data) calls the supplied function once per node
// The const reference prevents the callback from changing a stored ordering key
template <class T>
void Bst<T>::InOrder(const BstNode<T> * node, void (*visit)(const T &)) const
{
    if(node != nullptr)
    {
        InOrder(node->left, visit);
        visit(node->data); // The client's function decides how to process the value
        InOrder(node->right, visit);
    }
}

// Public traversal wrapper: begin preorder traversal at the root
// Pass the same matching callback through every recursive call, the client never needs node pointers
template <class T>
void Bst<T>::PreOrder(void (*visit)(const T &)) const
{
    PreOrder(m_root, visit);
}

// Recursive preorder traversal: CURRENT value, LEFT subtree, RIGHT subtree
template <class T>
void Bst<T>::PreOrder(const BstNode<T> * node, void (*visit)(const T &)) const
{
    if(node != nullptr)
    {
        visit(node->data);
        PreOrder(node->left, visit);
        PreOrder(node->right, visit);
    }
}

// Public traversal wrapper: begin postorder traversal at the root
// Only the visit order changes, the callback still returns void and takes const T &
template <class T>
void Bst<T>::PostOrder(void (*visit)(const T &)) const
{
    PostOrder(m_root, visit);
}

// Recursive postorder traversal: LEFT subtree, RIGHT subtree, CURRENT value
// DeleteTree uses this same children-before-parent order to release memory
template <class T>
void Bst<T>::PostOrder(const BstNode<T> * node, void (*visit)(const T &)) const
{
    if(node != nullptr)
    {
        PostOrder(node->left, visit);
        PostOrder(node->right, visit);
        visit(node->data);
    }
}

// Public debug check: start at the root without any lower or upper ancestor bounds
// Return true when BST ordering holds and false when a violation is found
template <class T>
bool Bst<T>::CheckInvariant() const
{
    return CheckInvariant(m_root, nullptr, nullptr); // Root has no lower or upper ancestor bound
}

// Recursive invariant checker: every left-subtree value must be smaller and every right-subtree value larger
// Carry lower and upper bounds from all relevant ancestors, not only the immediate parent
// Example: 60 anywhere in the left subtree of root 50 fails, even if its own parent is 30
// An empty subtree satisfies the invariant;
template <class T>
bool Bst<T>::CheckInvariant(const BstNode<T> * node, const BstNode<T> * lower,
                            const BstNode<T> * upper) const
{
    if(node == nullptr)
    {
        return true;
    }
    if(lower != nullptr && !(node->data > lower->data))
    {
        return false; // Must be greater than every applicable lower ancestor
    }
    if(upper != nullptr && !(node->data < upper->data))
    {
        return false; // Must be smaller than every applicable upper ancestor
    }
    if(!CheckInvariant(node->left, lower, node)) // Left subtree inherits this node as its upper bound
    {
        return false;
    }
    return CheckInvariant(node->right, node, upper); // Right subtree inherits this node as its lower bound
}

#endif
