/********************************************************************
 *
 *	SOURCE:   ./Test0/aa4z6.ic
 *	OUTPUT:   ./Test0/aa4z6.c
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

iC_Gt IX0_1    = { 1, -iC_INPX, iC_GATE, 0, "IX0.1", 0, 0, 0 };
iC_Gt IX0_2    = { 1, -iC_INPX, iC_GATE, 0, "IX0.2", 0, 0, &IX0_1 };
iC_Gt IX0_4    = { 1, -iC_INPX, iC_GATE, 0, "IX0.4", 0, 0, &IX0_2 };
iC_Gt IX0_5    = { 1, -iC_INPX, iC_GATE, 0, "IX0.5", 0, 0, &IX0_4 };
iC_Gt QX0_1    = { 1, -iC_AND, iC_GATE, 0, "QX0.1", 0, &iC_l_[0], &IX0_5 };
iC_Gt QX0_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.1_0", 0, &iC_l_[4], &QX0_1 };
iC_Gt QX0_2    = { 1, -iC_AND, iC_GATE, 0, "QX0.2", 0, &iC_l_[7], &QX0_1_0 };
iC_Gt QX0_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.2_0", 0, &iC_l_[13], &QX0_2 };
iC_Gt QX0_3    = { 1, -iC_AND, iC_GATE, 0, "QX0.3", 0, &iC_l_[16], &QX0_2_0 };
iC_Gt QX0_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.3_0", 0, &iC_l_[22], &QX0_3 };
iC_Gt QX0_4    = { 1, -iC_AND, iC_GATE, 0, "QX0.4", 0, &iC_l_[25], &QX0_3_0 };
iC_Gt QX0_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.4_0", 0, &iC_l_[31], &QX0_4 };
iC_Gt QX0_5    = { 1, -iC_AND, iC_GATE, 0, "QX0.5", 0, &iC_l_[34], &QX0_4_0 };
iC_Gt QX0_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.5_0", 0, &iC_l_[40], &QX0_5 };
iC_Gt QX0_6    = { 1, -iC_AND, iC_GATE, 0, "QX0.6", 0, &iC_l_[43], &QX0_5_0 };
iC_Gt QX0_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.6_0", 0, &iC_l_[47], &QX0_6 };

iC_Gt *		iC___Test0_aa4z6_list = &QX0_6_0;
iC_Gt **	iC_list[] = { &iC___Test0_aa4z6_list, 0, };

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QX0.1 */	&IX0_5, &IX0_4, 0, 0,
/* QX0.1_0 */	&QX0_1, 0, 0,
/* QX0.2 */	&IX0_5, &IX0_4, &IX0_2, &IX0_1, 0, 0,
/* QX0.2_0 */	&QX0_2, 0, 0,
/* QX0.3 */	&IX0_5, &IX0_4, &IX0_2, &IX0_1, 0, 0,
/* QX0.3_0 */	&QX0_3, 0, 0,
/* QX0.4 */	&IX0_5, &IX0_4, &IX0_2, &IX0_1, 0, 0,
/* QX0.4_0 */	&QX0_4, 0, 0,
/* QX0.5 */	&IX0_5, &IX0_4, &IX0_2, &IX0_1, 0, 0,
/* QX0.5_0 */	&QX0_5, 0, 0,
/* QX0.6 */	&IX0_5, &IX0_4, 0, 0,
/* QX0.6_0 */	&QX0_6, 0, 0,
};
