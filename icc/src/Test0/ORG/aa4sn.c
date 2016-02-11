/********************************************************************
 *
 *	SOURCE:   ./Test0/aa4sn.ic
 *	OUTPUT:   ./Test0/aa4sn.c
 *
 *******************************************************************/

static const char	iC_compiler[] =
"@(#)     $Id: icc.v,v 1.115 2007/03/26 12:27:14 jw Exp $";

#include	<icg.h>

#define iC_MV(n)	iC_gf->gt_rlist[n]->gt_new
static iC_Gt *	iC_l_[];

/********************************************************************
 *
 *	Gate list
 *
 *******************************************************************/

iC_Gt IB1      = { 1, -iC_INPW, iC_ARITH, 0, "IB1", 0, 0, 0 };
iC_Gt IB2      = { 1, -iC_INPW, iC_ARITH, 0, "IB2", 0, 0, &IB1 };
iC_Gt QB1_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB1_0", 0, &iC_l_[0], &IB2 };
iC_Gt QB2      = { 1, -iC_OR, iC_ARITH, 0, "QB2", 0, &iC_l_[3], &QB1_0 };
iC_Gt QB2_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB2_0", 0, &iC_l_[6], &QB2 };
iC_Gt x        = { 1, -iC_ARN, iC_ARITH, 0, "x", 0, &iC_l_[9], &QB2_0 };
iC_Gt y        = { 1, -iC_ARN, iC_GATE, 0, "y", 0, &iC_l_[12], &x };
iC_Gt QB1      = { 1, -iC_ALIAS, iC_ARITH, 0, "QB1", 0, (iC_Gt**)&x, &y, 0 };

iC_Gt *		iC___Test0_aa4sn_list = &QB1;
iC_Gt **	iC_list[] = { &iC___Test0_aa4sn_list, 0, };

/********************************************************************
 *
 *	Literal blocks and embedded C fragment functions
 *
 *******************************************************************/

static int iC_2(iC_Gt * iC_gf) {
#line 4 "./Test0/aa4sn.ic"
	return iC_MV(1)+2;
#line 44 "./Test0/aa4sn.c"
}

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QB1_0 */	(iC_Gt*)0, &x, 0,
/* QB2 */	&y, 0, 0,
/* QB2_0 */	(iC_Gt*)0, &QB2, 0,
/* x */		(iC_Gt*)iC_2, &IB1, 0,
/* y */		(iC_Gt*)iC_2, &IB2, 0,
};
