// ----------------------------------------------------------------------------
// Shea Line
// sline
// 2023 Winter CSE101 pa8
// ----------------------------------------------------------------------------
#include"Dictionary.h"

#define RED -20
#define BLACK -10

// Helper Functions (Optional) ---------------------------------------------

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const
{
   if (R != nil) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}


// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string will consist of keys only, with the order determined by a pre-order
// tree walk. The keys stored in black Nodes will be appended as "key\n", and
// the keys stored in red Nodes will be appended as "key (RED)\n". The output 
// of this function is demonstrated in /Examples/pa8/DictionaryClient-out.
void Dictionary::preOrderString(std::string& s, Node* R) const
{
   if (R != nil) 
   {
        if (R != nil) 
        {
            s += R->key;
            if (R->color == RED)
                s += " (RED)";
            s += "\n";
            preOrderString(s, R->left);
            preOrderString(s, R->right);
        }
    }
}


// BST_insert()
// Inserts a copy of the Node *M into this Dictionary.
void Dictionary::BST_insert(Node* M) 
{
    Node *y = this->nil;
    Node *x = this->root;
    Node *z = M;
    while (x != this->nil) 
    {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == this->nil)
        this->root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;
    z->left = this->nil;
    z->right = this->nil;
    z->color = RED;
    this->RB_InsertFixUp(z);
}


// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N)
{
   if (R!= N) 
   {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// Deletes all Nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R)
{
    if (R != nil) 
    {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        //num_pairs--;
        delete R;
    }
}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const
{
   while (R != nil && k != R->key) 
   {
        if (k < R->key) 
            R = search(R->left, k);
        else 
            R = search(R->right, k);
    }
    return R;
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R)
{
    if (R == nil) 
        return nil;
    Node *min = R;
    while (min->left != nil)
        min = min->left;
    return min;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R)
{
    if (R == nil) 
        return nil;
    Node *max = R;
    while (max->right != nil)
        max = max->right;
    return max;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N)
{
    Node *n = N;
    if (n->right != nil)
        return findMin(n->right);
    Node* y = n->parent;
    while (y != nil && n == y->right) 
    {
        n = y;
        y = y->parent;
    }
    return y;
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N)
{
    Node *n = N;
    if (n->left != nil)
        return findMax(n->left);
    Node* y = n->parent;
    while (y != nil && n == y->left) 
    {
        n = y;
        y = y->parent;
    }
   return y;
}
/*
// transplant()
void Dictionary::transplant(Node* u, Node* v)
{
    if (u->parent == nil)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != nil)
        v->parent = u->parent;
}
*/
// RBT Helper Functions ----------------------------------------------------

// LeftRotate()
// Rotates the tree around the given node x.
void Dictionary::LeftRotate(Node* x)
{
   Node* y = x->right;
   x->right = y->left;
   if (y->left != nil)
      y->left->parent = x;
   y->parent = x->parent;
   if (x->parent == nil)
      root = y;
   else if (x == x->parent->left)
      x->parent->left = y;
   else
      x->parent->right = y;
   y->left = x;
   x->parent = y;
}

// RightRotate()
// Rotates the tree around the given node x.
void Dictionary::RightRotate(Node* x)
{
   Node* y = x->left;
   x->left = y->right;
   if (y->right != nil)
      y->right->parent = x;
   y->parent = x->parent;
   if (x->parent == nil)
      root = y;
   else if (x == x->parent->right)
      x->parent->right = y;
   else
      x->parent->left = y;
   y->right = x;
   x->parent = y;
}

// insertFixup()
// Fixes the red-black tree properties after an insertion.
void Dictionary::RB_InsertFixUp(Node* z)
{
   while (z->parent->color == RED) 
   {
      if (z->parent == z->parent->parent->left) 
      {
         Node* y = z->parent->parent->right;
         if (y->color == RED) 
         {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
         } 
         else 
         {
            if (z == z->parent->right) 
            {
               z = z->parent;
               LeftRotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            RightRotate(z->parent->parent);
         }
      } 
      else 
      {
         Node* y = z->parent->parent->left;
         if (y->color == RED) 
         {
            z->parent->color = BLACK;
            y->color = BLACK;
            z->parent->parent->color = RED;
            z = z->parent->parent;
         } 
         else 
         {
            if (z == z->parent->left) 
            {
               z = z->parent;
               RightRotate(z);
            }
            z->parent->color = BLACK;
            z->parent->parent->color = RED;
            LeftRotate(z->parent->parent);
         }
      }
   }
   root->color = BLACK;
}

// RB_Transplant()
   void Dictionary::RB_Transplant(Node* u, Node* v) 
   {
      if (u->parent == this->nil)
         this->root = v;
      else if (u == u->parent->left)
         u->parent->left = v;
      else
         u->parent->right = v;
      v->parent = u->parent;
   }
// DeleteFixup()
// Fixes the red-black tree properties after a deletion.
void Dictionary::RB_DeleteFixUp(Node* x)
{
   while (x != root && x->color == BLACK) 
   {
      if (x == x->parent->left) 
      {
         Node* w = x->parent->right;
         if (w->color == RED) 
         {
            w->color = BLACK;
            x->parent->color = RED;
            LeftRotate(x->parent);
            w = x->parent->right;
         }
         if (w->left->color == BLACK && w->right->color == BLACK) 
         {
            w->color = RED;
            x = x->parent;
         } 
         else 
         {
            if (w->right->color == BLACK) 
            {
               w->left->color = BLACK;
               w->color = RED;
               RightRotate(w);
               w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->right->color = BLACK;
            LeftRotate(x->parent);
            x = root;
         }
      } 
      else 
      {
         Node* w = x->parent->left;
         if (w->color == RED) 
         {
            w->color = BLACK;
            x->parent->color = RED;
            RightRotate(x->parent);
            w = x->parent->left;
         }
         if (w->right->color == BLACK && w->right->color == BLACK) 
         {
            w->color = RED;
            x = x->parent;
         } 
         else 
         {
            if (w->left->color == BLACK) 
            {
               w->right->color = BLACK;
               w->color = RED;
               LeftRotate(w);
               w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = BLACK;
            w->left->color = BLACK;
            RightRotate(x->parent);
            x = root;
         }
      }
   }
   x->color = BLACK;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* N) 
{
      Node* z = N;
      Node* y = z;
      Node* x;
      int y_original_color = y->color;
      if (z->left == this->nil) {
         x = z->right;     // omfg another mistake
         this->RB_Transplant(z, z->right); //mistake
      }

      // forgot this entire block bruh
      else if (z->right == this->nil) {
         x = z->left;
         this->RB_Transplant(z, z->left);
      }

      else {
         y = this->findMin(z->right);
         y_original_color = y->color;
         x = y->right;
         if (y->parent == z) {
            x->parent = y;
         }
         else {
            this->RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
         }
         this->RB_Transplant(z, y);
         y->left = z->left;
         y->left->parent = y;
         y->color = z->color;
      }

      if (y_original_color == BLACK) {
         this->RB_DeleteFixUp(x);
      }
}


// Class Constructors & Destructors ----------------------------------------

// Node constructor
Dictionary::Node::Node(keyType k, valType v)
{
   key = k;
   val = v;
   parent = nullptr;
   left = nullptr;
   right = nullptr;
   color = BLACK;
}

// Creates new Dictionary in the empty state.
Dictionary::Dictionary()
{
   nil = new Node("", -5);
   root = nil;
   current = nil;
   num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D)
{
   nil = new Node("", 0);
   root = nil;
   current = nil;
   num_pairs = 0;
   preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary() 
{
    clear();
    delete nil;
}

// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const 
{
      return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const {
    return (search(root, k) != nil);
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const
{
   Node* node = this->search(root, k);
   if(node != nil) 
      return node->val;
   else
      throw std::logic_error("Dictionary: getValue(): key \"" + k +"\" does not exist");
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false 
// otherwise.
bool Dictionary::hasCurrent() const
{
   return (current != nil);
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent()
keyType Dictionary::currentKey() const 
{
    if(hasCurrent())
        return current->key;
    else
      throw std::logic_error("Dictionary: currentKey(): current undefined");
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const 
{
    if(hasCurrent())
        return current->val;
    else
         throw std::logic_error("Dictionary: currentVal(): current undefined");
}

// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear() 
{
    if (this->num_pairs > 0)
    {
        postOrderDelete(root);
        root = nil;
        current = nil;
        num_pairs = 0;
    }
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v) 
{
    Node *temp = search(this->root, k);
    if (temp != nil)
    {
        temp->val = v;
        return;
    }
    Node* b = new Node(k, v);
    BST_insert(b);
    num_pairs++;

}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) 
{
    if (contains(k))
    {
        Node* a = search(root,k);
        if (a == this->current)
            current = nil;
        RB_Delete(a);
        delete a;
        a = nullptr;
        num_pairs--;
    }  
    else
        throw std::logic_error("Dictionary: remove(): key \"" + k +"\" does not exist");
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing.
void Dictionary::begin() 
{
    if (this->num_pairs > 0)
        current = findMin(root);
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end() 
{
    if (this->num_pairs > 0)
        current = findMax(root);
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next() 
{
    if(hasCurrent())
        current = findNext(current);
    else 
      throw std::logic_error("Dictionary: next(): current undefined");
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev() 
{
    if(hasCurrent())
        current = findPrev(current);
    else
      throw std::logic_error("Dictionary: prev(): current undefined");
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const 
{
    std::string s;
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. The key order is 
// given by a pre-order tree walk. The keys stored in black Nodes will appear in
// the returned string as "key\n", and the keys stored in red Nodes will appear 
// as "key (RED)\n".  The output of this function is demonstrated in 
// /Examples/pa8/DictionaryClient-out.
std::string Dictionary::pre_string() const 
{
    std::string s;
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const 
{
    if(num_pairs != D.num_pairs)
        return false;
    Dictionary copyD(D);
    Dictionary copyThis(*this);
    copyThis.begin();
    copyD.begin();
    while(copyThis.hasCurrent() && copyD.hasCurrent()) 
    {
        if(copyThis.currentKey() != copyD.currentKey() || copyThis.currentVal() != copyD.currentVal())
            return false;
        copyThis.next();
        copyD.next();
    }
    return true;
}

// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<(std::ostream& stream, Dictionary& D) 
{
    return stream << D.to_string();
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==(const Dictionary& A, const Dictionary& B) 
{
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=(const Dictionary& D) 
{
    if( this != &D )
    {
      Dictionary temp = D;
      std::swap(this->num_pairs, temp.num_pairs);
      std::swap(this->nil, temp.nil);
      std::swap(this->root, temp.root);
      std::swap(this->current, temp.current);
   }
   return *this;
}