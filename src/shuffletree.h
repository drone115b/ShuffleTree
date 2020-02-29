// ------------------------------------------------
// Copyright 2004, 2005, Mayur Patel
// Source code for illustrative purposes only.
// Use at your own risk.
// ------------------------------------------------

// ------------------------------------------------
// _shuffletree: associative data structure
//
// base class for associative data structures
// using shuffle tree algorithms:
// a self-adjusting binary search tree.
// ------------------------------------------------
#ifndef shuffletree_HEADER
#define shuffletree_HEADER

#include <cassert>
#include <utility> // std::pair<>, std::less<>
#include <tt800.h> // RNG, 30% faster than anything else tested.

template< class Node, class Cmp >
class _shuffletree 
{
public:
	typedef typename Node::value_type	value_type;
	typedef typename Node::key_type		key_type;
	typedef Cmp							key_compare;

public:
	class const_iterator; // forward declaration

	class iterator
	{
	public:
		iterator(Node*p){ _p=p;}
		iterator(const iterator&i){ _p=i._p;}
		iterator(){}
		~iterator(){}

		iterator&
		operator = ( const iterator& i ){ _p = i._p; return *this; }
		bool
		operator==(const iterator& i ){ return _p == i._p; }
		bool
		operator!=(const iterator& i ){ return _p != i._p; }

		value_type&
		operator*(void){ return (*_p).value(); }

		iterator&
		operator++(void){ _p = (*_p).next; return(*this); }		
		iterator
		operator++(int){ iterator i(*this); _p=(*_p).next; return i; }

		iterator&
		operator--(void){ _p = (*_p).prev; return(*this); }
		iterator
		operator--(int){ iterator i(*this); _p=(*_p).prev; return i; }

	protected:
		Node *_p;
		friend class const_iterator;
		friend class _shuffletree< Node, Cmp >;
	};

	class const_iterator
	{
	public:
		const_iterator(Node*p){ _p=p;}
		const_iterator(const iterator&i){ _p=i._p;}
		const_iterator(const const_iterator& i ){ _p=i._p;}
		const_iterator(){}
		~const_iterator(){}

		const_iterator&
		operator = ( const const_iterator& i ){ _p = i._p; return *this; }
		bool
		operator==(const const_iterator& i ){ return _p == i._p; }
		bool
		operator!=(const const_iterator& i ){ return _p != i._p; }
		
		bool
		operator==(const iterator& i ){ return _p == i._p; }
		bool
		operator!=(const iterator& i ){ return _p != i._p; }

		const value_type&
		operator*(void)const{ return (*_p).value(); }

		const_iterator&
		operator++(void){ _p = (*_p).next; return(*this); }
		const_iterator
		operator++(int){ const_iterator i(*this); _p=(*_p).next; return i; }

		const_iterator&
		operator--(void){ _p = (*_p).prev; return(*this); }
		const_iterator
		operator--(int){ const_iterator i(*this); _p=(*_p).prev; return i; }

	protected:
		Node *_p;
	};

public:

	_shuffletree( const Cmp& cmp = Cmp() ) : key_cmp( cmp )
	{
		_uSize = 0;
		_uMask = 0;
		_n.right = 0;
		_n.prev = &_n;
		_n.next = &_n;
	}

	_shuffletree( const _shuffletree<Node,Cmp>& c )
	{
		_uSize = 0;
		_uMask = 0;
		_n.right = 0;
		_n.prev = &_n;
		_n.next = &_n;

		const_iterator p = c.begin();
		while( p != c.end() )
		{
			insert( *p );
			++p;
		}
	}

	virtual
	~_shuffletree(void){ clear(); }
	
	_shuffletree<Node,Cmp>&
	operator=( const _shuffletree<Node,Cmp> & st )
	{
		if( this != (&st) )
		{
			clear();
			const_iterator p = st.begin();
			while( p != st.end() )
			{
				insert( *p );
				++p;
			}
		}
		return (*this);
	}	

	void
	seed( unsigned long l ){ _rand = tt800( l ); }

public:

	iterator
	begin(void){ return iterator( _n.next ); }

	const_iterator
	begin(void) const { return const_iterator( _n.next ); }

	iterator
	end(void){ return iterator( &_n ); }

	const_iterator
	end(void) const { return const_iterator( &_n ); }

public:

	std::pair< iterator, bool >
	insert( const value_type& v );
	
	iterator
	find( const key_type& k );

	const_iterator
	find( const key_type& k ) const;

