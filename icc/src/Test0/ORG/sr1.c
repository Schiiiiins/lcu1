/********************************************************************
 *
 *	SOURCE:   ./Test0/sr1.ic
 *	OUTPUT:   ./Test0/sr1.c
 *
 *******************************************************************/

static const char	iC_compiler[] =
"@(#)     $Id: icc.v,v 1.115 2007/03/26 12:27:14 jw Exp $";

#include	<icg.h>

static iC_Gt *	iC_l_[];

/********************************************************************
 *
 *	Gate list
 *
 *******************************************************************/

iC_Gt IX0_0    = { 1, -iC_INPX, iC_GATE, 0, "IX0.0", 0, 0, 0 };
iC_Gt IX0_2    = { 1, -iC_INPX, iC_GATE, 0, "IX0.2", 0, 0, &IX0_0 };
iC_Gt QX0_0    = { 1, -iC_FF, iC_GATE, 0, "QX0.0", 0, 0, &IX0_2 };
iC_Gt QX0_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.0_0", 0, &iC_l_[0], &QX0_0 };
iC_Gt QX0_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX0.0_1", &iC_l_[3], &iC_l_[5], &QX0_0_0 };
iC_Gt QX0_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.0_2", &iC_l_[8], &iC_l_[10], &QX0_0_1 };
iC_Gt QX0_1    = { 1, -iC_FF, iC_GATE, 0, "QX0.1", 0, 0, &QX0_0_2 };
iC_Gt QX0_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.1_0", 0, &iC_l_[14], &QX0_1 };
iC_Gt QX0_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.1_1", &iC_l_[17], &iC_l_[19], &QX0_1_0 };
iC_Gt QX0_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.1_2", &iC_l_[23], &iC_l_[25], &QX0_1_1 };
iC_Gt QX0_2    = { 1, -iC_AND, iC_GATE, 0, "QX0.2", 0, &iC_l_[29], &QX0_1_2 };
iC_Gt QX0_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.2_0", 0, &iC_l_[33], &QX0_2 };
iC_Gt QX0_2_1  = { 1, -iC_OR, iC_GATE, 0, "QX0.2_1", 0, &iC_l_[36], &QX0_2_0 };
iC_Gt QX0_3    = { 1, -iC_AND, iC_GATE, 0, "QX0.3", 0, &iC_l_[40], &QX0_2_1 };
iC_Gt QX0_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.3_0", 0, &iC_l_[44], &QX0_3 };
iC_Gt res      = { 1, -iC_AND, iC_GATE, 0, "res", 0, &iC_l_[47], &QX0_3_0 };

iC_Gt *		iC___Test0_sr1_list = &res;
iC_Gt **	iC_list[] = { &iC___Test0_sr1_list, 0, };

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QX0.0_0 */	&QX0_0, 0, 0,
/* QX0.0_1 */	&QX0_0, &iClock,	0, &IX0_0, 0,
/* QX0.0_2 */	&QX0_0, &iClock,	&IX0_2, &IX0_0, 0, 0,
/* QX0.1_0 */	&QX0_1, 0, 0,
/* QX0.1_1 */	&QX0_1, &iClock,	0, &res, &IX0_0, 0,
/* QX0.1_2 */	&QX0_1, &iClock,	&res, &IX0_0, 0, 0,
/* QX0.2 */	&QX0_2_1, 0, &IX0_0, 0,
/* QX0.2_0 */	&QX0_2, 0, 0,
/* QX0.2_1 */	0, &IX0_2, &IX0_0, 0,
/* QX0.3 */	&IX0_2, &IX0_0, 0, 0,
/* QX0.3_0 */	&QX0_3, 0, 0,
/* res */	&IX0_2, &IX0_0, 0, 0,
};
