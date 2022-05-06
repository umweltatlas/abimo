/*****************************************************************************
 *   Copyright (C) Glugla, G. und B. König (1989): Der mikrorechnergestützte *
 *   Arbeitsplatz Grundwasserdargebot. Wasserwirtschaft-Wassertechnik,       *
 *   39.Jg. / Meiko Rachimow und Claus Rachimow (2009)                       *
 *   This file is part of Abimo 3.2                                          *
 *   Abimo 3.2 is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 2 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.               *
 *****************************************************************************/

#include "bagrov.h"

/*
 =======================================================================================================================
    Translated by FOR_C, v2.3.2, on 10/16/94 at 18:28:43 ;
    FOR_C Options SET: none ;
    include <stdio.h> ;
    #include <f_rt.h> ;
    NBAGRO & BAGROV - Module zur Lesung der BAGROV-Gleichung AUFRUF: CALL NBAGRO(BAG,y,x) CALL BAGROV(BAG,x,y)
    PROGRAMMTYP: SUBROUTINE, SPRACHE: FORTRAN80 EINGABEPARAMETER: BAG- Bagrov-Parameter mit n=BAG x - x=P/ETP mit
    P=PMD*niedKorrFaktor [mm/a] ETP::Mittlere potentielle Verdunstung [mm/a] AUSGABEPARAMETER: y - ETR/ETP ETR::
    Mittlere reale Verdunstung [mm/a]
 =======================================================================================================================
 */
void Bagrov::nbagro(float *bage, float *y, float *x)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    int				_do0, i, i_, ia, ie, j, j_;
    float			bag, bag1;
    float			a, a0, a1, a2, b, c, epa, eyn, h, h13, h23, qbag1, s1, s2, w, w13, w23, y0;
    static float	aa[16] =
    {
        0.9946811499F,
        1.213648255F,
        -1.350801214F,
        11.80883489F,
        -21.53832235F,
        19.3775197F,
        0.862954876F,
        9.184851852F,
        -147.2049991F,
        1291.164889F,
        -6357.554955F,
        19022.42165F,
        -35235.40521F,
        39509.02815F,
        -24573.23867F,
        6515.556685F
    };
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    y0 = 0.0F;
    bag = *bage;
    if (bag > 20.0) bag = 20.0F;
    if (*x < 0.0005F) goto L_13;
    if (*x > 15.0F) *x = 15.0F;
    bag1 = bag + 1.0F;
    qbag1 = (float) (1.0 / bag1);
    h13 = 1.0F / 3.0F;
    w13 = h13;
    h23 = h13 + h13;
    w23 = h23;
    h13 = (float) exp(-bag1 * 1.09861);
    h23 = (float) exp(-bag1 * 0.405465);

    /* KOEFFIZIENTEN DER BEDINGUNGSGLEICHUNG */
    a2 = -13.5F * qbag1 * (1.0F + 3.0F * (h13 - h23));
    a1 = 9.0F * qbag1 * (h13 + h13 - h23) - w23 * a2;
    a0 = 1.0F - qbag1 - 0.5F * a1 - w13 * a2;
    a0 = 1.0F / a0;
    a1 = a0 * a1;
    a2 = a0 * a2;

    /* KOEFFIZIENTEN DES LOESUNSANSATZES */
    if (bag >= 0.49999F)
        b = 0.5F * a1 - (float) sqrt(0.25 * a1 * a1 - a2);
    else
        b = -(float) sqrt(0.5F * a1 * a1 - a2);
    c = a1 - b;
    a = a0 / (b - c);

    epa = (float) exp(*x / a);
    w = b - c * epa;

    /* NULLTE NAEHERUNGSLOESUNG (1. Naeherungsloesung) */
    y0 = (epa - 1.0F) / w;
    if (y0 > 0.99999F) y0 = 0.99999F;
    if (bag >= 0.7F && bag <= 3.8F) goto L_13;
    j = 0;
    if (bag >= 3.8F) goto L_12;

    /* NUMERISCHE INTEGRATION FUER BAG<0.7 (2.Naeherungsloesung) */
    for (j = 1; j <= 30; j++)
    {
        j_ = j - 1;
        eyn = (float) exp(bag * log(y0));
        if (eyn > 0.9F) goto L_13;
        if (eyn >= 0.7F && bag > 4.0F) goto L_13;
        ia = 2;
        ie = 6;
        if (eyn > 0.7F) ia = 8;
        if (eyn > 0.7F) ie = 16;
        s1 = 0.0F;
        s2 = 0.0F;
        h = 1.0F;
        for (i = ia, _do0 = ie; i <= _do0; i++)
        {
            i_ = i - 1;
            h = h * eyn;
            w = aa[i_] * h;
            j = i - ia + 1; /* cls J=I-IA+1 */
            s2 = s2 + w / (j * (float) bag + 1.0F);
            s1 = s1 + w;
        }

        ia = ia - 1;
        h = aa[ia - 1];
        h = (*x - y0 * s2 - y0 * h) / (h + s1);
        y0 = y0 + h;
        if (fabs(h) / y0 < 0.007F) goto L_15;
    }

