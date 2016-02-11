/********************************************************************
 *
 *	SOURCE:   ./Test0/aa4z5.ic
 *	OUTPUT:   ./Test0/aa4z5.c
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

iC_Gt IB4      = { 1, -iC_INPW, iC_ARITH, 0, "IB4", 0, 0, 0 };
iC_Gt IB5      = { 1, -iC_INPW, iC_ARITH, 0, "IB5", 0, 0, &IB4 };
iC_Gt QB1      = { 1, -iC_ARN, iC_ARITH, 0, "QB1", 0, &iC_l_[0], &IB5 };
iC_Gt QB1_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB1_0", 0, &iC_l_[4], &QB1 };
iC_Gt QB2      = { 1, -iC_ARN, iC_ARITH, 0, "QB2", 0, &iC_l_[7], &QB1_0 };
iC_Gt QB2_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB2_0", 0, &iC_l_[11], &QB2 };
iC_Gt QB3      = { 1, -iC_ARN, iC_ARITH, 0, "QB3", 0, &iC_l_[14], &QB2_0 };
iC_Gt QB3_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB3_0", 0, &iC_l_[18], &QB3 };
iC_Gt QB4      = { 1, -iC_ARN, iC_ARITH, 0, "QB4", 0, &iC_l_[21], &QB3_0 };
iC_Gt QB4_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB4_0", 0, &iC_l_[25], &QB4 };
iC_Gt QB5      = { 1, -iC_ARN, iC_ARITH, 0, "QB5", 0, &iC_l_[28], &QB4_0 };
iC_Gt QB5_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB5_0", 0, &iC_l_[32], &QB5 };
iC_Gt QB6      = { 1, -iC_ARN, iC_ARITH, 0, "QB6", 0, &iC_l_[35], &QB5_0 };
iC_Gt QB6_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB6_0", 0, &iC_l_[39], &QB6 };

iC_Gt *		iC___Test0_aa4z5_list = &QB6_0;
iC_Gt **	iC_list[] = { &iC___Test0_aa4z5_list, 0, };

/********************************************************************
 *
 *	Literal blocks and embedded C fragment functions
 *
 *******************************************************************/

static int iC_9(iC_Gt * iC_gf) {
#line 46 "./Test0/aa4z5.ic"
	return ((iC_MV(1)+iC_MV(2))+iC_MV(1))+iC_MV(1);
#line 50 "./Test0/aa4z5.c"
}

static int iC_10(iC_Gt * iC_gf) {
#line 47 "./Test0/aa4z5.ic"
	return iC_MV(1)+(iC_MV(1)+((4+4)+(iC_MV(1)+iC_MV(2))));
#line 56 "./Test0/aa4z5.c"
}

static int iC_11(iC_Gt * iC_gf) {
#line 48 "./Test0/aa4z5.ic"
	return ((iC_MV(1)+iC_MV(2))+((4+4)+(iC_MV(1)+iC_MV(2))))+((4+4)+(iC_MV(1)+iC_MV(2)));
#line 62 "./Test0/aa4z5.c"
}

static int iC_12(iC_Gt * iC_gf) {
#line 49 "./Test0/aa4z5.ic"
	return (4+4)+((4+4)+((4+4)+(iC_MV(1)+iC_MV(2))));
#line 68 "./Test0/aa4z5.c"
}

static int iC_13(iC_Gt * iC_gf) {
#line 50 "./Test0/aa4z5.ic"
	return (((4+4)+(iC_MV(1)+iC_MV(2)))+(4+4))+(4+4);
#line 74 "./Test0/aa4z5.c"
}

static int iC_14(iC_Gt * iC_gf) {
#line 51 "./Test0/aa4z5.ic"
	return iC_MV(1)+(iC_MV(1)+iC_MV(2));
#line 80 "./Test0/aa4z5.c"
}

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QB1 */	(iC_Gt*)iC_9, &IB4, &IB5, 0,
/* QB1_0 */	(iC_Gt*)0, &QB1, 0,
/* QB2 */	(iC_Gt*)iC_10, &IB4, &IB5, 0,
/* QB2_0 */	(iC_Gt*)0, &QB2, 0,
/* QB3 */	(iC_Gt*)iC_11, &IB4, &IB5, 0,
/* QB3_0 */	(iC_Gt*)0, &QB3, 0,
/* QB4 */	(iC_Gt*)iC_12, &IB4, &IB5, 0,
/* QB4_0 */	(iC_Gt*)0, &QB4, 0,
/* QB5 */	(iC_Gt*)iC_13, &IB4, &IB5, 0,
/* QB5_0 */	(iC_Gt*)0, &QB5, 0,
/* QB6 */	(iC_Gt*)iC_14, &IB4, &IB5, 0,
/* QB6_0 */	(iC_Gt*)0, &QB6, 0,
};
