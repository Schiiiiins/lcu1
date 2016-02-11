/********************************************************************
 *
 *	SOURCE:   ./Test0/barfy.ic
 *	OUTPUT:   ./Test0/barfy.c
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

iC_Gt IB8      = { 1, -iC_INPW, iC_ARITH, 0, "IB8", 0, 0, 0 };
iC_Gt IX0_0    = { 1, -iC_INPX, iC_GATE, 0, "IX0.0", 0, 0, &IB8 };
iC_Gt IX0_2    = { 1, -iC_INPX, iC_GATE, 0, "IX0.2", 0, 0, &IX0_0 };
iC_Gt IX0_3    = { 1, -iC_INPX, iC_GATE, 0, "IX0.3", 0, 0, &IX0_2 };
iC_Gt IX0_4    = { 1, -iC_INPX, iC_GATE, 0, "IX0.4", 0, 0, &IX0_3 };
iC_Gt IX0_5    = { 1, -iC_INPX, iC_GATE, 0, "IX0.5", 0, 0, &IX0_4 };
iC_Gt IX0_6    = { 1, -iC_INPX, iC_GATE, 0, "IX0.6", 0, 0, &IX0_5 };
iC_Gt IX0_7    = { 1, -iC_INPX, iC_GATE, 0, "IX0.7", 0, 0, &IX0_6 };
iC_Gt QX0_0    = { 1, -iC_FF, iC_GATE, 0, "QX0.0", 0, 0, &IX0_7 };
iC_Gt QX0_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.0_0", 0, &iC_l_[0], &QX0_0 };
iC_Gt QX0_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX0.0_1", &iC_l_[3], &iC_l_[5], &QX0_0_0 };
iC_Gt QX0_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.0_2", &iC_l_[8], &iC_l_[10], &QX0_0_1 };
iC_Gt QX0_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX0.0_3", 0, 0, &QX0_0_2 };
iC_Gt QX0_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX0.0_4", &iC_l_[14], &iC_l_[16], &QX0_0_3 };
iC_Gt QX0_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX0.0_5", &iC_l_[20], &iC_l_[22], &QX0_0_4 };
iC_Gt QX0_1    = { 1, -iC_FF, iC_GATE, 0, "QX0.1", 0, 0, &QX0_0_5 };
iC_Gt QX0_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.1_0", 0, &iC_l_[26], &QX0_1 };
iC_Gt QX0_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.1_1", &iC_l_[29], &iC_l_[31], &QX0_1_0 };
iC_Gt QX0_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.1_2", &iC_l_[35], &iC_l_[37], &QX0_1_1 };
iC_Gt QX0_2    = { 1, -iC_FF, iC_GATE, 0, "QX0.2", 0, 0, &QX0_1_2 };
iC_Gt QX0_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.2_0", 0, &iC_l_[41], &QX0_2 };
iC_Gt QX0_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.2_1", &iC_l_[44], &iC_l_[46], &QX0_2_0 };
iC_Gt QX0_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.2_2", &iC_l_[50], &iC_l_[52], &QX0_2_1 };
iC_Gt QX0_3    = { 1, -iC_FF, iC_GATE, 0, "QX0.3", 0, 0, &QX0_2_2 };
iC_Gt QX0_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.3_0", 0, &iC_l_[56], &QX0_3 };
iC_Gt QX0_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.3_1", &iC_l_[59], &iC_l_[61], &QX0_3_0 };
iC_Gt QX0_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.3_2", &iC_l_[65], &iC_l_[67], &QX0_3_1 };
iC_Gt QX0_4    = { 1, -iC_FF, iC_GATE, 0, "QX0.4", 0, 0, &QX0_3_2 };
iC_Gt QX0_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.4_0", 0, &iC_l_[71], &QX0_4 };
iC_Gt QX0_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.4_1", &iC_l_[74], &iC_l_[76], &QX0_4_0 };
iC_Gt QX0_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.4_2", &iC_l_[80], &iC_l_[82], &QX0_4_1 };
iC_Gt QX0_5    = { 1, -iC_FF, iC_GATE, 0, "QX0.5", 0, 0, &QX0_4_2 };
iC_Gt QX0_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.5_0", 0, &iC_l_[86], &QX0_5 };
iC_Gt QX0_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.5_1", &iC_l_[89], &iC_l_[91], &QX0_5_0 };
iC_Gt QX0_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.5_2", &iC_l_[95], &iC_l_[97], &QX0_5_1 };
iC_Gt QX0_6    = { 1, -iC_FF, iC_GATE, 0, "QX0.6", 0, 0, &QX0_5_2 };
iC_Gt QX0_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.6_0", 0, &iC_l_[101], &QX0_6 };
iC_Gt QX0_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.6_1", &iC_l_[104], &iC_l_[106], &QX0_6_0 };
iC_Gt QX0_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX0.6_2", &iC_l_[110], &iC_l_[112], &QX0_6_1 };
iC_Gt QX0_7    = { 1, -iC_FF, iC_GATE, 0, "QX0.7", 0, 0, &QX0_6_2 };
iC_Gt QX0_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX0.7_0", 0, &iC_l_[116], &QX0_7 };
iC_Gt QX0_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX0.7_1", &iC_l_[119], &iC_l_[121], &QX0_7_0 };
iC_Gt QX0_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX0.7_2", &iC_l_[125], &iC_l_[127], &QX0_7_1 };
iC_Gt QX1_0    = { 1, -iC_FF, iC_GATE, 0, "QX1.0", 0, 0, &QX0_7_2 };
iC_Gt QX1_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.0_0", 0, &iC_l_[130], &QX1_0 };
iC_Gt QX1_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX1.0_1", &iC_l_[133], &iC_l_[135], &QX1_0_0 };
iC_Gt QX1_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.0_2", &iC_l_[138], &iC_l_[140], &QX1_0_1 };
iC_Gt QX1_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX1.0_3", 0, 0, &QX1_0_2 };
iC_Gt QX1_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX1.0_4", &iC_l_[144], &iC_l_[146], &QX1_0_3 };
iC_Gt QX1_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX1.0_5", &iC_l_[150], &iC_l_[152], &QX1_0_4 };
iC_Gt QX1_1    = { 1, -iC_FF, iC_GATE, 0, "QX1.1", 0, 0, &QX1_0_5 };
iC_Gt QX1_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.1_0", 0, &iC_l_[156], &QX1_1 };
iC_Gt QX1_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.1_1", &iC_l_[159], &iC_l_[161], &QX1_1_0 };
iC_Gt QX1_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.1_2", &iC_l_[165], &iC_l_[167], &QX1_1_1 };
iC_Gt QX1_2    = { 1, -iC_FF, iC_GATE, 0, "QX1.2", 0, 0, &QX1_1_2 };
iC_Gt QX1_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.2_0", 0, &iC_l_[171], &QX1_2 };
iC_Gt QX1_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.2_1", &iC_l_[174], &iC_l_[176], &QX1_2_0 };
iC_Gt QX1_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.2_2", &iC_l_[180], &iC_l_[182], &QX1_2_1 };
iC_Gt QX1_3    = { 1, -iC_FF, iC_GATE, 0, "QX1.3", 0, 0, &QX1_2_2 };
iC_Gt QX1_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.3_0", 0, &iC_l_[186], &QX1_3 };
iC_Gt QX1_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.3_1", &iC_l_[189], &iC_l_[191], &QX1_3_0 };
iC_Gt QX1_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.3_2", &iC_l_[195], &iC_l_[197], &QX1_3_1 };
iC_Gt QX1_4    = { 1, -iC_FF, iC_GATE, 0, "QX1.4", 0, 0, &QX1_3_2 };
iC_Gt QX1_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.4_0", 0, &iC_l_[201], &QX1_4 };
iC_Gt QX1_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.4_1", &iC_l_[204], &iC_l_[206], &QX1_4_0 };
iC_Gt QX1_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.4_2", &iC_l_[210], &iC_l_[212], &QX1_4_1 };
iC_Gt QX1_5    = { 1, -iC_FF, iC_GATE, 0, "QX1.5", 0, 0, &QX1_4_2 };
iC_Gt QX1_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.5_0", 0, &iC_l_[216], &QX1_5 };
iC_Gt QX1_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.5_1", &iC_l_[219], &iC_l_[221], &QX1_5_0 };
iC_Gt QX1_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.5_2", &iC_l_[225], &iC_l_[227], &QX1_5_1 };
iC_Gt QX1_6    = { 1, -iC_FF, iC_GATE, 0, "QX1.6", 0, 0, &QX1_5_2 };
iC_Gt QX1_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.6_0", 0, &iC_l_[231], &QX1_6 };
iC_Gt QX1_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.6_1", &iC_l_[234], &iC_l_[236], &QX1_6_0 };
iC_Gt QX1_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX1.6_2", &iC_l_[240], &iC_l_[242], &QX1_6_1 };
iC_Gt QX1_7    = { 1, -iC_FF, iC_GATE, 0, "QX1.7", 0, 0, &QX1_6_2 };
iC_Gt QX1_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX1.7_0", 0, &iC_l_[246], &QX1_7 };
iC_Gt QX1_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX1.7_1", &iC_l_[249], &iC_l_[251], &QX1_7_0 };
iC_Gt QX1_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX1.7_2", &iC_l_[255], &iC_l_[257], &QX1_7_1 };
iC_Gt QX2_0    = { 1, -iC_FF, iC_GATE, 0, "QX2.0", 0, 0, &QX1_7_2 };
iC_Gt QX2_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.0_0", 0, &iC_l_[260], &QX2_0 };
iC_Gt QX2_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX2.0_1", &iC_l_[263], &iC_l_[265], &QX2_0_0 };
iC_Gt QX2_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.0_2", &iC_l_[268], &iC_l_[270], &QX2_0_1 };
iC_Gt QX2_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX2.0_3", 0, 0, &QX2_0_2 };
iC_Gt QX2_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX2.0_4", &iC_l_[274], &iC_l_[276], &QX2_0_3 };
iC_Gt QX2_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX2.0_5", &iC_l_[280], &iC_l_[282], &QX2_0_4 };
iC_Gt QX2_1    = { 1, -iC_FF, iC_GATE, 0, "QX2.1", 0, 0, &QX2_0_5 };
iC_Gt QX2_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.1_0", 0, &iC_l_[286], &QX2_1 };
iC_Gt QX2_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.1_1", &iC_l_[289], &iC_l_[291], &QX2_1_0 };
iC_Gt QX2_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.1_2", &iC_l_[295], &iC_l_[297], &QX2_1_1 };
iC_Gt QX2_2    = { 1, -iC_FF, iC_GATE, 0, "QX2.2", 0, 0, &QX2_1_2 };
iC_Gt QX2_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.2_0", 0, &iC_l_[301], &QX2_2 };
iC_Gt QX2_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.2_1", &iC_l_[304], &iC_l_[306], &QX2_2_0 };
iC_Gt QX2_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.2_2", &iC_l_[310], &iC_l_[312], &QX2_2_1 };
iC_Gt QX2_3    = { 1, -iC_FF, iC_GATE, 0, "QX2.3", 0, 0, &QX2_2_2 };
iC_Gt QX2_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.3_0", 0, &iC_l_[316], &QX2_3 };
iC_Gt QX2_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.3_1", &iC_l_[319], &iC_l_[321], &QX2_3_0 };
iC_Gt QX2_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.3_2", &iC_l_[325], &iC_l_[327], &QX2_3_1 };
iC_Gt QX2_4    = { 1, -iC_FF, iC_GATE, 0, "QX2.4", 0, 0, &QX2_3_2 };
iC_Gt QX2_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.4_0", 0, &iC_l_[331], &QX2_4 };
iC_Gt QX2_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.4_1", &iC_l_[334], &iC_l_[336], &QX2_4_0 };
iC_Gt QX2_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.4_2", &iC_l_[340], &iC_l_[342], &QX2_4_1 };
iC_Gt QX2_5    = { 1, -iC_FF, iC_GATE, 0, "QX2.5", 0, 0, &QX2_4_2 };
iC_Gt QX2_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.5_0", 0, &iC_l_[346], &QX2_5 };
iC_Gt QX2_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.5_1", &iC_l_[349], &iC_l_[351], &QX2_5_0 };
iC_Gt QX2_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.5_2", &iC_l_[355], &iC_l_[357], &QX2_5_1 };
iC_Gt QX2_6    = { 1, -iC_FF, iC_GATE, 0, "QX2.6", 0, 0, &QX2_5_2 };
iC_Gt QX2_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.6_0", 0, &iC_l_[361], &QX2_6 };
iC_Gt QX2_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.6_1", &iC_l_[364], &iC_l_[366], &QX2_6_0 };
iC_Gt QX2_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX2.6_2", &iC_l_[370], &iC_l_[372], &QX2_6_1 };
iC_Gt QX2_7    = { 1, -iC_FF, iC_GATE, 0, "QX2.7", 0, 0, &QX2_6_2 };
iC_Gt QX2_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX2.7_0", 0, &iC_l_[376], &QX2_7 };
iC_Gt QX2_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX2.7_1", &iC_l_[379], &iC_l_[381], &QX2_7_0 };
iC_Gt QX2_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX2.7_2", &iC_l_[385], &iC_l_[387], &QX2_7_1 };
iC_Gt QX3_0    = { 1, -iC_FF, iC_GATE, 0, "QX3.0", 0, 0, &QX2_7_2 };
iC_Gt QX3_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.0_0", 0, &iC_l_[390], &QX3_0 };
iC_Gt QX3_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX3.0_1", &iC_l_[393], &iC_l_[395], &QX3_0_0 };
iC_Gt QX3_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.0_2", &iC_l_[398], &iC_l_[400], &QX3_0_1 };
iC_Gt QX3_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX3.0_3", 0, 0, &QX3_0_2 };
iC_Gt QX3_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX3.0_4", &iC_l_[404], &iC_l_[406], &QX3_0_3 };
iC_Gt QX3_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX3.0_5", &iC_l_[410], &iC_l_[412], &QX3_0_4 };
iC_Gt QX3_1    = { 1, -iC_FF, iC_GATE, 0, "QX3.1", 0, 0, &QX3_0_5 };
iC_Gt QX3_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.1_0", 0, &iC_l_[416], &QX3_1 };
iC_Gt QX3_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.1_1", &iC_l_[419], &iC_l_[421], &QX3_1_0 };
iC_Gt QX3_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.1_2", &iC_l_[425], &iC_l_[427], &QX3_1_1 };
iC_Gt QX3_2    = { 1, -iC_FF, iC_GATE, 0, "QX3.2", 0, 0, &QX3_1_2 };
iC_Gt QX3_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.2_0", 0, &iC_l_[431], &QX3_2 };
iC_Gt QX3_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.2_1", &iC_l_[434], &iC_l_[436], &QX3_2_0 };
iC_Gt QX3_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.2_2", &iC_l_[440], &iC_l_[442], &QX3_2_1 };
iC_Gt QX3_3    = { 1, -iC_FF, iC_GATE, 0, "QX3.3", 0, 0, &QX3_2_2 };
iC_Gt QX3_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.3_0", 0, &iC_l_[446], &QX3_3 };
iC_Gt QX3_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.3_1", &iC_l_[449], &iC_l_[451], &QX3_3_0 };
iC_Gt QX3_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.3_2", &iC_l_[455], &iC_l_[457], &QX3_3_1 };
iC_Gt QX3_4    = { 1, -iC_FF, iC_GATE, 0, "QX3.4", 0, 0, &QX3_3_2 };
iC_Gt QX3_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.4_0", 0, &iC_l_[461], &QX3_4 };
iC_Gt QX3_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.4_1", &iC_l_[464], &iC_l_[466], &QX3_4_0 };
iC_Gt QX3_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.4_2", &iC_l_[470], &iC_l_[472], &QX3_4_1 };
iC_Gt QX3_5    = { 1, -iC_FF, iC_GATE, 0, "QX3.5", 0, 0, &QX3_4_2 };
iC_Gt QX3_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.5_0", 0, &iC_l_[476], &QX3_5 };
iC_Gt QX3_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.5_1", &iC_l_[479], &iC_l_[481], &QX3_5_0 };
iC_Gt QX3_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.5_2", &iC_l_[485], &iC_l_[487], &QX3_5_1 };
iC_Gt QX3_6    = { 1, -iC_FF, iC_GATE, 0, "QX3.6", 0, 0, &QX3_5_2 };
iC_Gt QX3_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.6_0", 0, &iC_l_[491], &QX3_6 };
iC_Gt QX3_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.6_1", &iC_l_[494], &iC_l_[496], &QX3_6_0 };
iC_Gt QX3_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX3.6_2", &iC_l_[500], &iC_l_[502], &QX3_6_1 };
iC_Gt QX3_7    = { 1, -iC_FF, iC_GATE, 0, "QX3.7", 0, 0, &QX3_6_2 };
iC_Gt QX3_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX3.7_0", 0, &iC_l_[506], &QX3_7 };
iC_Gt QX3_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX3.7_1", &iC_l_[509], &iC_l_[511], &QX3_7_0 };
iC_Gt QX3_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX3.7_2", &iC_l_[515], &iC_l_[517], &QX3_7_1 };
iC_Gt QX4_0    = { 1, -iC_FF, iC_GATE, 0, "QX4.0", 0, 0, &QX3_7_2 };
iC_Gt QX4_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.0_0", 0, &iC_l_[520], &QX4_0 };
iC_Gt QX4_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX4.0_1", &iC_l_[523], &iC_l_[525], &QX4_0_0 };
iC_Gt QX4_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.0_2", &iC_l_[528], &iC_l_[530], &QX4_0_1 };
iC_Gt QX4_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX4.0_3", 0, 0, &QX4_0_2 };
iC_Gt QX4_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX4.0_4", &iC_l_[534], &iC_l_[536], &QX4_0_3 };
iC_Gt QX4_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX4.0_5", &iC_l_[540], &iC_l_[542], &QX4_0_4 };
iC_Gt QX4_1    = { 1, -iC_FF, iC_GATE, 0, "QX4.1", 0, 0, &QX4_0_5 };
iC_Gt QX4_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.1_0", 0, &iC_l_[546], &QX4_1 };
iC_Gt QX4_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.1_1", &iC_l_[549], &iC_l_[551], &QX4_1_0 };
iC_Gt QX4_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.1_2", &iC_l_[555], &iC_l_[557], &QX4_1_1 };
iC_Gt QX4_2    = { 1, -iC_FF, iC_GATE, 0, "QX4.2", 0, 0, &QX4_1_2 };
iC_Gt QX4_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.2_0", 0, &iC_l_[561], &QX4_2 };
iC_Gt QX4_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.2_1", &iC_l_[564], &iC_l_[566], &QX4_2_0 };
iC_Gt QX4_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.2_2", &iC_l_[570], &iC_l_[572], &QX4_2_1 };
iC_Gt QX4_3    = { 1, -iC_FF, iC_GATE, 0, "QX4.3", 0, 0, &QX4_2_2 };
iC_Gt QX4_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.3_0", 0, &iC_l_[576], &QX4_3 };
iC_Gt QX4_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.3_1", &iC_l_[579], &iC_l_[581], &QX4_3_0 };
iC_Gt QX4_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.3_2", &iC_l_[585], &iC_l_[587], &QX4_3_1 };
iC_Gt QX4_4    = { 1, -iC_FF, iC_GATE, 0, "QX4.4", 0, 0, &QX4_3_2 };
iC_Gt QX4_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.4_0", 0, &iC_l_[591], &QX4_4 };
iC_Gt QX4_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.4_1", &iC_l_[594], &iC_l_[596], &QX4_4_0 };
iC_Gt QX4_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.4_2", &iC_l_[600], &iC_l_[602], &QX4_4_1 };
iC_Gt QX4_5    = { 1, -iC_FF, iC_GATE, 0, "QX4.5", 0, 0, &QX4_4_2 };
iC_Gt QX4_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.5_0", 0, &iC_l_[606], &QX4_5 };
iC_Gt QX4_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.5_1", &iC_l_[609], &iC_l_[611], &QX4_5_0 };
iC_Gt QX4_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.5_2", &iC_l_[615], &iC_l_[617], &QX4_5_1 };
iC_Gt QX4_6    = { 1, -iC_FF, iC_GATE, 0, "QX4.6", 0, 0, &QX4_5_2 };
iC_Gt QX4_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.6_0", 0, &iC_l_[621], &QX4_6 };
iC_Gt QX4_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.6_1", &iC_l_[624], &iC_l_[626], &QX4_6_0 };
iC_Gt QX4_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX4.6_2", &iC_l_[630], &iC_l_[632], &QX4_6_1 };
iC_Gt QX4_7    = { 1, -iC_FF, iC_GATE, 0, "QX4.7", 0, 0, &QX4_6_2 };
iC_Gt QX4_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX4.7_0", 0, &iC_l_[636], &QX4_7 };
iC_Gt QX4_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX4.7_1", &iC_l_[639], &iC_l_[641], &QX4_7_0 };
iC_Gt QX4_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX4.7_2", &iC_l_[645], &iC_l_[647], &QX4_7_1 };
iC_Gt QX5_0    = { 1, -iC_FF, iC_GATE, 0, "QX5.0", 0, 0, &QX4_7_2 };
iC_Gt QX5_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.0_0", 0, &iC_l_[650], &QX5_0 };
iC_Gt QX5_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX5.0_1", &iC_l_[653], &iC_l_[655], &QX5_0_0 };
iC_Gt QX5_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.0_2", &iC_l_[658], &iC_l_[660], &QX5_0_1 };
iC_Gt QX5_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX5.0_3", 0, 0, &QX5_0_2 };
iC_Gt QX5_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX5.0_4", &iC_l_[664], &iC_l_[666], &QX5_0_3 };
iC_Gt QX5_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX5.0_5", &iC_l_[670], &iC_l_[672], &QX5_0_4 };
iC_Gt QX5_1    = { 1, -iC_FF, iC_GATE, 0, "QX5.1", 0, 0, &QX5_0_5 };
iC_Gt QX5_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.1_0", 0, &iC_l_[676], &QX5_1 };
iC_Gt QX5_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.1_1", &iC_l_[679], &iC_l_[681], &QX5_1_0 };
iC_Gt QX5_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.1_2", &iC_l_[685], &iC_l_[687], &QX5_1_1 };
iC_Gt QX5_2    = { 1, -iC_FF, iC_GATE, 0, "QX5.2", 0, 0, &QX5_1_2 };
iC_Gt QX5_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.2_0", 0, &iC_l_[691], &QX5_2 };
iC_Gt QX5_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.2_1", &iC_l_[694], &iC_l_[696], &QX5_2_0 };
iC_Gt QX5_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.2_2", &iC_l_[700], &iC_l_[702], &QX5_2_1 };
iC_Gt QX5_3    = { 1, -iC_FF, iC_GATE, 0, "QX5.3", 0, 0, &QX5_2_2 };
iC_Gt QX5_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.3_0", 0, &iC_l_[706], &QX5_3 };
iC_Gt QX5_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.3_1", &iC_l_[709], &iC_l_[711], &QX5_3_0 };
iC_Gt QX5_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.3_2", &iC_l_[715], &iC_l_[717], &QX5_3_1 };
iC_Gt QX5_4    = { 1, -iC_FF, iC_GATE, 0, "QX5.4", 0, 0, &QX5_3_2 };
iC_Gt QX5_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.4_0", 0, &iC_l_[721], &QX5_4 };
iC_Gt QX5_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.4_1", &iC_l_[724], &iC_l_[726], &QX5_4_0 };
iC_Gt QX5_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.4_2", &iC_l_[730], &iC_l_[732], &QX5_4_1 };
iC_Gt QX5_5    = { 1, -iC_FF, iC_GATE, 0, "QX5.5", 0, 0, &QX5_4_2 };
iC_Gt QX5_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.5_0", 0, &iC_l_[736], &QX5_5 };
iC_Gt QX5_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.5_1", &iC_l_[739], &iC_l_[741], &QX5_5_0 };
iC_Gt QX5_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.5_2", &iC_l_[745], &iC_l_[747], &QX5_5_1 };
iC_Gt QX5_6    = { 1, -iC_FF, iC_GATE, 0, "QX5.6", 0, 0, &QX5_5_2 };
iC_Gt QX5_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.6_0", 0, &iC_l_[751], &QX5_6 };
iC_Gt QX5_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.6_1", &iC_l_[754], &iC_l_[756], &QX5_6_0 };
iC_Gt QX5_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX5.6_2", &iC_l_[760], &iC_l_[762], &QX5_6_1 };
iC_Gt QX5_7    = { 1, -iC_FF, iC_GATE, 0, "QX5.7", 0, 0, &QX5_6_2 };
iC_Gt QX5_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX5.7_0", 0, &iC_l_[766], &QX5_7 };
iC_Gt QX5_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX5.7_1", &iC_l_[769], &iC_l_[771], &QX5_7_0 };
iC_Gt QX5_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX5.7_2", &iC_l_[775], &iC_l_[777], &QX5_7_1 };
iC_Gt QX6_0    = { 1, -iC_FF, iC_GATE, 0, "QX6.0", 0, 0, &QX5_7_2 };
iC_Gt QX6_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.0_0", 0, &iC_l_[780], &QX6_0 };
iC_Gt QX6_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX6.0_1", &iC_l_[783], &iC_l_[785], &QX6_0_0 };
iC_Gt QX6_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.0_2", &iC_l_[788], &iC_l_[790], &QX6_0_1 };
iC_Gt QX6_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX6.0_3", 0, 0, &QX6_0_2 };
iC_Gt QX6_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX6.0_4", &iC_l_[794], &iC_l_[796], &QX6_0_3 };
iC_Gt QX6_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX6.0_5", &iC_l_[800], &iC_l_[802], &QX6_0_4 };
iC_Gt QX6_1    = { 1, -iC_FF, iC_GATE, 0, "QX6.1", 0, 0, &QX6_0_5 };
iC_Gt QX6_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.1_0", 0, &iC_l_[806], &QX6_1 };
iC_Gt QX6_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.1_1", &iC_l_[809], &iC_l_[811], &QX6_1_0 };
iC_Gt QX6_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.1_2", &iC_l_[815], &iC_l_[817], &QX6_1_1 };
iC_Gt QX6_2    = { 1, -iC_FF, iC_GATE, 0, "QX6.2", 0, 0, &QX6_1_2 };
iC_Gt QX6_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.2_0", 0, &iC_l_[821], &QX6_2 };
iC_Gt QX6_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.2_1", &iC_l_[824], &iC_l_[826], &QX6_2_0 };
iC_Gt QX6_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.2_2", &iC_l_[830], &iC_l_[832], &QX6_2_1 };
iC_Gt QX6_3    = { 1, -iC_FF, iC_GATE, 0, "QX6.3", 0, 0, &QX6_2_2 };
iC_Gt QX6_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.3_0", 0, &iC_l_[836], &QX6_3 };
iC_Gt QX6_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.3_1", &iC_l_[839], &iC_l_[841], &QX6_3_0 };
iC_Gt QX6_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.3_2", &iC_l_[845], &iC_l_[847], &QX6_3_1 };
iC_Gt QX6_4    = { 1, -iC_FF, iC_GATE, 0, "QX6.4", 0, 0, &QX6_3_2 };
iC_Gt QX6_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.4_0", 0, &iC_l_[851], &QX6_4 };
iC_Gt QX6_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.4_1", &iC_l_[854], &iC_l_[856], &QX6_4_0 };
iC_Gt QX6_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.4_2", &iC_l_[860], &iC_l_[862], &QX6_4_1 };
iC_Gt QX6_5    = { 1, -iC_FF, iC_GATE, 0, "QX6.5", 0, 0, &QX6_4_2 };
iC_Gt QX6_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.5_0", 0, &iC_l_[866], &QX6_5 };
iC_Gt QX6_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.5_1", &iC_l_[869], &iC_l_[871], &QX6_5_0 };
iC_Gt QX6_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.5_2", &iC_l_[875], &iC_l_[877], &QX6_5_1 };
iC_Gt QX6_6    = { 1, -iC_FF, iC_GATE, 0, "QX6.6", 0, 0, &QX6_5_2 };
iC_Gt QX6_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.6_0", 0, &iC_l_[881], &QX6_6 };
iC_Gt QX6_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.6_1", &iC_l_[884], &iC_l_[886], &QX6_6_0 };
iC_Gt QX6_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX6.6_2", &iC_l_[890], &iC_l_[892], &QX6_6_1 };
iC_Gt QX6_7    = { 1, -iC_FF, iC_GATE, 0, "QX6.7", 0, 0, &QX6_6_2 };
iC_Gt QX6_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX6.7_0", 0, &iC_l_[896], &QX6_7 };
iC_Gt QX6_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX6.7_1", &iC_l_[899], &iC_l_[901], &QX6_7_0 };
iC_Gt QX6_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX6.7_2", &iC_l_[905], &iC_l_[907], &QX6_7_1 };
iC_Gt QX7_0    = { 1, -iC_FF, iC_GATE, 0, "QX7.0", 0, 0, &QX6_7_2 };
iC_Gt QX7_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.0_0", 0, &iC_l_[910], &QX7_0 };
iC_Gt QX7_0_1  = { 1, -iC_OR, iC_S_FF, 0, "QX7.0_1", &iC_l_[913], &iC_l_[915], &QX7_0_0 };
iC_Gt QX7_0_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.0_2", &iC_l_[918], &iC_l_[920], &QX7_0_1 };
iC_Gt QX7_0_3  = { 1, -iC_FF, iC_GATE, 0, "QX7.0_3", 0, 0, &QX7_0_2 };
iC_Gt QX7_0_4  = { 1, -iC_AND, iC_S_FF, 0, "QX7.0_4", &iC_l_[924], &iC_l_[926], &QX7_0_3 };
iC_Gt QX7_0_5  = { 1, -iC_AND, iC_R_FF, 0, "QX7.0_5", &iC_l_[930], &iC_l_[932], &QX7_0_4 };
iC_Gt QX7_1    = { 1, -iC_FF, iC_GATE, 0, "QX7.1", 0, 0, &QX7_0_5 };
iC_Gt QX7_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.1_0", 0, &iC_l_[936], &QX7_1 };
iC_Gt QX7_1_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.1_1", &iC_l_[939], &iC_l_[941], &QX7_1_0 };
iC_Gt QX7_1_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.1_2", &iC_l_[945], &iC_l_[947], &QX7_1_1 };
iC_Gt QX7_2    = { 1, -iC_FF, iC_GATE, 0, "QX7.2", 0, 0, &QX7_1_2 };
iC_Gt QX7_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.2_0", 0, &iC_l_[951], &QX7_2 };
iC_Gt QX7_2_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.2_1", &iC_l_[954], &iC_l_[956], &QX7_2_0 };
iC_Gt QX7_2_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.2_2", &iC_l_[960], &iC_l_[962], &QX7_2_1 };
iC_Gt QX7_3    = { 1, -iC_FF, iC_GATE, 0, "QX7.3", 0, 0, &QX7_2_2 };
iC_Gt QX7_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.3_0", 0, &iC_l_[966], &QX7_3 };
iC_Gt QX7_3_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.3_1", &iC_l_[969], &iC_l_[971], &QX7_3_0 };
iC_Gt QX7_3_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.3_2", &iC_l_[975], &iC_l_[977], &QX7_3_1 };
iC_Gt QX7_4    = { 1, -iC_FF, iC_GATE, 0, "QX7.4", 0, 0, &QX7_3_2 };
iC_Gt QX7_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.4_0", 0, &iC_l_[981], &QX7_4 };
iC_Gt QX7_4_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.4_1", &iC_l_[984], &iC_l_[986], &QX7_4_0 };
iC_Gt QX7_4_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.4_2", &iC_l_[990], &iC_l_[992], &QX7_4_1 };
iC_Gt QX7_5    = { 1, -iC_FF, iC_GATE, 0, "QX7.5", 0, 0, &QX7_4_2 };
iC_Gt QX7_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.5_0", 0, &iC_l_[996], &QX7_5 };
iC_Gt QX7_5_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.5_1", &iC_l_[999], &iC_l_[1001], &QX7_5_0 };
iC_Gt QX7_5_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.5_2", &iC_l_[1005], &iC_l_[1007], &QX7_5_1 };
iC_Gt QX7_6    = { 1, -iC_FF, iC_GATE, 0, "QX7.6", 0, 0, &QX7_5_2 };
iC_Gt QX7_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.6_0", 0, &iC_l_[1011], &QX7_6 };
iC_Gt QX7_6_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.6_1", &iC_l_[1014], &iC_l_[1016], &QX7_6_0 };
iC_Gt QX7_6_2  = { 1, -iC_AND, iC_R_FF, 0, "QX7.6_2", &iC_l_[1020], &iC_l_[1022], &QX7_6_1 };
iC_Gt QX7_7    = { 1, -iC_FF, iC_GATE, 0, "QX7.7", 0, 0, &QX7_6_2 };
iC_Gt QX7_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX7.7_0", 0, &iC_l_[1026], &QX7_7 };
iC_Gt QX7_7_1  = { 1, -iC_AND, iC_S_FF, 0, "QX7.7_1", &iC_l_[1029], &iC_l_[1031], &QX7_7_0 };
iC_Gt QX7_7_2  = { 1, -iC_OR, iC_R_FF, 0, "QX7.7_2", &iC_l_[1035], &iC_l_[1037], &QX7_7_1 };
iC_Gt QX9_0_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.0_0", 0, &iC_l_[1040], &QX7_7_2 };
iC_Gt QX9_1_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.1_0", 0, &iC_l_[1043], &QX9_0_0 };
iC_Gt QX9_2_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.2_0", 0, &iC_l_[1046], &QX9_1_0 };
iC_Gt QX9_3_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.3_0", 0, &iC_l_[1049], &QX9_2_0 };
iC_Gt QX9_4_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.4_0", 0, &iC_l_[1052], &QX9_3_0 };
iC_Gt QX9_5_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.5_0", 0, &iC_l_[1055], &QX9_4_0 };
iC_Gt QX9_6_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.6_0", 0, &iC_l_[1058], &QX9_5_0 };
iC_Gt QX9_7_0  = { 1, -iC_OR, iC_OUTX, 0, "QX9.7_0", 0, &iC_l_[1061], &QX9_6_0 };
iC_Gt TX0_1    = { 1, -iC_INPX, iC_GATE, 0, "TX0.1", 0, 0, &QX9_7_0 };
iC_Gt TX0_2    = { 1, -iC_INPX, iC_GATE, 0, "TX0.2", 0, 0, &TX0_1 };
iC_Gt TX0_3    = { 1, -iC_INPX, iC_GATE, 0, "TX0.3", 0, 0, &TX0_2 };
iC_Gt TX0_4    = { 1, -iC_INPX, iC_GATE, 0, "TX0.4", 0, 0, &TX0_3 };
iC_Gt TX0_5    = { 1, -iC_INPX, iC_GATE, 0, "TX0.5", 0, 0, &TX0_4 };
iC_Gt TX0_6    = { 1, -iC_INPX, iC_GATE, 0, "TX0.6", 0, 0, &TX0_5 };
iC_Gt TX0_7    = { 1, -iC_INPX, iC_GATE, 0, "TX0.7", 0, 0, &TX0_6 };
iC_Gt b0       = { 1, -iC_OR, iC_GATE, 0, "b0", 0, &iC_l_[1064], &TX0_7 };
iC_Gt b0_1     = { 1, -iC_FF, iC_GATE, 0, "b0_1", 0, 0, &b0 };
iC_Gt b0_2     = { 1, -iC_VF, iC_GATE, 0, "b0_2", 0, 0, &b0_1 };
iC_Gt b0_3     = { 1, -iC_AND, iC_CH_BIT, 0, "b0_3", &iC_l_[1068], &iC_l_[1070], &b0_2 };
iC_Gt b0_4     = { 1, -iC_AND, iC_D_FF, 0, "b0_4", &iC_l_[1074], &iC_l_[1077], &b0_3 };
iC_Gt base     = { 1, -iC_OR, iC_GATE, 0, "base", 0, &iC_l_[1081], &b0_4 };
iC_Gt base_1   = { 1, -iC_AND, iC_GATE, 0, "base_1", 0, &iC_l_[1090], &base };
iC_Gt base_2   = { 1, -iC_AND, iC_GATE, 0, "base_2", 0, &iC_l_[1099], &base_1 };
iC_Gt base_3   = { 1, -iC_AND, iC_GATE, 0, "base_3", 0, &iC_l_[1108], &base_2 };
iC_Gt base_4   = { 1, -iC_AND, iC_GATE, 0, "base_4", 0, &iC_l_[1116], &base_3 };
iC_Gt base_5   = { 1, -iC_AND, iC_GATE, 0, "base_5", 0, &iC_l_[1123], &base_4 };
iC_Gt base_6   = { 1, -iC_AND, iC_GATE, 0, "base_6", 0, &iC_l_[1129], &base_5 };
iC_Gt base_7   = { 1, -iC_AND, iC_GATE, 0, "base_7", 0, &iC_l_[1134], &base_6 };
iC_Gt c0       = { 1, -iC_CLK, iC_CLCKL, 0, "c0", 0, 0, &base_7 };
iC_Gt c0_1     = { 1, -iC_OR, iC_CLCK, 0, "c0_1", &iC_l_[1138], &iC_l_[1140], &c0 };
iC_Gt f0       = { 1, -iC_ARN, iC_GATE, 0, "f0", 0, &iC_l_[1143], &c0_1 };
iC_Gt t        = { 1, -iC_TIM, iC_TIMRL, 0, "t", 0, 0, &f0 };
iC_Gt t_1      = { 1, -iC_OR, iC_TIMR, 0, "t_1", &iC_l_[1146], &iC_l_[1148], &t };
iC_Gt t_2      = { 1, -iC_OR, iC_TIMR, 0, "t_2", &iC_l_[1151], &iC_l_[1153], &t_1 };
iC_Gt QX9_0    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.0", 0, (iC_Gt**)&QX0_0_3, &t_2, 0 };
iC_Gt QX9_1    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.1", 0, (iC_Gt**)&QX1_0_3, &QX9_0, 0 };
iC_Gt QX9_2    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.2", 0, (iC_Gt**)&QX2_0_3, &QX9_1, 0 };
iC_Gt QX9_3    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.3", 0, (iC_Gt**)&QX3_0_3, &QX9_2, 0 };
iC_Gt QX9_4    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.4", 0, (iC_Gt**)&QX4_0_3, &QX9_3, 0 };
iC_Gt QX9_5    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.5", 0, (iC_Gt**)&QX5_0_3, &QX9_4, 0 };
iC_Gt QX9_6    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.6", 0, (iC_Gt**)&QX6_0_3, &QX9_5, 0 };
iC_Gt QX9_7    = { 1, -iC_ALIAS, iC_GATE, 0, "QX9.7", 0, (iC_Gt**)&QX7_0_3, &QX9_6, 0 };
iC_Gt d0       = { 1, -iC_ALIAS, iC_ARITH, 0, "d0", 0, (iC_Gt**)&IB8, &QX9_7, 0 };

iC_Gt *		iC___Test0_barfy_list = &d0;
iC_Gt **	iC_list[] = { &iC___Test0_barfy_list, 0, };

/********************************************************************
 *
 *	Connection lists
 *
 *******************************************************************/

