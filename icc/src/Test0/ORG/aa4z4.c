/********************************************************************
 *
 *	SOURCE:   ./Test0/aa4z4.ic
 *	OUTPUT:   ./Test0/aa4z4.c
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

iC_Gt QB1      = { 1, -iC_ARN, iC_ARITH, 0, "QB1", 0, &iC_l_[0], 0 };
iC_Gt QB1_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB1_0", 0, &iC_l_[3], &QB1 };
iC_Gt QB2      = { 1, -iC_ARN, iC_ARITH, 0, "QB2", 0, &iC_l_[6], &QB1_0 };
iC_Gt QB2_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB2_0", 0, &iC_l_[9], &QB2 };
iC_Gt QB3      = { 1, -iC_ARN, iC_ARITH, 0, "QB3", 0, &iC_l_[12], &QB2_0 };
iC_Gt QB3_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB3_0", 0, &iC_l_[15], &QB3 };
iC_Gt QB4      = { 1, -iC_ARN, iC_ARITH, 0, "QB4", 0, &iC_l_[18], &QB3_0 };
iC_Gt QB4_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB4_0", 0, &iC_l_[21], &QB4 };
iC_Gt QB5      = { 1, -iC_ARN, iC_ARITH, 0, "QB5", 0, &iC_l_[24], &QB4_0 };
iC_Gt QB5_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB5_0", 0, &iC_l_[27], &QB5 };
iC_Gt QB6      = { 1, -iC_ARN, iC_ARITH, 0, "QB6", 0, &iC_l_[30], &QB5_0 };
iC_Gt QB6_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB6_0", 0, &iC_l_[33], &QB6 };
extern iC_Gt iConst; /* 6 */

iC_Gt *		iC___Test0_aa4z4_list = &QB6_0;
iC_Gt **	iC_list[] = { &iC___Test0_aa4z4_list, 0, };

/********************************************************************
 *
 *	Literal blocks and embedded C fragment functions
 *
 *******************************************************************/

static int iC_10(iC_Gt * iC_gf) {
#line 46 "./Test0/aa4z4.ic"
	return ((4+5)+4)+4;
#line 49 "./Test0/aa4z4.c"
}

static int iC_11(iC_Gt * iC_gf) {
#line 47 "./Test0/aa4z4.ic"
	return 4+(4+((4+4)+(4+5)));
#line 55 "./Test0/aa4z4.c"
}

static int iC_12(iC_Gt * iC_gf) {
#line 48 "./Test0/aa4z4.ic"
	return ((4+5)+((4+4)+(4+5)))+((4+4)+(4+5));
#line 61 "./Test0/aa4z4.c"
}

static int iC_13(iC_Gt * iC_gf) {
#line 49 "./Test0/aa4z4.ic"
	return (4+4)+((4+4)+((4+4)+(4+5)));
#line 67 "./Test0/aa4z4.c"
}

static int iC_14(iC_Gt * iC_gf) {
#line 50 "./Test0/aa4z4.ic"
	return (((4+4)+(4+5))+(4+4))+(4+4);
#line 73 "./Test0/aa4z4.c"
}

static int iC_15(iC_Gt * iC_gf) {
#line 51 "./Test0/aa4z4.ic"
	return 4+(4+5);
#line 79 "./Test0/aa4z4.c"
}

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QB1 */	(iC_Gt*)iC_10, &iConst, 0,
/* QB1_0 */	(iC_Gt*)0, &QB1, 0,
/* QB2 */	(iC_Gt*)iC_11, &iConst, 0,
/* QB2_0 */	(iC_Gt*)0, &QB2, 0,
/* QB3 */	(iC_Gt*)iC_12, &iConst, 0,
/* QB3_0 */	(iC_Gt*)0, &QB3, 0,
/* QB4 */	(iC_Gt*)iC_13, &iConst, 0,
/* QB4_0 */	(iC_Gt*)0, &QB4, 0,
/* QB5 */	(iC_Gt*)iC_14, &iConst, 0,
/* QB5_0 */	(iC_Gt*)0, &QB5, 0,
/* QB6 */	(iC_Gt*)iC_15, &iConst, 0,
/* QB6_0 */	(iC_Gt*)0, &QB6, 0,
};
