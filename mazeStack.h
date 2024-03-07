#ifndef CS300HMW1_MAZESTACK_H
#define CS300HMW1_MAZESTACK_H

#include <exception>

//Important note: All the class implementation of the stack including the member functions are taken directly
//from the lecture slides, I did not change any or implemented new member functions.
//I have only added the class for the UnderFlow exception.

template <class Object>
class mazeStack{

public:

    mazeStack( );
    mazeStack( const mazeStack & rhs );
    ~mazeStack( );

    bool isEmpty( ) const;
    bool isFull( ) const;
    void makeEmpty( );

    void pop( );
    void push( const Object & x );
    Object topAndPop( );
    const Object & top( ) const;

    const mazeStack & operator=( const mazeStack & rhs );

private:

    struct node{

        Object element;
        node *next;

        node( const Object & theElement, node * n = NULL )
                : element( theElement ), next( n ) { }


    };

    node * topOfStack;

};

class UnderflowException : public std::exception {
public:
    const char* what() const throw() {
        return "Underflow error occurred!";
    }
};

template<class Object>
mazeStack<Object>::mazeStack() {
    topOfStack = NULL;
}

template <class Object>
bool mazeStack<Object>::isFull( ) const
{
    return false;
}

template <class Object>
const Object & mazeStack<Object>::top( ) const
{
    if ( isEmpty( ) )
        throw UnderflowException();
    return topOfStack->element;
}

template <class Object>
void mazeStack<Object>::pop( )
{
    if ( isEmpty( ) )
        throw UnderflowException();
    node *oldTop = topOfStack;
    topOfStack = topOfStack->next;
    delete oldTop;
}

template <class Object>
void mazeStack<Object>::push( const Object & x )
{
    topOfStack = new node( x, topOfStack );
}

template <class Object>
Object mazeStack<Object>::topAndPop( )
{
    Object topItem = top( );
    pop( );
    return topItem;
}

template <class Object>
void mazeStack<Object>::makeEmpty( )
{
    while ( ! isEmpty( ) )
        pop( );
}

template <class Object>
const mazeStack<Object> & mazeStack<Object>::
operator=( const mazeStack<Object> & rhs )
{
    if ( this != &rhs )
    {
        makeEmpty( );
        if ( rhs.isEmpty( ) )
            return *this;

        node *rptr = rhs.topOfStack;
        node *ptr  = new node( rptr->element );
        topOfStack = ptr;

        for ( rptr = rptr->next; rptr != NULL; rptr = rptr->next )
            ptr = ptr->next = new node( rptr->element );
    }
    return *this;
}

template <class Object>
mazeStack<Object>::mazeStack( const mazeStack<Object> & rhs )
{
    topOfStack = NULL;
    *this = rhs; // deep copy
}

template <class Object>
mazeStack<Object>::~mazeStack( )
{
    makeEmpty( );
}

template <class Object>
bool mazeStack<Object>::isEmpty( ) const
{
    return topOfStack == NULL;
}

#endif