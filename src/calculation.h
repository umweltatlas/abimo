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

#ifndef CALCULATION_H
#define CALCULATION_H
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <math.h>

#include "dbaseReader.h"
#include "dbaseWriter.h"
#include "pdr.h"
#include "bagrov.h"


class Calculation :public  QObject
{
    Q_OBJECT

public:
    Calculation(DbaseReader & dbR,InitValues & init, QTextStream & protoStream);
    bool calc(QString fileOut);
    long getProtCount();
    long getKeineFlaechenAngegeben();
    long getNutzungIstNull();
    int getTotalRecWrite();
    int getTotalRecRead();
    int getTotalBERtoZeroForced();
    QString getError();
    void stop();

signals:
    void processSignal(int, QString);

private:
    //
    InitValues & initValues;
    QTextStream & protokollStream;
    DbaseReader & dbReader;
    PDR ptrDA;
    QString error;

    // Anzahl der Protokolleinträge
    long protcount;

    // Anzahl der nicht berechneten Flaechen
    long keineFlaechenAngegeben;

    long nutzungIstNull;

    // ******vorläufig aus Teilblock 0 wird für die Folgeblocks genommen
    float regenja, regenso;

    // Abflüsse nach Bagrov für N1 bis N4
    float RDV, R1V, R2V, R3V, R4V;

    float RUV;

    // Regenwasserabfluss in qubikzentimeter pro sec
    float ROWVOL;

    // unterirdischer Gesamtabfluss in qcm/s
    float RIVOL;

    // Gesamtabfluss in qcm/s
    float RVOL;

    // potentielle Aufstiegshöhe
    float TAS;

    //Niederschlags-Korrekturfaktor
    float niedKorrFaktor;

    // Feldlänge von iTAS
    int lenTAS;

    // Feldlänge von inFK_S
    int lenS;

    //total written records
    int totalRecWrite;

    //total read records
    int totalRecRead;

    //Anzahl der Records fuer die BER==0 gesetzt werden musste
    int totalBERtoZeroForced;

    //to stop calc
    bool weiter;

    //functions
    float getTWS (int ert, char nutz);
    float min(float  x,float  y );
    int index (float wert, float *feld, int anz);
    float getNUV(PDR &B);
    float getF ( float wa );
    float getG02 ( int nFK);
    void getNUTZ(int nutz, int typ, int f30, int f150, QString codestr);
    void getKLIMA(int bez, QString codestr);
};
#endif
