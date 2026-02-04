#include <iostream> // for debugging/IO types
#include <algorithm> // for std::max
#include <climits>
#include <vector>
#include <string>

using namespace std;

// Node used by the AVL tree. Keeps pointers to left/right child, parent,
// an integer id (incremental insertion order), height for AVL balancing,
// and the post content string.
class Node {
    public :
    int id;
    Node* left;
    Node* right;
    Node* parent;
    int height;
    string content;
    Node(int k, const string& cont) : id(k), left(nullptr), right(nullptr), parent(nullptr), height(0), content(cont) {}
};

class AVLTree {
    private:
        Node* root;
        int size;

        int height(Node* N) {
            if (N == nullptr) return -1;
            return N->height;
        }

        string content(Node* N) {
            if (N == nullptr) return "";
            return N->content;
        }

    // Right rotation for AVL balancing. Returns the new subtree root.
    Node* rightRotate(Node* y) {
            Node* x = y->left;
            Node* T2 = x->right;

            // Perform rotation
            x->right = y;
            y->left = T2;
            // Update parent pointers correctly
            if (T2 != nullptr) T2->parent = y;
            Node* oldParent = y->parent;
            x->parent = oldParent;
            y->parent = x;
            // Re-attach x to oldParent
            if (oldParent != nullptr) {
                if (oldParent->left == y) oldParent->left = x;
                else if (oldParent->right == y) oldParent->right = x;
            }

            // Update heights
            y->height = max(height(y->left), height(y->right)) + 1;
            x->height = max(height(x->left), height(x->right)) + 1;

            // Return new root
            return x;
        }

    // Left rotation for AVL balancing. Returns the new subtree root.
    Node* leftRotate(Node* x) {
            Node* y = x->right;
            Node* T2 = y->left;

            x->right = T2;
            y->left = x;
            // Update parent pointers correctly
            if (T2 != nullptr) T2->parent = x;
            Node* oldParent = x->parent;
            y->parent = oldParent;
            x->parent = y;
            if (oldParent != nullptr) {
                if (oldParent->left == x) oldParent->left = y;
                else if (oldParent->right == x) oldParent->right = y;
            }

            x->height = max(height(x->left), height(x->right)) + 1;
            y->height = max(height(y->left), height(y->right)) + 1;

            return y;
        }

        // Balance factor = height(left) - height(right)
        int getBalance(Node* N) {
            if (N == nullptr)
                return 0;
            return height(N->left) - height(N->right);
        }

    // insertHelper: standard BST insert + AVL rebalancing.
    // 'id' is used as the BST key to maintain insertion order.
    Node* insertHelper(Node* node, int id, Node* parent = nullptr, const string& content = "") {
            // 1. Standard BST insertion
            if (node == nullptr) {
                Node* newNode = new Node(id, content);
                newNode->parent = parent;
                return newNode;
            }

            if (id < node->id) {
                node->left = insertHelper(node->left, id, node, content);
                node->left->parent = node;
            }
            else if (id > node->id) {
                node->right = insertHelper(node->right, id, node, content);
                node->right->parent = node;
            }
            else // Duplicate keys are not allowed
            return node;

            // 2. Update height of this ancestor node
            node->height = 1 + max(height(node->left), height(node->right));

            // 3. Get the balance factor to check for imbalance
            int balance = getBalance(node);

            // 4. If unbalanced, perform rotations
            // Left Left Case
            if (balance > 1 && id < node->left->id){
                return rightRotate(node);
            }

            // Right Right Case
            if (balance < -1 && id > node->right->id){
                return leftRotate(node);
            }

            // Left Right Case
            if (balance > 1 && id > node->left->id) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }

            // Right Left Case
            if (balance < -1 && id < node->right->id) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }

            return node;
        }

    // Recursively delete all nodes in the subtree (used by destructor)
    void deleteTree(Node* node) {
            if (node != nullptr) {
                deleteTree(node->left);
                deleteTree(node->right);
                delete node;
            }
        }

        // Helper for reverse-inorder traversal to collect up to N posts.
        // We traverse right-root-left so that higher ids (newer posts)
        // can be collected first if desired.
        vector<string> post_traversal_helper(Node* node, vector<string>& result, bool& end, int N) {
            if (node != nullptr && !end) {
                post_traversal_helper(node->right, result, end, N);
                if (result.size() < N) {
                    result.push_back(node->content);
                } else {
                    end = true;
                }
                post_traversal_helper(node->left, result, end, N);
            }
            return result;
        }



        public:

        // Construct an empty AVL tree
        AVLTree() {
            root = nullptr;
            size = 0;
        }

        ~AVLTree() {
            deleteTree(root);
        }

        void insert( const string& content = "") {
            size++;
            root = insertHelper(root, size, nullptr, content);
        }

        int getRootBalanceFactor() {
            if (root == nullptr)return 10;
            return getBalance(root);
        }

        vector<string> traversal_post(int N) {
            // Per spec: if N == -1, return all posts. Use INT_MAX as "no limit".
            if (N == -1) N = INT_MAX;
            vector<string> result;
            bool end = false;
            post_traversal_helper(root, result, end, N);
            return result;
        }

};