	iterator
	erase( iterator i )
	{
		Node *p = i._p;
		iterator j(i);

		if( i != end() )
		{
			++j;
			erase( (*p).key() );
		}

		return j;
	}

	unsigned
	erase( const key_type& k );

	unsigned
	size(void) const {return _uSize;}

	bool
	empty( void ) const { return (! _uSize); }

	void
	clear(void);

protected:

	void
	incr_size( void )
	{
		++_uSize;
		_uMask <<= 1;
		_uMask |= _uSize;
		_uMask >>= 1;
	}

	void
	decr_size( void )
	{
		--_uSize;
		_uMask |= _uSize;
		_uMask >>= 1;
	}

	void
	reset_counter( void ) const
	{
		_iCounter = (*_rand) & _uMask;
		++_rand;
	}


	// Returns the node with key k;
	// or if it doesn't exist, the leaf node
	// with the closest key to k.
    Node *
    traverse( register Node **p, const key_type& k ) const;

	void
    replace_child( Node *pParent, Node *pNode, Node *pRepl )
    {
        assert( pNode );
        if( pParent )
            if( pParent->left == pNode )
                pParent->left = pRepl;
            else {
                assert( pParent->right == pNode );
                pParent->right = pRepl;
            }
        return;
    }

protected:
	// Q.left/right are left/right subtrees of Q
	// _n.right is the root of the search tree (shuffle tree)
	// _n.next is the first element in order (root of linked-list)
	// _n.prev is the last element in order
	// &_n is the "end," or "null" address
	mutable Node			_n;

	Cmp						key_cmp;
	unsigned long			_uSize;

	// attributes used primarily for the random counter:
	mutable signed long		_iCounter; // cached as an optimization
	unsigned long			_uMask; // used as an optimization
	mutable tt800			_rand; // fast random sequence generator
};

// ------------------------------------------------

template< class Node, class Cmp >
std::pair< typename _shuffletree< Node, Cmp >::iterator, bool >
_shuffletree< Node, Cmp >::insert( typename const _shuffletree< Node, Cmp >::value_type& v )
{		
	Node            *p[3];
					p[0] = _n.right;
					p[1] = 0;
					p[2] = 0;
	Node            *pRet;
	bool			bLess, bGreater;

	std::pair< iterator, bool >
					ret( end(), true );

	if( p[0] )
	{
		reset_counter();
		pRet = traverse( p, Node::key(v) );
		assert( pRet );

		bGreater = key_cmp( (*pRet).key(), Node::key(v) );
		bLess = key_cmp( Node::key(v), (*pRet).key() );

		if(! (bLess || bGreater))
			ret.second = false;
		else 
		{	            
			Node *pNew = new Node( v );
			(*pNew).left = 0;
			(*pNew).right = 0;
			ret.first = iterator(pNew);
			incr_size();
	        
			if( bLess )
			{
				assert( (*pRet).left == 0 );
				(*pRet).left = pNew;

				//
				// connect to linked-list
				//
				(*pNew).prev = (*pRet).prev;
				(*pNew).next = pRet;
				(*pRet).prev = pNew;
				(*(*pNew).prev).next = pNew;

			} else {
				assert( (*pRet).right == 0 );
				(*pRet).right = pNew;

				//
				// connect to linked-list
				//
				(*pNew).next = (*pRet).next;
				(*pNew).prev = pRet;
				(*pRet).next = pNew;
				(*(*pNew).next).prev = pNew;
			}
		}
	    
	    
	} else {
		assert( _n.right == 0 );
		_n.right = new Node( v );
		_n.next = _n.right;
		_n.prev = _n.right;
		(*_n.right).next = &_n;
		(*_n.right).prev = &_n;
		(*_n.right).left = 0;
		(*_n.right).right = 0;
		ret.first = iterator(_n.right);

		assert( _uSize == 0 );
		assert( _uMask == 0 );
		incr_size();
	}

	return ret;
}



template< class Node, class Cmp >
typename _shuffletree< Node, Cmp >::iterator
_shuffletree< Node, Cmp >::find( typename const _shuffletree< Node, Cmp >::key_type& k )
{
	Node            *p[3];
					p[0] = _n.right;
					p[1] = 0;
					p[2] = 0;
	Node            *pRet;

	if( p[0] )
	{
		reset_counter();
		pRet = traverse( p, k );
		assert( pRet );
		if( key_cmp((*pRet).key(),k) || key_cmp(k,(*pRet).key()) )
			pRet = &_n;
	} else
		pRet = &_n;

	return iterator( pRet );
}



