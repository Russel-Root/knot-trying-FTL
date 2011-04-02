#ifndef __LinkedList_h__
#define __LinkedList_h__

#ifndef NULL
#define NULL 0
#endif // NULL
// MyLinkedList, MyLinkedListIterator, ListNode»¥Îªfriend class
template<typename T> class MyLinkedList;
template<typename T> class MyLinkedListIterator;

template<typename T> class ListNode{
public:
	ListNode(){
		node = 0; 
		next = 0;
		previous = 0;
	}
public:
	T*			node;
	ListNode*	next;
	ListNode*	previous;
};

template<typename T> class MyLinkedList{
public:
	MyLinkedList();
	~MyLinkedList();

public:
	int		size() const;
	
	bool	add(T* node);
	bool	remove(T* node);
	bool	remove(int index);

	T*		get(int index) const;
	T*		getFirst() const;
	T*		getLast() const;
	T*		getPrevious(T* currentNode) const;
	T*		getNext(T* currentNode) const;

	ListNode<T>* head;
	ListNode<T>* tail;
	int		n;
};

template<typename T> class MyLinkedListIterator{
public:
	MyLinkedListIterator(const MyLinkedList<T>* lList);
	~MyLinkedListIterator();

	void rewind();
	bool hasNext();
	bool hasPrevius();

	T* next();
	T* previous();

private:
	ListNode<T>*			current;
	const MyLinkedList<T>*	list;
};

template<typename T> MyLinkedList<T>::MyLinkedList(){
	head = new ListNode<T>();
	tail = new ListNode<T>();

	head->next = head->previous = tail;
	tail->next = tail->previous = head;
	n = 0;
}

template<typename T> MyLinkedList<T>::~MyLinkedList(){
	ListNode<T>* current = this->head->next;
	ListNode<T>* temp;
	while ( current != tail ){
		temp = current;
		current = current->next;
		delete temp;
	}
	delete head;
	delete tail;
}

template<typename T> int MyLinkedList<T>::size() const{
	return this->n;
}

template<typename T> bool MyLinkedList<T>::add(T* node){
	if (node == 0)
		return false;

	ListNode<T>* newNode = new ListNode<T>();
	newNode->node = node;
	newNode->next = tail;
	newNode->previous = tail->previous;
	tail->previous->next = newNode;
	tail->previous = newNode;
	n++;
	return true;
}

template<typename T> bool MyLinkedList<T>::remove(T* node){
	if ( !node )
		return false;

	ListNode<T>* current = this->head->next;
	while( current != this->tail ){
		if ( current->node == node ){
			current->previous->next = current->next;
			current->next->previous = current->previous;
			delete current;
			n++;
			return true;
		}
		else
			current = current->next;
	}
	return false;
}

template<typename T> T* MyLinkedList<T>::get(int index) const{
	if ( index < 0 || index >= n )
		return 0;
	
	ListNode<T>* current = this->head->next;
	int i = 0;
	while( current != this->tail && i < index ){
		i++;
		current = current->next;
	}
	return current->node;
}

template<typename T> bool MyLinkedList<T>::remove(int index){
	if ( index < 0 || index >= n )
		return false;

	ListNode<T>* current = this->head->next;
	int i = 0;

	while( i < index ){
		i++;
		current = current->next;
	}
	current->previous->next = current->next;
	current->next->previous = current->previous;
	delete current;
	return true;
}

template<typename T> T* MyLinkedList<T>::getFirst() const{
	if ( this->head->next != this->tail )
		return this->head->next->node;
	else
		return 0;
}

template<typename T> T* MyLinkedList<T>::getLast() const{
	if (this->head != this->tail->previous)
		return this->tail->previous->node;
	else
		return 0;
}

template<typename T> T* MyLinkedList<T>::getNext(T* currentNode) const {
	if ( currentNode->next != this->tail )
		return currentNode->next->node;
	else
		return 0;
}

template<typename T> T* MyLinkedList<T>::getPrevious(T* currentNode) const {
	if ( currentNode->previous != head )
		return currentNode->previous->node;
	else
		return 0;
}

template<typename T> MyLinkedListIterator<T>::MyLinkedListIterator(const MyLinkedList<T>* _list):list(_list){
	this->current = NULL;
}

template<typename T> MyLinkedListIterator<T>::~MyLinkedListIterator(){}

template<typename T> void MyLinkedListIterator<T>::rewind(){
	if ( this->list->n > 0 )
		this->current = this->list->head->next;
	else
		this->current = NULL;
}

template<typename T> bool MyLinkedListIterator<T>::hasNext(){
	return (this->current != NULL) && (this->current != this->list->tail);
}

template<typename T> T* MyLinkedListIterator<T>::next(){
	if ( !this->hasNext() )
		return NULL;
	this->current = this->current->next;
	return this->current->previous->node;
}

template<typename T>  bool MyLinkedListIterator<T>::hasPrevius(){
	return (this->current != NULL) && (this->current != this->list->head);
}

template<typename T> T* MyLinkedListIterator<T>::previous(){
	if ( !this->hasPrevius() )
		return NULL;
	this->current = this->current->previous;
	return this->current->next->node;
}

#endif // __LinkedList_h__