static iC_Gt *	iC_l_[] = {
/* QX0.0_0 */	&QX0_0, 0, 0,
/* QX0.0_1 */	&QX0_0, &c0,	0, &QX0_0_3, 0,
/* QX0.0_2 */	&QX0_0, &c0,	&QX0_0_3, 0, &QX0_1, 0,
/* QX0.0_4 */	&QX0_0_3, &c0,	&QX0_6, 0, &QX0_7, 0,
/* QX0.0_5 */	&QX0_0_3, &c0,	&QX0_0, 0, &QX0_1, 0,
/* QX0.1_0 */	&QX0_1, 0, 0,
/* QX0.1_1 */	&QX0_1, &c0,	&QX0_0, 0, &QX0_0_3, 0,
/* QX0.1_2 */	&QX0_1, &c0,	&QX0_0_3, 0, &QX0_2, 0,
/* QX0.2_0 */	&QX0_2, 0, 0,
/* QX0.2_1 */	&QX0_2, &c0,	&QX0_1, 0, &QX0_0_3, 0,
/* QX0.2_2 */	&QX0_2, &c0,	&QX0_0_3, 0, &QX0_3, 0,
/* QX0.3_0 */	&QX0_3, 0, 0,
/* QX0.3_1 */	&QX0_3, &c0,	&QX0_2, 0, &QX0_0_3, 0,
/* QX0.3_2 */	&QX0_3, &c0,	&QX0_0_3, 0, &QX0_4, 0,
/* QX0.4_0 */	&QX0_4, 0, 0,
/* QX0.4_1 */	&QX0_4, &c0,	&QX0_3, 0, &QX0_0_3, 0,
/* QX0.4_2 */	&QX0_4, &c0,	&QX0_0_3, 0, &QX0_5, 0,
/* QX0.5_0 */	&QX0_5, 0, 0,
/* QX0.5_1 */	&QX0_5, &c0,	&QX0_4, 0, &QX0_0_3, 0,
/* QX0.5_2 */	&QX0_5, &c0,	&QX0_0_3, 0, &QX0_6, 0,
/* QX0.6_0 */	&QX0_6, 0, 0,
/* QX0.6_1 */	&QX0_6, &c0,	&QX0_5, 0, &QX0_0_3, 0,
/* QX0.6_2 */	&QX0_6, &c0,	&QX0_0_3, 0, &QX0_7, 0,
/* QX0.7_0 */	&QX0_7, 0, 0,
/* QX0.7_1 */	&QX0_7, &c0,	&QX0_6, 0, &QX0_0_3, 0,
/* QX0.7_2 */	&QX0_7, &c0,	&QX0_0_3, 0, 0,
/* QX1.0_0 */	&QX1_0, 0, 0,
/* QX1.0_1 */	&QX1_0, &c0,	0, &QX1_0_3, 0,
/* QX1.0_2 */	&QX1_0, &c0,	&QX1_0_3, 0, &QX1_1, 0,
/* QX1.0_4 */	&QX1_0_3, &c0,	&QX1_6, 0, &QX1_7, 0,
/* QX1.0_5 */	&QX1_0_3, &c0,	&QX1_0, 0, &QX1_1, 0,
/* QX1.1_0 */	&QX1_1, 0, 0,
/* QX1.1_1 */	&QX1_1, &c0,	&QX1_0, 0, &QX1_0_3, 0,
/* QX1.1_2 */	&QX1_1, &c0,	&QX1_0_3, 0, &QX1_2, 0,
/* QX1.2_0 */	&QX1_2, 0, 0,
/* QX1.2_1 */	&QX1_2, &c0,	&QX1_1, 0, &QX1_0_3, 0,
/* QX1.2_2 */	&QX1_2, &c0,	&QX1_0_3, 0, &QX1_3, 0,
/* QX1.3_0 */	&QX1_3, 0, 0,
/* QX1.3_1 */	&QX1_3, &c0,	&QX1_2, 0, &QX1_0_3, 0,
/* QX1.3_2 */	&QX1_3, &c0,	&QX1_0_3, 0, &QX1_4, 0,
/* QX1.4_0 */	&QX1_4, 0, 0,
/* QX1.4_1 */	&QX1_4, &c0,	&QX1_3, 0, &QX1_0_3, 0,
/* QX1.4_2 */	&QX1_4, &c0,	&QX1_0_3, 0, &QX1_5, 0,
/* QX1.5_0 */	&QX1_5, 0, 0,
/* QX1.5_1 */	&QX1_5, &c0,	&QX1_4, 0, &QX1_0_3, 0,
/* QX1.5_2 */	&QX1_5, &c0,	&QX1_0_3, 0, &QX1_6, 0,
/* QX1.6_0 */	&QX1_6, 0, 0,
/* QX1.6_1 */	&QX1_6, &c0,	&QX1_5, 0, &QX1_0_3, 0,
/* QX1.6_2 */	&QX1_6, &c0,	&QX1_0_3, 0, &QX1_7, 0,
/* QX1.7_0 */	&QX1_7, 0, 0,
/* QX1.7_1 */	&QX1_7, &c0,	&QX1_6, 0, &QX1_0_3, 0,
/* QX1.7_2 */	&QX1_7, &c0,	&QX1_0_3, 0, 0,
/* QX2.0_0 */	&QX2_0, 0, 0,
/* QX2.0_1 */	&QX2_0, &c0,	0, &QX2_0_3, 0,
/* QX2.0_2 */	&QX2_0, &c0,	&QX2_0_3, 0, &QX2_1, 0,
/* QX2.0_4 */	&QX2_0_3, &c0,	&QX2_6, 0, &QX2_7, 0,
/* QX2.0_5 */	&QX2_0_3, &c0,	&QX2_0, 0, &QX2_1, 0,
/* QX2.1_0 */	&QX2_1, 0, 0,
/* QX2.1_1 */	&QX2_1, &c0,	&QX2_0, 0, &QX2_0_3, 0,
/* QX2.1_2 */	&QX2_1, &c0,	&QX2_0_3, 0, &QX2_2, 0,
/* QX2.2_0 */	&QX2_2, 0, 0,
/* QX2.2_1 */	&QX2_2, &c0,	&QX2_1, 0, &QX2_0_3, 0,
/* QX2.2_2 */	&QX2_2, &c0,	&QX2_0_3, 0, &QX2_3, 0,
/* QX2.3_0 */	&QX2_3, 0, 0,
/* QX2.3_1 */	&QX2_3, &c0,	&QX2_2, 0, &QX2_0_3, 0,
/* QX2.3_2 */	&QX2_3, &c0,	&QX2_0_3, 0, &QX2_4, 0,
/* QX2.4_0 */	&QX2_4, 0, 0,
/* QX2.4_1 */	&QX2_4, &c0,	&QX2_3, 0, &QX2_0_3, 0,
/* QX2.4_2 */	&QX2_4, &c0,	&QX2_0_3, 0, &QX2_5, 0,
/* QX2.5_0 */	&QX2_5, 0, 0,
/* QX2.5_1 */	&QX2_5, &c0,	&QX2_4, 0, &QX2_0_3, 0,
/* QX2.5_2 */	&QX2_5, &c0,	&QX2_0_3, 0, &QX2_6, 0,
/* QX2.6_0 */	&QX2_6, 0, 0,
/* QX2.6_1 */	&QX2_6, &c0,	&QX2_5, 0, &QX2_0_3, 0,
/* QX2.6_2 */	&QX2_6, &c0,	&QX2_0_3, 0, &QX2_7, 0,
/* QX2.7_0 */	&QX2_7, 0, 0,
/* QX2.7_1 */	&QX2_7, &c0,	&QX2_6, 0, &QX2_0_3, 0,
/* QX2.7_2 */	&QX2_7, &c0,	&QX2_0_3, 0, 0,
/* QX3.0_0 */	&QX3_0, 0, 0,
/* QX3.0_1 */	&QX3_0, &c0,	0, &QX3_0_3, 0,
/* QX3.0_2 */	&QX3_0, &c0,	&QX3_0_3, 0, &QX3_1, 0,
/* QX3.0_4 */	&QX3_0_3, &c0,	&QX3_6, 0, &QX3_7, 0,
/* QX3.0_5 */	&QX3_0_3, &c0,	&QX3_0, 0, &QX3_1, 0,
/* QX3.1_0 */	&QX3_1, 0, 0,
/* QX3.1_1 */	&QX3_1, &c0,	&QX3_0, 0, &QX3_0_3, 0,
/* QX3.1_2 */	&QX3_1, &c0,	&QX3_0_3, 0, &QX3_2, 0,
/* QX3.2_0 */	&QX3_2, 0, 0,
/* QX3.2_1 */	&QX3_2, &c0,	&QX3_1, 0, &QX3_0_3, 0,
/* QX3.2_2 */	&QX3_2, &c0,	&QX3_0_3, 0, &QX3_3, 0,
/* QX3.3_0 */	&QX3_3, 0, 0,
/* QX3.3_1 */	&QX3_3, &c0,	&QX3_2, 0, &QX3_0_3, 0,
/* QX3.3_2 */	&QX3_3, &c0,	&QX3_0_3, 0, &QX3_4, 0,
/* QX3.4_0 */	&QX3_4, 0, 0,
/* QX3.4_1 */	&QX3_4, &c0,	&QX3_3, 0, &QX3_0_3, 0,
/* QX3.4_2 */	&QX3_4, &c0,	&QX3_0_3, 0, &QX3_5, 0,
/* QX3.5_0 */	&QX3_5, 0, 0,
/* QX3.5_1 */	&QX3_5, &c0,	&QX3_4, 0, &QX3_0_3, 0,
/* QX3.5_2 */	&QX3_5, &c0,	&QX3_0_3, 0, &QX3_6, 0,
/* QX3.6_0 */	&QX3_6, 0, 0,
/* QX3.6_1 */	&QX3_6, &c0,	&QX3_5, 0, &QX3_0_3, 0,
/* QX3.6_2 */	&QX3_6, &c0,	&QX3_0_3, 0, &QX3_7, 0,
/* QX3.7_0 */	&QX3_7, 0, 0,
/* QX3.7_1 */	&QX3_7, &c0,	&QX3_6, 0, &QX3_0_3, 0,
/* QX3.7_2 */	&QX3_7, &c0,	&QX3_0_3, 0, 0,
/* QX4.0_0 */	&QX4_0, 0, 0,
/* QX4.0_1 */	&QX4_0, &c0,	0, &QX4_0_3, 0,
/* QX4.0_2 */	&QX4_0, &c0,	&QX4_0_3, 0, &QX4_1, 0,
/* QX4.0_4 */	&QX4_0_3, &c0,	&QX4_6, 0, &QX4_7, 0,
/* QX4.0_5 */	&QX4_0_3, &c0,	&QX4_0, 0, &QX4_1, 0,
/* QX4.1_0 */	&QX4_1, 0, 0,
/* QX4.1_1 */	&QX4_1, &c0,	&QX4_0, 0, &QX4_0_3, 0,
/* QX4.1_2 */	&QX4_1, &c0,	&QX4_0_3, 0, &QX4_2, 0,
/* QX4.2_0 */	&QX4_2, 0, 0,
/* QX4.2_1 */	&QX4_2, &c0,	&QX4_1, 0, &QX4_0_3, 0,
/* QX4.2_2 */	&QX4_2, &c0,	&QX4_0_3, 0, &QX4_3, 0,
/* QX4.3_0 */	&QX4_3, 0, 0,
/* QX4.3_1 */	&QX4_3, &c0,	&QX4_2, 0, &QX4_0_3, 0,
/* QX4.3_2 */	&QX4_3, &c0,	&QX4_0_3, 0, &QX4_4, 0,
/* QX4.4_0 */	&QX4_4, 0, 0,
/* QX4.4_1 */	&QX4_4, &c0,	&QX4_3, 0, &QX4_0_3, 0,
/* QX4.4_2 */	&QX4_4, &c0,	&QX4_0_3, 0, &QX4_5, 0,
/* QX4.5_0 */	&QX4_5, 0, 0,
/* QX4.5_1 */	&QX4_5, &c0,	&QX4_4, 0, &QX4_0_3, 0,
/* QX4.5_2 */	&QX4_5, &c0,	&QX4_0_3, 0, &QX4_6, 0,
/* QX4.6_0 */	&QX4_6, 0, 0,
/* QX4.6_1 */	&QX4_6, &c0,	&QX4_5, 0, &QX4_0_3, 0,
/* QX4.6_2 */	&QX4_6, &c0,	&QX4_0_3, 0, &QX4_7, 0,
/* QX4.7_0 */	&QX4_7, 0, 0,
/* QX4.7_1 */	&QX4_7, &c0,	&QX4_6, 0, &QX4_0_3, 0,
/* QX4.7_2 */	&QX4_7, &c0,	&QX4_0_3, 0, 0,
/* QX5.0_0 */	&QX5_0, 0, 0,
/* QX5.0_1 */	&QX5_0, &c0,	0, &QX5_0_3, 0,
/* QX5.0_2 */	&QX5_0, &c0,	&QX5_0_3, 0, &QX5_1, 0,
/* QX5.0_4 */	&QX5_0_3, &c0,	&QX5_6, 0, &QX5_7, 0,
/* QX5.0_5 */	&QX5_0_3, &c0,	&QX5_0, 0, &QX5_1, 0,
/* QX5.1_0 */	&QX5_1, 0, 0,
/* QX5.1_1 */	&QX5_1, &c0,	&QX5_0, 0, &QX5_0_3, 0,
/* QX5.1_2 */	&QX5_1, &c0,	&QX5_0_3, 0, &QX5_2, 0,
/* QX5.2_0 */	&QX5_2, 0, 0,
/* QX5.2_1 */	&QX5_2, &c0,	&QX5_1, 0, &QX5_0_3, 0,
/* QX5.2_2 */	&QX5_2, &c0,	&QX5_0_3, 0, &QX5_3, 0,
/* QX5.3_0 */	&QX5_3, 0, 0,
/* QX5.3_1 */	&QX5_3, &c0,	&QX5_2, 0, &QX5_0_3, 0,
/* QX5.3_2 */	&QX5_3, &c0,	&QX5_0_3, 0, &QX5_4, 0,
/* QX5.4_0 */	&QX5_4, 0, 0,
/* QX5.4_1 */	&QX5_4, &c0,	&QX5_3, 0, &QX5_0_3, 0,
/* QX5.4_2 */	&QX5_4, &c0,	&QX5_0_3, 0, &QX5_5, 0,
/* QX5.5_0 */	&QX5_5, 0, 0,
/* QX5.5_1 */	&QX5_5, &c0,	&QX5_4, 0, &QX5_0_3, 0,
/* QX5.5_2 */	&QX5_5, &c0,	&QX5_0_3, 0, &QX5_6, 0,
/* QX5.6_0 */	&QX5_6, 0, 0,
/* QX5.6_1 */	&QX5_6, &c0,	&QX5_5, 0, &QX5_0_3, 0,
/* QX5.6_2 */	&QX5_6, &c0,	&QX5_0_3, 0, &QX5_7, 0,
/* QX5.7_0 */	&QX5_7, 0, 0,
/* QX5.7_1 */	&QX5_7, &c0,	&QX5_6, 0, &QX5_0_3, 0,
/* QX5.7_2 */	&QX5_7, &c0,	&QX5_0_3, 0, 0,
/* QX6.0_0 */	&QX6_0, 0, 0,
/* QX6.0_1 */	&QX6_0, &c0,	0, &QX6_0_3, 0,
/* QX6.0_2 */	&QX6_0, &c0,	&QX6_0_3, 0, &QX6_1, 0,
/* QX6.0_4 */	&QX6_0_3, &c0,	&QX6_6, 0, &QX6_7, 0,
/* QX6.0_5 */	&QX6_0_3, &c0,	&QX6_0, 0, &QX6_1, 0,
/* QX6.1_0 */	&QX6_1, 0, 0,
/* QX6.1_1 */	&QX6_1, &c0,	&QX6_0, 0, &QX6_0_3, 0,
/* QX6.1_2 */	&QX6_1, &c0,	&QX6_0_3, 0, &QX6_2, 0,
/* QX6.2_0 */	&QX6_2, 0, 0,
/* QX6.2_1 */	&QX6_2, &c0,	&QX6_1, 0, &QX6_0_3, 0,
/* QX6.2_2 */	&QX6_2, &c0,	&QX6_0_3, 0, &QX6_3, 0,
/* QX6.3_0 */	&QX6_3, 0, 0,
/* QX6.3_1 */	&QX6_3, &c0,	&QX6_2, 0, &QX6_0_3, 0,
/* QX6.3_2 */	&QX6_3, &c0,	&QX6_0_3, 0, &QX6_4, 0,
/* QX6.4_0 */	&QX6_4, 0, 0,
/* QX6.4_1 */	&QX6_4, &c0,	&QX6_3, 0, &QX6_0_3, 0,
/* QX6.4_2 */	&QX6_4, &c0,	&QX6_0_3, 0, &QX6_5, 0,
/* QX6.5_0 */	&QX6_5, 0, 0,
/* QX6.5_1 */	&QX6_5, &c0,	&QX6_4, 0, &QX6_0_3, 0,
/* QX6.5_2 */	&QX6_5, &c0,	&QX6_0_3, 0, &QX6_6, 0,
/* QX6.6_0 */	&QX6_6, 0, 0,
/* QX6.6_1 */	&QX6_6, &c0,	&QX6_5, 0, &QX6_0_3, 0,
/* QX6.6_2 */	&QX6_6, &c0,	&QX6_0_3, 0, &QX6_7, 0,
/* QX6.7_0 */	&QX6_7, 0, 0,
/* QX6.7_1 */	&QX6_7, &c0,	&QX6_6, 0, &QX6_0_3, 0,
/* QX6.7_2 */	&QX6_7, &c0,	&QX6_0_3, 0, 0,
/* QX7.0_0 */	&QX7_0, 0, 0,
/* QX7.0_1 */	&QX7_0, &c0,	0, &QX7_0_3, 0,
/* QX7.0_2 */	&QX7_0, &c0,	&QX7_0_3, 0, &QX7_1, 0,
/* QX7.0_4 */	&QX7_0_3, &c0,	&QX7_6, 0, &QX7_7, 0,
/* QX7.0_5 */	&QX7_0_3, &c0,	&QX7_0, 0, &QX7_1, 0,
/* QX7.1_0 */	&QX7_1, 0, 0,
/* QX7.1_1 */	&QX7_1, &c0,	&QX7_0, 0, &QX7_0_3, 0,
/* QX7.1_2 */	&QX7_1, &c0,	&QX7_0_3, 0, &QX7_2, 0,
/* QX7.2_0 */	&QX7_2, 0, 0,
/* QX7.2_1 */	&QX7_2, &c0,	&QX7_1, 0, &QX7_0_3, 0,
/* QX7.2_2 */	&QX7_2, &c0,	&QX7_0_3, 0, &QX7_3, 0,
/* QX7.3_0 */	&QX7_3, 0, 0,
/* QX7.3_1 */	&QX7_3, &c0,	&QX7_2, 0, &QX7_0_3, 0,
/* QX7.3_2 */	&QX7_3, &c0,	&QX7_0_3, 0, &QX7_4, 0,
/* QX7.4_0 */	&QX7_4, 0, 0,
/* QX7.4_1 */	&QX7_4, &c0,	&QX7_3, 0, &QX7_0_3, 0,
/* QX7.4_2 */	&QX7_4, &c0,	&QX7_0_3, 0, &QX7_5, 0,
/* QX7.5_0 */	&QX7_5, 0, 0,
/* QX7.5_1 */	&QX7_5, &c0,	&QX7_4, 0, &QX7_0_3, 0,
/* QX7.5_2 */	&QX7_5, &c0,	&QX7_0_3, 0, &QX7_6, 0,
/* QX7.6_0 */	&QX7_6, 0, 0,
/* QX7.6_1 */	&QX7_6, &c0,	&QX7_5, 0, &QX7_0_3, 0,
/* QX7.6_2 */	&QX7_6, &c0,	&QX7_0_3, 0, &QX7_7, 0,
/* QX7.7_0 */	&QX7_7, 0, 0,
/* QX7.7_1 */	&QX7_7, &c0,	&QX7_6, 0, &QX7_0_3, 0,
/* QX7.7_2 */	&QX7_7, &c0,	&QX7_0_3, 0, 0,
/* QX9.0_0 */	&QX0_0_3, 0, 0,
/* QX9.1_0 */	&QX1_0_3, 0, 0,
/* QX9.2_0 */	&QX2_0_3, 0, 0,
/* QX9.3_0 */	&QX3_0_3, 0, 0,
/* QX9.4_0 */	&QX4_0_3, 0, 0,
/* QX9.5_0 */	&QX5_0_3, 0, 0,
/* QX9.6_0 */	&QX6_0_3, 0, 0,
/* QX9.7_0 */	&QX7_0_3, 0, 0,
/* b0 */	&b0_2, &b0_1, 0, 0,
/* b0_3 */	&b0_2, &iClock,	&IX0_0, 0, &f0, 0,
/* b0_4 */	&b0_1, &t, &IB8,	&f0, 0, &b0, 0,
/* base */	&base_7, &base_6, &base_5, &base_4, &base_3, &base_2,
		&base_1, 0, 0,
/* base_1 */	&TX0_1, 0, &IX0_7, &IX0_6, &IX0_5, &IX0_4, &IX0_3,
		&IX0_2, 0,
/* base_2 */	&TX0_2, &IX0_2, 0, &IX0_7, &IX0_6, &IX0_5, &IX0_4,
		&IX0_3, 0,
/* base_3 */	&TX0_3, &IX0_3, 0, &IX0_7, &IX0_6, &IX0_5, &IX0_4, 0,
/* base_4 */	&TX0_4, &IX0_4, 0, &IX0_7, &IX0_6, &IX0_5, 0,
/* base_5 */	&TX0_5, &IX0_5, 0, &IX0_7, &IX0_6, 0,
/* base_6 */	&TX0_6, &IX0_6, 0, &IX0_7, 0,
/* base_7 */	&TX0_7, &IX0_7, 0, 0,
/* c0_1 */	&c0, &iClock,	&b0, 0, 0,
/* f0 */	(iC_Gt*)0, &IB8, 0,
/* t_1 */	&t, &iClock,	&base, 0, 0,
/* t_2 */	&t, &iClock,	0, &base, 0,
};
