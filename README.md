# Shuffle Tree

    Reference implementation
    Mayur Patel
    March 2005

Shuffle trees are a randomized self-adjusting
binary search tree.  Unlike splay trees, they conduct at most one rotation per access.
Shuffle trees will be best suited to large data sets, or large
numbers of queries or whenever a write to storage is slow.  In
other cases, a splay tree might outperform a shuffle tree.

If you haven't heard of shuffle trees, don't dispair.  I developed the data structure, but there has never been a paper on them.  

Like splay trees, they can be implemented around a traversal function.  Like scapegoat trees, they use a counter to detect imbalance in the tree.  When a point of imbalance is identified (the counter is zero) then a single rotation is performed to improve balance.


    // returns node with key k,
    // or returns the leaf containing
    // the closest key to k.
    node *
    Traverse(
      key k,
      node *root,
      int treesize
    ) {
      signed int iCounter = rand() % treesize;
      node *pRet = 0;
      node *p = root;
      while( p ) {
        pRet = p;
        if( k < value(p) ) {
          p = left(p);
          if(( ! iCounter ) && p ) {
            RotateRight( pRet );
            pRet = parent(p);
          } // end if
        } else if ( value(p) < k ) {
          p = right(p);
          if(( ! iCounter ) && p ) {
            RotateLeft( pRet );
            pRet = parent(p);
          } // end if
        } else
          break; // break while

        --iCounter;
        iCounter >>= 1;
      } // end while
      return( pRet );
    }


This repository implements a subset of the STL interface for associative
data structures, using shuffle tree algorithms.  
