// ------------------------------------------------
// Copyright 2004, 2005, Mayur Patel
// Source code for illustrative purposes only.
// Use at your own risk.
// ------------------------------------------------

// ------------------------------------------------
// shufflemap: associative data structure
//
// implements a subset of the STL interface
// for the "map" associative data structure
// using shuffle tree algorithms:
// a self-adjusting binary search tree.
//
// types: value_type, key_type, mapped_type,
// key_compare, iterator, const_iterator
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

#ifndef shufflemap_HEADER
#define shufflemap_HEADER

#include <utility> // std::pair<>, std::less<>
#include <shuffletree.h>

template< class K, class V >
class shufflemap_node
{
public:
	typedef typename std::pair< const K, V >	value_type;
	typedef const K key_type;
public:
	shufflemap_node( const value_type& v ) : val( v ) {}
	shufflemap_node( void ){}
	~shufflemap_node( void ){}

	shufflemap_node&
	operator = ( const shufflemap_node<K,V> & n )
	{ val = n.val; return *this; }

	static const K&
	key( const value_type& v )
	{ return v.first; }

    const K&
    key( void ) const
    { return val.first; }

    value_type&
    value( void )
    { return val; }

	const value_type&
    value( void ) const
    { return val; }

	shufflemap_node *		left;
	shufflemap_node *		right;
	shufflemap_node *		next;
	shufflemap_node *		prev;
protected:
	value_type     val;
};




template< class K, class V, class Cmp = std::less<K> >
class shufflemap : public _shuffletree< shufflemap_node< K, V >, Cmp >
{
private:
	typedef _shuffletree< shufflemap_node< K, V >, Cmp >
		parent_class;
public:
	typedef V mapped_type;
public:
	shufflemap( const Cmp& cmp = Cmp() ) : parent_class(cmp) {}
	shufflemap( const shufflemap<K,V,Cmp>& m ) : parent_class(m) {}
	~shufflemap( void ){}

	shufflemap<K,V,Cmp>&
	operator =( const shufflemap<K,V,Cmp>& st )
	{ parent_class::operator=(st); return *this; }
};

#endif
