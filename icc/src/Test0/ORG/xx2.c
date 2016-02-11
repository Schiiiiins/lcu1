/********************************************************************
 *
 *	SOURCE:   ./Test0/xx2.ic
 *	OUTPUT:   ./Test0/xx2.c
 *
 *******************************************************************/

static const char	iC_compiler[] =
"@(#)     $Id: icc.v,v 1.123 2008/09/06 01:32:56 jw Exp $ -O7";

#include	<icg.h>

static iC_Gt *	iC_l_[];

/********************************************************************
 *
 *	Gate list
 *
 *******************************************************************/

iC_Gt IB0      = { 1, -iC_INPW, iC_ARITH, 0, "IB0", 0, 0, 0 };
iC_Gt IB1      = { 1, -iC_INPW, iC_ARITH, 0, "IB1", 0, 0, &IB0 };
iC_Gt IB10     = { 1, -iC_INPW, iC_ARITH, 0, "IB10", 0, 0, &IB1 };
iC_Gt IB11     = { 1, -iC_INPW, iC_ARITH, 0, "IB11", 0, 0, &IB10 };
iC_Gt IB12     = { 1, -iC_INPW, iC_ARITH, 0, "IB12", 0, 0, &IB11 };
iC_Gt IB13     = { 1, -iC_INPW, iC_ARITH, 0, "IB13", 0, 0, &IB12 };
iC_Gt IB14     = { 1, -iC_INPW, iC_ARITH, 0, "IB14", 0, 0, &IB13 };
iC_Gt IB15     = { 1, -iC_INPW, iC_ARITH, 0, "IB15", 0, 0, &IB14 };
iC_Gt IB16     = { 1, -iC_INPW, iC_ARITH, 0, "IB16", 0, 0, &IB15 };
iC_Gt IB17     = { 1, -iC_INPW, iC_ARITH, 0, "IB17", 0, 0, &IB16 };
iC_Gt IB18     = { 1, -iC_INPW, iC_ARITH, 0, "IB18", 0, 0, &IB17 };
iC_Gt IB19     = { 1, -iC_INPW, iC_ARITH, 0, "IB19", 0, 0, &IB18 };
iC_Gt IB2      = { 1, -iC_INPW, iC_ARITH, 0, "IB2", 0, 0, &IB19 };
iC_Gt IB20     = { 1, -iC_INPW, iC_ARITH, 0, "IB20", 0, 0, &IB2 };
iC_Gt IB21     = { 1, -iC_INPW, iC_ARITH, 0, "IB21", 0, 0, &IB20 };
iC_Gt IB22     = { 1, -iC_INPW, iC_ARITH, 0, "IB22", 0, 0, &IB21 };
iC_Gt IB23     = { 1, -iC_INPW, iC_ARITH, 0, "IB23", 0, 0, &IB22 };
iC_Gt IB3      = { 1, -iC_INPW, iC_ARITH, 0, "IB3", 0, 0, &IB23 };
iC_Gt IB4      = { 1, -iC_INPW, iC_ARITH, 0, "IB4", 0, 0, &IB3 };
iC_Gt IB5      = { 1, -iC_INPW, iC_ARITH, 0, "IB5", 0, 0, &IB4 };
iC_Gt IB6      = { 1, -iC_INPW, iC_ARITH, 0, "IB6", 0, 0, &IB5 };
iC_Gt IB7      = { 1, -iC_INPW, iC_ARITH, 0, "IB7", 0, 0, &IB6 };
iC_Gt IB8      = { 1, -iC_INPW, iC_ARITH, 0, "IB8", 0, 0, &IB7 };
iC_Gt IB9      = { 1, -iC_INPW, iC_ARITH, 0, "IB9", 0, 0, &IB8 };
iC_Gt a0       = { 1, -iC_ARN, iC_GATE, 0, "a0", 0, &iC_l_[0], &IB9 };
iC_Gt a1       = { 1, -iC_ARN, iC_GATE, 0, "a1", 0, &iC_l_[3], &a0 };
iC_Gt a10      = { 1, -iC_ARN, iC_GATE, 0, "a10", 0, &iC_l_[6], &a1 };
iC_Gt a11      = { 1, -iC_ARN, iC_GATE, 0, "a11", 0, &iC_l_[9], &a10 };
iC_Gt a12      = { 1, -iC_ARN, iC_GATE, 0, "a12", 0, &iC_l_[12], &a11 };
iC_Gt a13      = { 1, -iC_ARN, iC_GATE, 0, "a13", 0, &iC_l_[15], &a12 };
iC_Gt a14      = { 1, -iC_ARN, iC_GATE, 0, "a14", 0, &iC_l_[18], &a13 };
iC_Gt a15      = { 1, -iC_ARN, iC_GATE, 0, "a15", 0, &iC_l_[21], &a14 };
iC_Gt a16      = { 1, -iC_ARN, iC_GATE, 0, "a16", 0, &iC_l_[24], &a15 };
iC_Gt a17      = { 1, -iC_ARN, iC_GATE, 0, "a17", 0, &iC_l_[27], &a16 };
iC_Gt a18      = { 1, -iC_ARN, iC_GATE, 0, "a18", 0, &iC_l_[30], &a17 };
iC_Gt a19      = { 1, -iC_ARN, iC_GATE, 0, "a19", 0, &iC_l_[33], &a18 };
iC_Gt a2       = { 1, -iC_ARN, iC_GATE, 0, "a2", 0, &iC_l_[36], &a19 };
iC_Gt a20      = { 1, -iC_ARN, iC_GATE, 0, "a20", 0, &iC_l_[39], &a2 };
iC_Gt a21      = { 1, -iC_ARN, iC_GATE, 0, "a21", 0, &iC_l_[42], &a20 };
iC_Gt a22      = { 1, -iC_ARN, iC_GATE, 0, "a22", 0, &iC_l_[45], &a21 };
iC_Gt a23      = { 1, -iC_ARN, iC_GATE, 0, "a23", 0, &iC_l_[48], &a22 };
iC_Gt a3       = { 1, -iC_ARN, iC_GATE, 0, "a3", 0, &iC_l_[51], &a23 };
iC_Gt a4       = { 1, -iC_ARN, iC_GATE, 0, "a4", 0, &iC_l_[54], &a3 };
iC_Gt a5       = { 1, -iC_ARN, iC_GATE, 0, "a5", 0, &iC_l_[57], &a4 };
iC_Gt a6       = { 1, -iC_ARN, iC_GATE, 0, "a6", 0, &iC_l_[60], &a5 };
iC_Gt a7       = { 1, -iC_ARN, iC_GATE, 0, "a7", 0, &iC_l_[63], &a6 };
iC_Gt a8       = { 1, -iC_ARN, iC_GATE, 0, "a8", 0, &iC_l_[66], &a7 };
iC_Gt a9       = { 1, -iC_ARN, iC_GATE, 0, "a9", 0, &iC_l_[69], &a8 };

