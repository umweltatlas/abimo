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

#ifndef PDR_H
#define PDR_H



class PDR
{
public:
    PDR();

    // Elementnummer EB_INDEX neu
    unsigned wIndex;

    // nFK-Wert (ergibt sich aus Bodenart) ID_NFK neu
    float nFK;

    // Flurabstandswert [m] ID_FLW 4.1 N
    float FLW;

    // Hauptnutzungsform [L,W,G,B,K,D] ID_NUT 001 C
    char NUT;

    // Langjähriger MW des Gesamtabflusses [mm/a] 004 N
    int R;

    // Langjähriger MW des Regenwasserabflusses [mm/a] 003 N
    int ROW;

    // Langjähriger MW des unterird. Abflusses [mm/a] 004 N
    int RI;

    // Versiegelungsgrad bebauter Flächen [%] ID_VER 002 N
    int VER;

    // ERTragsklasse landw. Nutzflächen ID_ERT 002 N
    int ERT;

    // j. Beregnungshöhe landw. Nutzfl. [mm/a] ID_BER 003 N
    int BER;

    // Niederschlag <MD-Wert> [mm/a] ID_PMD 003 N
    float P1;

    // l-j. MW der pot. Verdunstung [mm/a] ID_ETP 003 N
    int ETP;

    // Kapillarer Aufstieg pro Jahr ID_KR neu
    int KR;

    // Sommer-Niederschlag ID_PS neu
    float P1S;

    // potentielle Verdunstung im Sommer ID_ETPS neu
    int ETPS;
};
#endif
