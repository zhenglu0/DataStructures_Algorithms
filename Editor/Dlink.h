#ifndef __DLINK_H__
#define __DLINK_H__

// Doubly-linked list link node with freelist support
template <class Elem> class Link {
private:
  static Link<Elem>* freelist; // Head of the freelist
public:
  Elem element;       // Value for this node
  Link *next;         // Pointer to next node in list
  Link *prev;         // Pointer to previous node
  Link(const Elem& e, Link* prevp =NULL, Link* nextp =NULL)
    { element = e;  prev = prevp;  next = nextp; }
  Link(Link* prevp =NULL, Link* nextp =NULL)
    { prev = prevp;  next = nextp; }
  // Overload new and delete operators for freelist
  void* operator new(size_t);
  void operator delete(void*);
};

template <class Elem>
Link<Elem>* Link<Elem>::freelist = NULL;

template <class Elem>   // Overload for new operator
void* Link<Elem>::operator new(size_t) {
  if (freelist == NULL) return ::new Link; // Create space
  Link<Elem>* temp = freelist;  // Can take  from freelist
  freelist = freelist->next;
  return temp;                 // Return the link
}

template <class Elem>   // Overload for delete operator
void Link<Elem>::operator delete(void* ptr) {
  ((Link<Elem>*)ptr)->next = freelist; // Put on freelist
  freelist = (Link<Elem>*)ptr;
}

#endif