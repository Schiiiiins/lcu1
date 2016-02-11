/********************************************************************
 *
 *	SOURCE:   ./Test0/aa4a.ic
 *	OUTPUT:   ./Test0/aa4a.c
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
iC_Gt IX0_1    = { 1, -iC_INPX, iC_GATE, 0, "IX0.1", 0, 0, &IX0_0 };
iC_Gt IX0_2    = { 1, -iC_INPX, iC_GATE, 0, "IX0.2", 0, 0, &IX0_1 };
iC_Gt IX0_3    = { 1, -iC_INPX, iC_GATE, 0, "IX0.3", 0, 0, &IX0_2 };
iC_Gt IX0_4    = { 1, -iC_INPX, iC_GATE, 0, "IX0.4", 0, 0, &IX0_3 };
iC_Gt IX0_5    = { 1, -iC_INPX, iC_GATE, 0, "IX0.5", 0, 0, &IX0_4 };
iC_Gt IX0_6    = { 1, -iC_INPX, iC_GATE, 0, "IX0.6", 0, 0, &IX0_5 };
iC_Gt IX0_7    = { 1, -iC_INPX, iC_GATE, 0, "IX0.7", 0, 0, &IX0_6 };
iC_Gt QX0_0    = { 1, -iC_OR, iC_GATE, 0, "QX0.0", 0, &iC_l_[0], &IX0_7 };
iC_Gt QX0_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.0_0", 0, &iC_l_[4], &QX0_0 };
iC_Gt QX0_0_1  = { 1, -iC_AND, iC_GATE, 0, "QX0.0_1", 0, &iC_l_[7], &QX0_0_0 };
iC_Gt QX0_0_2  = { 1, -iC_OR, iC_GATE, 0, "QX0.0_2", 0, &iC_l_[13], &QX0_0_1 };
iC_Gt QX0_0_3  = { 1, -iC_OR, iC_GATE, 0, "QX0.0_3", 0, &iC_l_[17], &QX0_0_2 };
iC_Gt QX0_0_4  = { 1, -iC_AND, iC_GATE, 0, "QX0.0_4", 0, &iC_l_[21], &QX0_0_3 };

iC_Gt *		iC___Test0_aa4a_list = &QX0_0_4;
iC_Gt **	iC_list[] = { &iC___Test0_aa4a_list, 0, };

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QX0.0 */	&QX0_0_1, &IX0_6, 0, 0,
/* QX0.0_0 */	&QX0_0, 0, 0,
/* QX0.0_1 */	&QX0_0_3, &QX0_0_2, &IX0_7, &IX0_3, 0, 0,
/* QX0.0_2 */	&QX0_0_4, &IX0_2, 0, 0,
/* QX0.0_3 */	&IX0_5, &IX0_4, 0, 0,
/* QX0.0_4 */	&IX0_1, &IX0_0, 0, 0,
};
