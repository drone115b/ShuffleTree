#ifndef TT800_HEADER
#define TT800_HEADER

/* TT800 Pseudo-random number generator: July 8th 1996 Version */
/* by M. Matsumoto, email: matumoto@math.keio.ac.jp */
/* One may choose any initial 25 seeds except all zeros. */
/* See: ACM Transactions on Modelling and Computer Simulation, */
/* Vol. 4, No. 3, 1994, pages 254-266. */

// ------------------------------------------------
// C++ refactoring by Mayur Patel
// Implements a ForwardIterator interface
//
// For illustrative purposes only.
// Use at your own risk.
// ------------------------------------------------
class tt800 {
protected:
	void refresh(void);
public:
	typedef unsigned long data_type;
	
	static const data_type MAX;

	tt800( data_type seed = 0 );

	data_type operator*(void) const
	{
		register data_type y;
		y = x[i];
		y ^= (y << 7) & 0x2b5b2500; /* s and b, magic vectors */
		y ^= (y << 15) & 0xdb8b0000; /* t and c, magic vectors */
		y &= MAX; /* portablility */
		y ^= (y >> 16); /* added to the 1994 version */
		return y;
	}

	tt800& operator++(void)
	{
		++i;
		if( i >= 25 )
			refresh();
		return *this;
	}

	tt800 operator++(int);

	bool operator==(const tt800& t2) const;
	bool operator!=(const tt800& t2) const
	{ return !( (*this)==t2 ); }

protected:
	short i;
	data_type x[25];
};


// ************************************

const tt800::data_type tt800::MAX( 0xffffffff );

// ************************************

void tt800::refresh(void)
{
	static unsigned long mag01[2]={ 
        0x0, 0x8ebfd028 /* this is magic vector `a', don't change */
    };

	x[0] = x[7] ^ (x[0] >> 1) ^ mag01[ x[0] & 0x01 ];
	x[1] = x[8] ^ (x[1] >> 1) ^ mag01[ x[1] & 0x01 ];
	x[2] = x[9] ^ (x[2] >> 1) ^ mag01[ x[2] & 0x01 ];
	x[3] = x[10] ^ (x[3] >> 1) ^ mag01[ x[3] & 0x01 ];
	x[4] = x[11] ^ (x[4] >> 1) ^ mag01[ x[4] & 0x01 ];
	x[5] = x[12] ^ (x[5] >> 1) ^ mag01[ x[5] & 0x01 ];
	x[6] = x[13] ^ (x[6] >> 1) ^ mag01[ x[6] & 0x01 ];
	x[7] = x[14] ^ (x[7] >> 1) ^ mag01[ x[7] & 0x01 ];
	x[8] = x[15] ^ (x[8] >> 1) ^ mag01[ x[8] & 0x01 ];
	x[9] = x[16] ^ (x[9] >> 1) ^ mag01[ x[9] & 0x01 ];
	x[10] = x[17] ^ (x[10] >> 1) ^ mag01[ x[10] & 0x01 ];
	x[11] = x[18] ^ (x[11] >> 1) ^ mag01[ x[11] & 0x01 ];
	x[12] = x[19] ^ (x[12] >> 1) ^ mag01[ x[12] & 0x01 ];
	x[13] = x[20] ^ (x[13] >> 1) ^ mag01[ x[13] & 0x01 ];
	x[14] = x[21] ^ (x[14] >> 1) ^ mag01[ x[14] & 0x01 ];
	x[15] = x[22] ^ (x[15] >> 1) ^ mag01[ x[15] & 0x01 ];
	x[16] = x[23] ^ (x[16] >> 1) ^ mag01[ x[16] & 0x01 ];
	x[17] = x[24] ^ (x[17] >> 1) ^ mag01[ x[17] & 0x01 ];
	x[18] = x[0] ^ (x[18] >> 1) ^ mag01[ x[18] & 0x01 ];
	x[19] = x[1] ^ (x[19] >> 1) ^ mag01[ x[19] & 0x01 ];
	x[20] = x[2] ^ (x[20] >> 1) ^ mag01[ x[20] & 0x01 ];
	x[21] = x[3] ^ (x[21] >> 1) ^ mag01[ x[21] & 0x01 ];
	x[22] = x[4] ^ (x[22] >> 1) ^ mag01[ x[22] & 0x01 ];
	x[23] = x[5] ^ (x[23] >> 1) ^ mag01[ x[23] & 0x01 ];
	x[24] = x[6] ^ (x[24] >> 1) ^ mag01[ x[24] & 0x01 ];

	i=0;

	return;
}

// ************************************

tt800::tt800( tt800::data_type seed )
{
	short u;
	data_type v;
	v = seed * 1273713713 + 211719371;
	for( u=0; u<25; ++u )
	{
		x[u] = v;
		v ^= ( v << 17 ) + ( v >> 3 ) + 22111;
	}
	i=0;
}

// ************************************

tt800 tt800::operator++(int)
{
	tt800 ret = *this;
	++(*this);
	return ret;
}


// ************************************

bool tt800::operator==( const tt800& t2 ) const
{
	short u;

	if( i == t2.i )
	{
		for( u=0; u<25; ++u )
		{
			if( x[u] != t2.x[u] )
				return false;
		}
		return true;
	}

	return false;
}

// ************************************

#endif