template< class Node, class Cmp >
typename _shuffletree< Node, Cmp >::const_iterator
_shuffletree< Node, Cmp >::find( typename const _shuffletree< Node, Cmp >::key_type& k ) const
{
	Node            *p[3];
					p[0] = _n.right;
					p[1] = 0;
					p[2] = 0;
	Node            *pRet;

	if( p[0] )
	{
		reset_counter();
		pRet = traverse( p, k );
		assert( pRet );
		if( key_cmp((*pRet).key(),k) || key_cmp(k,(*pRet).key()) )
			pRet = &_n;
	} else
		pRet = &_n;

	return const_iterator( pRet );
}



template< class Node, class Cmp >
unsigned
_shuffletree< Node, Cmp >::erase( typename const _shuffletree< Node, Cmp >::key_type& k )
{
	Node            *p[6];
					p[0] = _n.right;
					p[1] = 0;
					p[2] = 0;
	Node            *pN;
	unsigned		u = 0;

	reset_counter();
	pN = traverse( p, k );

	if( pN )
	if( ! ( key_cmp( (*pN).key(), k) || key_cmp(k,(*pN).key())))
	{
		// remove from linked-list:
		(*(*pN).next).prev = (*pN).prev;
		(*(*pN).prev).next = (*pN).next;

		if( (! pN->left) && (! pN->right))
		{
			replace_child( p[2], pN, 0 );
			if( _n.right == pN )
				_n.right = 0;
		} else if( ! pN->left )
		{    
			replace_child( p[2], pN, pN->right );
			if( _n.right == pN )
				_n.right = pN->right;
		} else if ( ! pN->right )
		{
			replace_child( p[2], pN, pN->left );
			if( _n.right == pN )
				_n.right = pN->left;
		} else {
			p[3] = pN->right;
			p[4] = pN; assert( p[4] == p[1] );
			p[5] = p[2];
			Node *pSub = traverse( p+3, k );

			// splice pSub from tree:
			assert( pSub != 0 );            
			assert( pSub->left == 0 );

			replace_child( p[5], pSub, pSub->right );

			// swap pN for pSub in tree:
			pSub->left = pN->left;
			pSub->right = pN->right;
			replace_child( p[2], pN, pSub );
			if( _n.right == pN )
				_n.right = pSub;
			assert( pSub->left );
		}

		pN->left = 0;
		pN->right = 0;
		delete pN;
	    
		++u;
		decr_size();
	}
	
	return u;
}

template< class Node, class Cmp >
void
_shuffletree< Node, Cmp >::clear(void)
{
	Node *p = _n.next;
	Node *q;
	while( p != &_n ){
		q = p;
		p = (*p).next;
		delete q; 
	}
	_n.next = &_n;
	_n.prev = &_n;
	_n.right = 0;
	_uSize = 0;
	_uMask = 0;
	return;
}


template< class Node, class Cmp >
Node *
_shuffletree< Node, Cmp >::traverse( register Node **p, typename const _shuffletree< Node, Cmp >::key_type& k ) const
{
    while( *p )
    {
        p[2] = p[1]; // grandparent of node at iterator
        p[1] = p[0]; // parent of node at iterator
        if( key_cmp( k, ( **p ).key() ) )
        {
            *p = ( **p ).left;
            if(( ! _iCounter )&& (*p) )
            {
                // Rotate Right p[1]
                if( _n.right == p[1] )
                    _n.right = p[0];
                if( p[2] )
                    if( p[2]->left == p[1] )
                        p[2]->left = p[0];
                    else {
                        assert( p[2]->right == p[1] );
                        p[2]->right = p[0];
                    }
                p[1]->left = p[0]->right;
                p[0]->right = p[1];
                p[1] = p[2];
            }
        } else if ( key_cmp( ( **p ).key(), k) )
        {
            *p = ( **p ).right;
            if(( ! _iCounter )&& (*p) )
            {
                // Rotate Left p[1]
                if( _n.right == p[1] )
                    _n.right = p[0];
                if( p[2] )
                    if( p[2]->left == p[1] )
                        p[2]->left = p[0];
                    else {
                        assert( p[2]->right == p[1] );
                        p[2]->right = p[0];
                    }

                p[1]->right = p[0]->left;
                p[0]->left = p[1];
                p[1] = p[2];
            }
        } else
            break;

        --_iCounter;
        _iCounter >>= 1;    

    } // end while
    
    return( p[1] );
}

#endif
