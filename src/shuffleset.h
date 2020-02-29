// ------------------------------------------------
// Copyright 2004, 2005, Mayur Patel
// Source code for illustrative purposes only.
// Use at your own risk.
// ------------------------------------------------

// ------------------------------------------------
// shuffleset: associative data structure
//
// implements a subset of the STL interface
// for the "set" associative data structure
// using shuffle tree algorithms:
// a self-adjusting binary search tree.
//
// types: value_type, key_type, key_compare, 
// iterator, const_iterator
//
// begin(), end()
//
// insert( value_type )
// erase( iterator )
// erase( key_type )
// find( key_type )
//
// size(), empty(), clear()
// ------------------------------------------------

#ifndef shuffleset_HEADER
#define shuffleset_HEADER

#include <utility> // std::pair<>, std::less<>
#include <shuffletree.h>

template< class T >
class shuffleset_node
{
public:
	typedef T value_type;
	typedef T key_type;
public:
	shuffleset_node( const value_type& v ) : val( v ) {}
	shuffleset_node( void ){}
	~shuffleset_node( void ){}

	shuffleset_node&
	operator = ( const shuffleset_node<T> & n )
	{ val = n.val; return *this; }

	static const key_type&
	key( const value_type& v )
	{ return v; }

    const key_type&
    key( void ) const
    { return val; }

    value_type&
    value( void )
    { return val; }

    const value_type&
    value( void ) const
    { return val; }

	shuffleset_node *		left;
	shuffleset_node *		right;
	shuffleset_node *		next;
	shuffleset_node *		prev;
protected:
	value_type   val;
};




template< class T, class Cmp = std::less<T> >
class shuffleset : public _shuffletree< shuffleset_node< T >, Cmp >
{
private:
	typedef _shuffletree< shuffleset_node< T >, Cmp >
		parent_class;
public:
	shuffleset( const Cmp& cmp = Cmp() ) : parent_class(cmp) {}
	shuffleset( const shuffleset<T,Cmp>& m ) : parent_class(m) {}
	~shuffleset( void ){}

	shuffleset<T,Cmp>&
	operator =( const shuffleset<T,Cmp>& st )
	{ parent_class::operator=(st); return *this; }
};

#endif
