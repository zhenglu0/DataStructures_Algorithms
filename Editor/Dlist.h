#ifndef __DLIST_H__
#define __DLIST_H__

// This is the file to include in your code if you want access to the
// complete DList template class

// First, get the declaration for the base list class

#include "Dlink.h"
#include "List.h"

using namespace std;

// Linked list implementation
template <class Elem> class LList: public List<Elem> {
private:
  Link<Elem>* head;       // Pointer to list header
  Link<Elem>* tail;       // Pointer to last Elem in list 
  Link<Elem>* fence;      // Last element on left side
  int leftcnt;            // Size of left partition
  int rightcnt;           // Size of right partition
  void init() {           // Intialization routine
    fence = tail = head = new Link<Elem>;
    leftcnt = rightcnt = 0;
  }
  void removeall() {   // Return link nodes to free store
    while(head != NULL) {
      fence = head;
      head = head->next;
      delete fence;
    }
  }
public:
  LList(int size=DefaultListSize) { init(); }
  ~LList() { removeall(); }  // Destructor
  void clear() { removeall(); init(); }
  bool insert(const Elem&);
  bool append(const Elem&);
  bool remove(Elem&);
  void setStart()
    { fence = head; rightcnt += leftcnt; leftcnt = 0; }
  void setEnd()
    { fence = tail; leftcnt += rightcnt; rightcnt = 0; }
  void prev();
  void next() {
    if (fence != tail) // Don't move fence if right empty
      { fence = fence->next; rightcnt--; leftcnt++; }
  }
  int leftLength() const  { return leftcnt; }
  int rightLength() const { return rightcnt; }
  bool setPos(int pos);
  bool getValue(Elem& it) const {
    if(rightLength() == 0) return false;
    it = fence->next->element;
    return true;
  }
  void print() const;
};

// First are the functions not implemented in llist.h that
// are different from the singly linked list version.
template <class Elem> // Insert at front of right partition
bool LList<Elem>::insert(const Elem& item) {
  fence->next = new Link<Elem>(item, fence, fence->next);  
  if (fence->next->next != NULL) // If not deleting at end
    fence->next->next->prev = fence->next;
  if (tail == fence)             // Appending new Elem
    tail = fence->next;          //   so set tail
  rightcnt++;                    // Added to right
  return true;
}

template <class Elem> // Append Elem to end of the list.
bool LList<Elem>::append(const Elem& item) {
  tail = tail->next = new Link<Elem>(item, tail, NULL);
  rightcnt++;                    // Added to right
  return true;
}

// Remove and return first Elem in right partition
template <class Elem> bool LList<Elem>::remove(Elem& it) {
  if (fence->next == NULL) return false; // Empty right
  it = fence->next->element;       // Remember value
  Link<Elem>* ltemp = fence->next; // Remember link node
  if (ltemp->next != NULL) ltemp->next->prev = fence;
  else tail = fence;               // Reset tail
  fence->next = ltemp->next;       // Remove from list
  delete ltemp;                    // Reclaim space
  rightcnt--;                      // Removed from right
  return true;
}

// Move fence one step left; no change if left is empty
template <class Elem> void LList<Elem>::prev() {
  if (fence != head)  // Can't back up from list head
    { fence = fence->prev; leftcnt--; rightcnt++; }
}

// Here are the functions that are identical to their singly linked 
// list counterparts.
// Set the size of left partition to pos
template <class Elem> bool LList<Elem>::setPos(int pos) {
  if ((pos < 0) || (pos > rightcnt + leftcnt)) return false;
  fence = head;
  for(int i=0; i<pos; i++) fence = fence->next;
  rightcnt = rightcnt + leftcnt - pos;
  leftcnt = pos;
  return true;
}

template <class Elem> void LList<Elem>::print() const {
  Link<Elem>* temp = head;
  cout << "< ";
  while (temp != fence) {
    cout << temp->next->element << " ";
    temp = temp->next;
  }
  cout << "| ";
  while (temp->next != NULL) {
    cout << temp->next->element << " ";
    temp = temp->next;
  }
  cout << ">\n";
}

#endif