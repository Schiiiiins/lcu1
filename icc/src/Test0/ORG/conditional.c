/********************************************************************
 *
 *	SOURCE:   ./Test0/conditional.ic
 *	OUTPUT:   ./Test0/conditional.c
 *
 *******************************************************************/

static const char	iC_compiler[] =
"@(#)     $Id: icc.v,v 1.115 2007/03/26 12:27:14 jw Exp $";

#include	<icg.h>

#define iC_MV(n)	iC_gf->gt_rlist[n]->gt_new
#define iC_AV(n)	iC_gf->gt_list[n]->gt_new
#define iC_LV(n)	(iC_gf->gt_list[n]->gt_val < 0 ? 1 : 0)
#define iC_AA(n,v)	iC_assignA(iC_gf->gt_list[n], v)
#define iC_LA(n,v)	iC_assignL(iC_gf->gt_list[n], v)
static iC_Gt *	iC_l_[];

/********************************************************************
 *
 *	Gate list
 *
 *******************************************************************/

iC_Gt IB1      = { 1, -iC_INPW, iC_ARITH, 0, "IB1", 0, 0, 0 };
iC_Gt IB2      = { 1, -iC_INPW, iC_ARITH, 0, "IB2", 0, 0, &IB1 };
iC_Gt IX0_0    = { 1, -iC_INPX, iC_GATE, 0, "IX0.0", 0, 0, &IB2 };
iC_Gt IX0_1    = { 1, -iC_INPX, iC_GATE, 0, "IX0.1", 0, 0, &IX0_0 };
iC_Gt QB1      = { 1, -iC_ARN, iC_ARITH, 0, "QB1", 0, &iC_l_[0], &IX0_1 };
iC_Gt QB1_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB1_0", 0, &iC_l_[4], &QB1 };
iC_Gt QB2      = { 1, -iC_ARNC, iC_ARITH, 0, "QB2", 0, 0, &QB1_0 };
iC_Gt QB2_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB2_0", 0, &iC_l_[7], &QB2 };
iC_Gt QB3      = { 1, -iC_ARNC, iC_ARITH, 0, "QB3", 0, 0, &QB2_0 };
iC_Gt QB3_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB3_0", 0, &iC_l_[10], &QB3 };
iC_Gt QB4      = { 1, -iC_ARNC, iC_ARITH, 0, "QB4", 0, 0, &QB3_0 };
iC_Gt QB4_0    = { 1, -iC_ARN, iC_OUTW, 0, "QB4_0", 0, &iC_l_[13], &QB4 };
static iC_Gt _f1_1   = { 1, -iC_OR, iC_F_CF, 0, "_f1_1", &iC_l_[16], &iC_l_[26], &QB4_0 };
static iC_Gt _f2_1   = { 1, -iC_OR, iC_F_CF, 0, "_f2_1", &iC_l_[29], &iC_l_[34], &_f1_1 };
iC_Gt a        = { 1, -iC_ARNC, iC_ARITH, 0, "a", 0, 0, &_f2_1 };
iC_Gt b        = { 1, -iC_ARNC, iC_ARITH, 0, "b", 0, 0, &a };

iC_Gt *		iC___Test0_conditional_list = &b;
iC_Gt **	iC_list[] = { &iC___Test0_conditional_list, 0, };
static int	iC_tVar;

/********************************************************************
 *
 *	Literal blocks and embedded C fragment functions
 *
 *******************************************************************/

static int iC_2(iC_Gt * iC_gf) {
#line 38 "./Test0/conditional.ic"
	return iC_MV(1)?:iC_MV(2);
#line 57 "./Test0/conditional.c"
}

static int iC_3(iC_Gt * iC_gf) {
    if (iC_gf->gt_val < 0)
#line 40 "./Test0/conditional.ic"
{
    iC_AA(2 , iC_AV(3) ? : iC_AV(4));		/* compiled as C statement */
    iC_AA(5 , (iC_tVar = iC_AV(6), iC_AA(6 , iC_tVar + 1), iC_tVar) ? : iC_AV(4));		/* conditional with side effect */
    iC_AA(7 , (iC_tVar = iC_AV(8), iC_AA(8 , iC_tVar + 1), iC_tVar) ? (iC_tVar = iC_AV(8), iC_AA(8 , iC_tVar + 1), iC_tVar) : iC_AV(4));	/* conditional with active side effect */
}
#line 68 "./Test0/conditional.c"
    return 0;
}

static int iC_4(iC_Gt * iC_gf) {
    if (iC_gf->gt_val < 0)
#line 46 "./Test0/conditional.ic"
{
    iC_AA(2 , iC_AA(3 , 0));			/* reset a and b */
}
#line 78 "./Test0/conditional.c"
    return 0;
}

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QB1 */	(iC_Gt*)iC_2, &IB1, &IB2, 0,
/* QB1_0 */	(iC_Gt*)0, &QB1, 0,
/* QB2_0 */	(iC_Gt*)0, &QB2, 0,
/* QB3_0 */	(iC_Gt*)0, &QB3, 0,
/* QB4_0 */	(iC_Gt*)0, &QB4, 0,
/* _f1_1 */	(iC_Gt*)iC_3, &iClock, &QB2, &IB1, &IB2, &QB3, &a, &QB4,
		&b, (iC_Gt*)0x3769,	&IX0_0, 0, 0,
/* _f2_1 */	(iC_Gt*)iC_4, &iClock, &a, &b, (iC_Gt*)0xd,
		&IX0_1, 0, 0,
};
