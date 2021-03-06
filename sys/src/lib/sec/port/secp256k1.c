/* this file has been autogenerated in Jehanne
 * using a port of 9front mpc tool:
 * 
 *      echo '#include "os.h"' > secp256k1.c
 *      echo '#include <mp.h>' >> secp256k1.c
 *      mpc secp256k1.mp >> secp256k1.c
 */
#include "os.h"
#include <mp.h>
void secp256k1(mpint *p, mpint *a, mpint *b, mpint *x, mpint *y, mpint *n, mpint *h){
	strtomp("3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFFFFFFFFFC2F", nil, 16, p);
	mpassign(mpzero, a);
	uitomp(7UL, b);
	mpassign(mpzero, x);
	mpassign(mpzero, y);
	mpassign(mpzero, n);
	mpassign(mpone, h);
	}
