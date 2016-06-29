#ifndef __LKSTACK_H__
#define __LKSTACK_H__

template<class Node_entry>
struct Node {
//  data members
   Node_entry entry;
   Node<Node_entry> *next;
//  constructors
   Node();
   Node(Node_entry item, Node<Node_entry> *add_on = NULL);
};

template<class Stack_entry>
class Stack {
public:
//  Standard Stack methods
   Stack();
   bool empty() const;
   Error_code push(const Stack_entry &item);
   Error_code pop();
   Error_code top(Stack_entry &item) const;
   void clear();
//  Safety features for linked structures
   ~Stack();
   Stack(const Stack<Stack_entry> &original);
   void operator =(const Stack<Stack_entry> &original);
protected:
   Node<Stack_entry> *top_node;
};


template<class Node_entry>
Node<Node_entry>::Node()
{
   next = NULL;
}

template<class Node_entry>
Node<Node_entry>::Node(Node_entry item, Node<Node_entry> *add_on)
{
   entry = item;
   next = add_on;
}

template<class Stack_entry>
Stack<Stack_entry>::Stack()
{
   top_node = NULL;
}

template<class Stack_entry>
bool Stack<Stack_entry>::empty() const
{
  if(top_node == NULL)
     return true;
  else
     return false;
}

/*
Post: Stack_entry item is added to the top of
      the Stack; returns success or returns a code
      of overflow if dynamic memory is exhausted.
*/
template<class Stack_entry>
Error_code Stack<Stack_entry>::push(const Stack_entry &item)
{
   Node<Stack_entry> *new_top = new Node<Stack_entry>(item, top_node);
   if (new_top == NULL) return overflow;
   top_node = new_top;
   return success;
}

/*
Post: The top of the Stack is removed.  If the Stack
      is empty the method returns underflow; otherwise it returns success.
*/
template<class Stack_entry>
Error_code Stack<Stack_entry>::pop()
{
   Node<Stack_entry> *old_top = top_node;
   if (top_node == NULL) return underflow;
   top_node = old_top->next;
   delete old_top;
   return success;
}

template<class Stack_entry>
Error_code Stack<Stack_entry>::top(Stack_entry &item) const
{
   if(empty())
     return underflow;
   else{
     item = top_node->entry;
     return success;
   }
}

/*
Post: The Stack is cleared.
*/
template<class Stack_entry>
void Stack<Stack_entry>::clear() //  clear elememt
{
   while (!empty())
      pop();
}

/*
Post: The Stack is cleared.
*/
template<class Stack_entry>
Stack<Stack_entry>::~Stack() //  Destructor
{
  clear();
}

/*
Post: The Stack is initialized as a copy of Stack original.
*/
template<class Stack_entry>
Stack<Stack_entry>::Stack(const Stack<Stack_entry> &original) //  copy constructor
{
   Node<Stack_entry> *new_copy, *original_node = original.top_node;
   if (original_node == NULL) top_node = NULL;
   else {                         //  Duplicate the linked nodes.
      top_node = new_copy = new Node<Stack_entry>(original_node->entry);
      while (original_node->next != NULL) {
        original_node = original_node->next;
        new_copy->next = new Node<Stack_entry>(original_node->entry);
        new_copy = new_copy->next;
      }
   }
}

/*
Post: The Stack is reset as a copy of Stack original.
*/
template<class Stack_entry>
void Stack<Stack_entry>::operator = (const Stack<Stack_entry> &original) //  Overload assignment
{
   Node<Stack_entry> *new_top, *new_copy, *original_node = original.top_node;
   if (original_node == NULL) new_top = NULL;
   else {                         //  Duplicate the linked nodes
      new_copy = new_top = new Node<Stack_entry>(original_node->entry);
      while (original_node->next != NULL) {
        original_node = original_node->next;
        new_copy->next = new Node<Stack_entry>(original_node->entry);
        new_copy = new_copy->next;
      }
   }
   while (!empty())               //  Clean out old Stack entries
      pop();
   top_node = new_top;            //  and replace them with new entries.
}

#endif