iC_Gt *		iC___Test0_xx2_list = &a9;
iC_Gt **	iC_list[] = { &iC___Test0_xx2_list, 0, };

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* a0 */	(iC_Gt*)0, &IB0, 0,
/* a1 */	(iC_Gt*)0, &IB1, 0,
/* a10 */	(iC_Gt*)0, &IB10, 0,
/* a11 */	(iC_Gt*)0, &IB11, 0,
/* a12 */	(iC_Gt*)0, &IB12, 0,
/* a13 */	(iC_Gt*)0, &IB13, 0,
/* a14 */	(iC_Gt*)0, &IB14, 0,
/* a15 */	(iC_Gt*)0, &IB15, 0,
/* a16 */	(iC_Gt*)0, &IB16, 0,
/* a17 */	(iC_Gt*)0, &IB17, 0,
/* a18 */	(iC_Gt*)0, &IB18, 0,
/* a19 */	(iC_Gt*)0, &IB19, 0,
/* a2 */	(iC_Gt*)0, &IB2, 0,
/* a20 */	(iC_Gt*)0, &IB20, 0,
/* a21 */	(iC_Gt*)0, &IB21, 0,
/* a22 */	(iC_Gt*)0, &IB22, 0,
/* a23 */	(iC_Gt*)0, &IB23, 0,
/* a3 */	(iC_Gt*)0, &IB3, 0,
/* a4 */	(iC_Gt*)0, &IB4, 0,
/* a5 */	(iC_Gt*)0, &IB5, 0,
/* a6 */	(iC_Gt*)0, &IB6, 0,
/* a7 */	(iC_Gt*)0, &IB7, 0,
/* a8 */	(iC_Gt*)0, &IB8, 0,
/* a9 */	(iC_Gt*)0, &IB9, 0,
};