L_15:
    ;
    if (y0 > 0.9) bagrov(&bag, x, &y0);
    goto L_13;

    /* NUMERISCHE INTEGRATION FUER BAG>3.8 (3. Naeherungsloesung) */
L_12:
    ;
    j = j + 1;
    if (j > 15) goto L_13;
    if (y0 > 0.999) y0 = 0.999F;
    epa = (float) exp(bag * log(y0));
    h = 1.0f - epa;
    if (h < 1.0e-07) h = 1.0e-07F;
    if (h > 0.99999) h = 0.99999F;
    s1 = h - bag * epa / (float) log(h);
    h = h * (y0 + epa * y0 / s1 -*x);
    y0 = y0 - h;
    if (fabs(h) > 0.001) goto L_12;
L_13:
    ;
    if (y0 > 1.0) y0 = 1.0F;
    *y = y0;
    return;
}	/* end of function */

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
void Bagrov::bagrov(float *bagf, float *x0, float *y0)
{
    /*~~~~~~~~~~~~~~~~~~~~~~~~*/
    bool	doloop; /* LOGICAL16 */
    int		_do0, i, ii, ii_, j;
    /*~~~~~~~~~~~~~~~~~~~~~~~~*/

    /* meiko : initialisiere i (einzige Änderung) */
    i = 0;

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    float	delta, du, h, s, s1, sg, si, su, u, x;
    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    if (*x0 == 0.0) goto L_10;
    *y0 = 0.99F;
    goto L_20;
L_10:
    *y0 = 0.0F;
    return;
L_20:
    doloop = false;

    /* NUMERISCHE INTEGRATION DER BAGROVBEZIEHUNG */
L_21:
    j = 1;
    du = 2.0F **y0;
    h = 1.0F + 1.0F / (1.0F - (float) exp(*bagf * log(*y0)));
    si = h * du / 4.0F;
    sg = 0.0F;
    su = 0.0F;
L_1:
    s = si;
    j = j * 2;
    du = du / 2.0F;
    u = du / 2.0F;
    sg = sg + su;
    su = 0.0F;
    for (ii = 1, _do0 = j; ii <= _do0; ii += 2)
    {
        ii_ = ii - 1;
        su = su + 1.0F / (1.0F - (float) exp(*bagf * log(u)));
        u = u + du;
    }

    si = (2.0F * sg + 4.0F * su + h) * du / 6.0F;
    s1 = 0.001F * s;
    if (fabs(s - si) > s1) goto L_1;
    x = si;

    /* ENDE DER NUMERISCHEN INTEGRATION */
    if (doloop) goto L_42;
    if (*x0 > x) goto L_30;
    *y0 = 0.5F;
    goto L_40;
L_30:
    *y0 = 1.0F;
    return;
L_40:
    i = 1;

    /* SCHLEIFE I=1(1)10 ZUR BERECHNUNG VON DELTA */
L_41:
    doloop = true;
    goto L_21;
L_42:
    delta = (*x0 - x) * (1.0F - (float) exp(*bagf * (float) log(*y0)));
    *y0 = *y0 + delta;
    if (*y0 >= 1.0) goto L_50;
    if (*y0 <= 0.0) goto L_60;
    goto L_70;
L_50:
    *y0 = 0.99F;
    goto L_90;
L_60:
    *y0 = 0.01F;
    goto L_90;
L_70:
    if (fabs(delta) < 0.01F) goto L_80;
    goto L_90;
L_80:
    ;
    return;
L_90:
    if (i < 10) goto L_91;
    goto L_92;
L_91:
    i = i + 1;
    goto L_41;
L_92:
    ;
    return;
}	/* end of function */
