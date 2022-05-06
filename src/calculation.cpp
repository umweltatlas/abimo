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

#include "calculation.h"

static float EKA[]=  {0.04176F  ,-0.647F   ,0.218F    , 0.01472F ,0.0002089F,
                      0.04594F  ,-0.314F   ,0.417F    , 0.02463F ,0.0001143F,
                      0.05177F  ,-0.010F   ,0.596F    , 0.02656F ,0.0002786F,
                      0.05693F  , 0.033F   ,0.676F    , 0.0279F  ,0.00035F  ,
                      0.06162F  , 0.176F   ,0.773F    , 0.02809F ,0.0004695F,
                      0.06962F  , 0.24F    ,0.904F    , 0.02562F ,0.0007149F,
                      0.0796F   , 0.31F    ,1.039F    , 0.0288F  ,0.0008696F,
                      0.07998F  , 0.7603F  ,1.2F      , 0.0471F  ,0.000293F ,
                      0.08762F  , 1.019F   ,1.373F    , 0.04099F ,0.0014141F,
                      0.11833F  , 1.1334F  ,1.95F     , 0.0525F  ,0.00125F  ,
                      0.155F    , 1.5F     ,2.64999F  , 0.0725F  ,0.001249F ,
                      0.20041F  , 2.0918F  ,3.69999F  , 0.08F    ,0.001999F ,
                      0.33895F  , 3.721F   ,6.69999F  ,-0.07F    ,0.013F
                     };

static float iTAS[] = {0.1F, 0.2F, 0.3F, 0.4F, 0.5F, 0.6F, 0.7F, 0.8F,
                       0.9F, 1.0F, 1.2F, 1.4F, 1.7F, 2.0F, 2.3F
                      };

// Sande
static float inFK_S[] = {8.0F, 9.0F, 14.0F, 14.5F, 15.5F, 17.0F, 20.5F };

static float ijkr_S[] = {7.0F,6.0F,5.0F,1.5F,0.5F,0.2F,0.1F,0.0F,0.0F,0.0F,0.0F,0.0F,0.0F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,5.0F,3.0F,1.2F,0.5F,0.2F,0.1F,0.0F,0.0F,0.0F,0.0F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,6.0F,5.0F,3.0F,1.5F,0.7F,0.3F,0.15F,0.1F,0.0F,0.0F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,6.0F,5.0F,3.0F,2.0F,1.0F,0.7F,0.4F,0.15F,0.1F,0.0F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,6.0F,5.0F,4.5F,2.5F,1.5F,0.7F,0.4F,0.15F,0.1F,0.0F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,6.0F,5.0F,5.0F,3.5F,2.0F,1.5F,0.8F,0.3F,0.1F,0.05F,0.0F,0.0F,
                         7.0F,7.0F,6.0F,6.0F,6.0F,5.0F,5.0F,5.0F,3.0F,2.0F,1.0F,0.5F,0.15F,0.0F,0.0F
                        };

Calculation::Calculation(DbaseReader & dbR,InitValues & init, QTextStream & protoStream):
        initValues(init), protokollStream(protoStream), dbReader(dbR),
        protcount(0), regenja(0), regenso(0), RDV(0), R1V(0), R2V(0),
        R3V(0), R4V(0), RUV(0), ROWVOL(0), RIVOL(0), RVOL(0), TAS(0),
        lenTAS(15), lenS(7), totalRecWrite(0), totalRecRead(0), totalBERtoZeroForced(0),
        weiter(true)
{}

void Calculation::stop() {
    weiter = false;
}

long Calculation::getProtCount()
{
    return protcount;
}

long Calculation::getKeineFlaechenAngegeben()
{
    return keineFlaechenAngegeben;
}

int Calculation::getTotalRecWrite() {
    return totalRecWrite;
}

int Calculation::getTotalRecRead() {
    return totalRecRead;
}

int Calculation::getTotalBERtoZeroForced() {
    return totalBERtoZeroForced;
}

long Calculation::getNutzungIstNull() {
    return nutzungIstNull;
}

QString Calculation::getError() {
    return error;
}


/**
 =======================================================================================================================
    I m p o r t D B () Diese Funktion importiert die Datensätze aus der DBASE-Datei FileName in das DA Feld ein
    (GWD-Daten). Parameter: out-file Rückgabewert: BOOL TRUE, wenn das Einlesen der Datei
    erfolgreich war.
 =======================================================================================================================
 */
bool Calculation::calc(QString   fileOut)
{

    /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
    /* Variablen zur Berechnung */
    int    index = 0;
    float  vgd, vgb, vgs, kd, kb, ks;
    float  bl1, bl2, bl3, bl4, bls1, bls2, bls3, bls4;
    float  fb, fs, fbant, fsant;
    float  row1, row2, row3, row4;
    float  ri1, ri2, ri3, ri4;
    float  rowd, rid;                /* Dachflächen */
    float  rowuvs, riuvs;        /* unversiegelte Str.-fläche */
    float  riuv;                        /* unversiegelte Fläche */
    float  r, ri, row;                /* float-Zwischenwerte */

    niedKorrFaktor = initValues.getNiedKorrF();

    /* zählen der protokolleinträge */
    protcount = 0L;
    keineFlaechenAngegeben = 0L;
    nutzungIstNull = 0L;

    /* Starteintrag ins protokoll */

    DbaseWriter writer(fileOut, initValues);

    totalRecRead = dbReader.getNumberOfRecords();
    int k;
    for (k=0; k < totalRecRead; k++)
    {
        if (!weiter) {
            protokollStream << "Berechnungen abgebrochen.\r\n";
            return true;
        }
        ptrDA.wIndex = index;
        int nutzung = dbReader.getRecord(k, "NUTZUNG").toInt();
        if (nutzung != 0)
        {
            QString codestr = dbReader.getRecord(k, "CODE");

            regenja = (dbReader.getRecord(k, "REGENJA").toInt()); /* Jetzt regenja,-so OK */
            regenso = (dbReader.getRecord(k, "REGENSO").toInt());

            ptrDA.FLW = (dbReader.getRecord(k, "FLUR")).toFloat();

            getNUTZ(
                nutzung,
                (dbReader.getRecord(k, "TYP")).toInt(),
                (dbReader.getRecord(k, "FELD_30")).toInt(),
                (dbReader.getRecord(k, "FELD_150")).toInt(),
                codestr
            );
            /* cls_6a: an dieser Stelle muss garantiert werden, dass f30 und f150
               als Parameter von getNutz einen definierten Wert erhalten und zwar 0.

               FIXED: alle werte sind definiert... wenn keine 0, sondern nichts b.z.w. leerzeichen angegeben wurden
               wird nun eine 0 eingesetzt
               aber eigentlich war das auch schon so ... ???
            */

            /* Bagrov-Berechnung für versiegelte Flächen */
            getKLIMA((dbReader.getRecord(k, "BEZIRK")).toInt(), codestr);

            /* Dateneingabe */
            vgd = (dbReader.getRecord(k, "PROBAU")).toFloat() / 100.0F; /* Dachflächen */
            vgb = (dbReader.getRecord(k, "PROVGU")).toFloat() / 100.0F; /* Hofflächen */
            ptrDA.VER = (int)round((vgd * 100) + (vgb * 100));
            vgs = (dbReader.getRecord(k, "VGSTRASSE")).toFloat() / 100.0F;
            kd = (dbReader.getRecord(k, "KAN_BEB")).toFloat() / 100.0F;
            kb = (dbReader.getRecord(k, "KAN_VGU")).toFloat() / 100.0F;
            ks = (dbReader.getRecord(k, "KAN_STR")).toFloat() / 100.0F;

            bl1 = (dbReader.getRecord(k, "BELAG1")).toFloat() / 100.0F;
            bl2 = (dbReader.getRecord(k, "BELAG2")).toFloat() / 100.0F;
            bl3 = (dbReader.getRecord(k, "BELAG3")).toFloat() / 100.0F;
            bl4 = (dbReader.getRecord(k, "BELAG4")).toFloat() / 100.0F;
            bls1 = (dbReader.getRecord(k, "STR_BELAG1")).toFloat() / 100.0F;
            bls2 = (dbReader.getRecord(k, "STR_BELAG2")).toFloat() / 100.0F;
            bls3 = (dbReader.getRecord(k, "STR_BELAG3")).toFloat() / 100.0F;
            bls4 = (dbReader.getRecord(k, "STR_BELAG4")).toFloat() / 100.0F;
            fb = (dbReader.getRecord(k, "FLGES")).toFloat();
            fs = (dbReader.getRecord(k, "STR_FLGES")).toFloat();

            if (fb + fs < 0.0001)
            {
                //*protokollStream << "\r\nDie Fläche des Elements " + codestr + " ist 0 \r\nund wird automatisch auf 100 gesetzt\r\n";
                protcount++;
                keineFlaechenAngegeben++;
                fb = 100.0F;
            }

            fbant = fb / (fb + fs);
            fsant = fs / (fb + fs);

            /* Abfluss für versiegelte Flächen */
            /* cls_1: Fehler a:
               rowd = (1.0F - initValues.getInfdach()) * vgd * kb * fbant * RDV;
               richtige Zeile folgt (kb ----> kd)
            */
            rowd = (1.0F - initValues.getInfdach()) * vgd * kd * fbant * RDV;
            row1 = (1.0F - initValues.getInfbel1()) * (bl1 * kb * vgb * fbant + bls1 * ks * vgs * fsant) * R1V;
            row2 = (1.0F - initValues.getInfbel2()) * (bl2 * kb * vgb * fbant + bls2 * ks * vgs * fsant) * R2V;
            row3 = (1.0F - initValues.getInfbel3()) * (bl3 * kb * vgb * fbant + bls3 * ks * vgs * fsant) * R3V;
            row4 = (1.0F - initValues.getInfbel4()) * (bl4 * kb * vgb * fbant + bls4 * ks * vgs * fsant) * R4V;

            /* Infiltration für versiegelte Flächen */
            rid = (1 - kd) * vgd * fbant * RDV;
            ri1 = (bl1 * vgb * fbant + bls1 * vgs * fsant) * R1V - row1;
            ri2 = (bl2 * vgb * fbant + bls2 * vgs * fsant) * R2V - row2;
            ri3 = (bl3 * vgb * fbant + bls3 * vgs * fsant) * R3V - row3;
            ri4 = (bl4 * vgb * fbant + bls4 * vgs * fsant) * R4V - row4;

            /* Unversiegelte Strassenfläche werden wie Belag 4 behandelt: */
            rowuvs = 0.0F;        /* old: 0.11F * (1-vgs) * fsant * R4V; */
            riuvs = (1 - vgs) * fsant * R4V;        /* old: 0.89F * (1-vgs) * fsant * R4V;  */

            /* Abfluss für unversiegelte Fläche rowuv = 0 */
            riuv = (100.0F - (float) ptrDA.VER) / 100.0F * RUV;

            /* Berechnung des Abflusses für die gesamte Blockteilfläche (FLGES+STR_FLGES) */
            row = (row1 + row2 + row3 + row4 + rowd + rowuvs);        /* mm/a */
            ptrDA.ROW = (int)round(row);
            ROWVOL = row * 3.171F * (fb + fs) / 100000.0F;                /* qcm/s */
            ri = (ri1 + ri2 + ri3 + ri4 + rid + riuvs + riuv);        /* mm/a */
            ptrDA.RI = (int)round(ri);
            RIVOL = ri * 3.171F * (fb + fs) / 100000.0F;                /* qcm/s */
            r = row + ri;
            ptrDA.R = (int)round(r);
            RVOL = ROWVOL + RIVOL;

            float        flaeche1 = fb + fs;
// cls_5b:
            float verdunst = (regenja * niedKorrFaktor) - r;

            writer.addRecord();
            writer.setRecordField("CODE", codestr);
            writer.setRecordField("R", r);
            writer.setRecordField("ROW", row);
            writer.setRecordField("RI", ri);
            writer.setRecordField("RVOL", RVOL);
            writer.setRecordField("ROWVOL", ROWVOL);
            writer.setRecordField("RIVOL", RIVOL);
            writer.setRecordField("FLAECHE", flaeche1);
// cls_5c:
            writer.setRecordField("VERDUNSTUN", verdunst);

            index++;
        }
        else {
            nutzungIstNull++;

        }
        /* cls_2: hier könnten falls gewünscht die Flächen dokumentiert werden,
           deren NUTZUNG=NULL (siehe auch cls_3)
        */
        float tmp = (k) * 50;
        float prog = tmp/(float)totalRecRead;
        emit processSignal((int)(prog),"Berechne");
    }

    totalRecWrite = index;

    emit processSignal(50,"Schreibe Ergebnisse.");

    if (!writer.write()) {
        protokollStream << "Error: "+ writer.getError() +"\r\n";
        error = "Fehler beim Schreiben der Ergebnisse.\n" + writer.getError();
        return false;
    }


    return true;
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
void Calculation::getNUTZ(int nutz, int typ, int f30, int f150, QString codestr)
{


    /* globale Größen für den aktuellen Record */
    float        TWS;        /* Durchwurzelungstiefe */
    float        kr;                /* mittlere pot. kapillare Aufstiegsrate d. Sommerhalbjahres */
    int                dw;                /* mittlere Zahl der Wachstumstage */

    /*
     * Feldlängen von iTAS und inFK_S, L, T, U ;
     * extern int lenTAS, lenS, lenL, lenT, lenU;
     */
    ptrDA.ERT = 0;
    ptrDA.BER = 0;
    switch (nutz)
    {
    case 10:
    case 21:
    case 22:
    case 23:
    case 30:
        switch (typ)
        {
        case 1:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 2:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 3:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 4:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 5:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 6:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 7:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 8:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 9:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 10:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 11:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 21:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 22:
            ptrDA.NUT = 'K';
            ptrDA.ERT = 40;
            ptrDA.BER = 75;
            break;

        case 23:
            ptrDA.NUT = 'K';
            ptrDA.ERT = 40;
            ptrDA.BER = 75;
            break;

        case 24:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 55;
            ptrDA.BER = 75;
            break;

        case 25:
            ptrDA.NUT = 'K';
            ptrDA.ERT = 40;
            ptrDA.BER = 75;
            break;

        case 26:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 29:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 30;
            break;

        case 33:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 38:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 39:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 71:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 72:
        case 73:
        case 74:
            /* cls_4: Baustrukturtypen 73 und 74 neu eingeführt - werden behandelt wie 72
            */
            ptrDA.NUT = 'L';
            ptrDA.ERT = 50;
            break;

        default:
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr +  "\r\nTyp=72 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'L';
            ptrDA.ERT = 50;
            break;
        }
        break;

    case 40:
        switch (typ)
        {
        case 30:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 31:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 30;
            break;

        default:
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=31 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'L';
            ptrDA.ERT = 30;
            break;
        }
        break;

    case 50:
        switch (typ)
        {
        case 12:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 13:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 50;
            break;

        case 14:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 50;
            break;

        case 28:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 41:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 42:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 43:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 35;
            break;

        case 44:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            ptrDA.BER = 50;
            break;

        case 45:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 46:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 50;
            ptrDA.BER = 50;
            break;

        case 47:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 49:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            ptrDA.BER = 50;
            break;

        case 50:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            ptrDA.BER = 50;
            break;

        case 51:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        case 60:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;

        default:
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=60 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'L';
            ptrDA.ERT = 45;
            break;
        }
        break;

    case 60:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 45;
        break;

    case 70:
        if (typ == 59)
        {
            ptrDA.NUT = 'K';
            ptrDA.ERT = 40;
            ptrDA.BER = 75;
        }
        else
        {
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=59 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'K';
            ptrDA.ERT = 40;
            ptrDA.BER = 75;
        }
        break;

    case 80:
        switch (typ)
        {
        case 91:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 40;
            break;

        case 92:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 25;
            break;

        case 93:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 30;
            break;

        case 94:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 30;
            break;

        case 99:
            ptrDA.NUT = 'L';
            ptrDA.ERT = 10;
            break;

        default:
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=99 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'L';
            ptrDA.ERT = 10;
            break;
        }
        break;

    case 90:
        if (typ == 98)
        {
            ptrDA.NUT = 'D';
            ptrDA.ERT = 1;
            ptrDA.BER = 0;
        }
        else
        {
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=98 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'D';
            ptrDA.ERT = 1;
            ptrDA.BER = 0;
        }
        break;

    case 100:
        if (typ == 55)
            ptrDA.NUT = 'W';
        else
        {
            protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=55 angenommen\r\n";
            protcount++;
            ptrDA.NUT = 'W';
        }
        break;

    case 101:
        ptrDA.NUT = 'W';
        break;

    case 102:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 60;
        break;

    case 110:
        ptrDA.NUT = 'G';
        break;

    case 121:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 40;
        break;

    case 122:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 35;
        break;

    case 130:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 50;
        ptrDA.BER = 50;
        break;

    case 140:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 50;
        break;

    case 150:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 50;
        ptrDA.BER = 100;
        break;

    case 160:
        ptrDA.NUT = 'K';
        ptrDA.ERT = 40;
        ptrDA.BER = 75;
        break;

    case 161:
        ptrDA.NUT = 'K';
        ptrDA.ERT = 40;
        ptrDA.BER = 75;
        break;

    case 162:
        ptrDA.NUT = 'K';
        ptrDA.ERT = 40;
        ptrDA.BER = 75;
        break;

    case 170:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 10;
        break;

    case 171:
        ptrDA.NUT = 'D';
        ptrDA.ERT = 1;
        ptrDA.BER = 0;
        break;

    case 172:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 40;
        break;

    case 173:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 45;
        break;

    case 174:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 60;
        break;

    case 180:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 50;
        break;

    case 190:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 40;
        break;

    case 200:
        ptrDA.NUT = 'L';
        ptrDA.ERT = 50;
        ptrDA.BER = 50;
        break;

    default:
        /*
           *protokollStream << "\r\nNutzungstyp nicht definiert für Element " + codestr + "\r\nTyp=200 angenommen\r\n";
           protcount++;
           ptrDA.NUT = 'L';
           ptrDA.ERT = 50;
           ptrDA.BER = 50;
           cls_3: dies ist nicht korrekt, da die Nutzung und nicht der Nutzungstyp im switch liegt
           und ein NULL in NUTZUNG hoffentlich immer zu nutzung=0 führt, wenn oben
           int nutzung = dbReader.getRecord(k, "NUTZUNG").toInt();
           aufgerufen wird (siehe auch cls_2) -
           deshalb folgende Fehlermeldung
        */
        protokollStream << "\r\nDiese  Meldung sollte nie erscheinen: \r\nNutzung nicht definiert für Element " + codestr + "\r\n";
        break;
    }

    if (ptrDA.NUT != 'G')
    {
        /* pot. Aufstiegshöhe TAS = FLUR - mittl. Durchwurzelungstiefe TWS */
        TWS = getTWS(ptrDA.ERT, ptrDA.NUT);
        TAS = ptrDA.FLW - TWS;

        /* Feldkapazität */
        /* cls_6b: der Fall der mit NULL belegten FELD_30 und FELD_150 Werte
           wird hier im erten Fall behandelt - ich erwarte dann den Wert 0 */
        if (min(f30, f150)<1)
            ptrDA.nFK = 13.0F;
        else if (abs(f30 - f150) < min(f30, f150))       /* unwesentliche Abweichung */
            if (ptrDA.NUT == 'W')
                ptrDA.nFK = (float) f150;
            else
                ptrDA.nFK = (float) f30;
        else if (ptrDA.NUT == 'W')
            ptrDA.nFK = 0.75F * (float) f150 + 0.25F * (float) f30;
        else
            ptrDA.nFK = 0.75F * (float) f30 + 0.25F * (float) f150;

        /*
         * mittlere pot. kapillare Aufstiegsrate kr (mm/d) des Sommerhalbjahres ;
         * switch (bod) { case S: case U: case L: case T: case LO: case HN: } wird
         * eingefügt, wenn die Bodenart in das Zahlenmaterial aufgenommen wird vorläufig
         * wird Sande angenommen ;
         * Sande
         */
        if (TAS <= 0.0)
            kr = 7.0F;
        else
        {
            kr = ijkr_S[index(TAS, iTAS, lenTAS) + index(ptrDA.nFK, inFK_S, lenS) * lenTAS];
        }

        /* mittlere pot. kapillare Aufstiegsrate kr (mm/d) des Sommerhalbjahres */
        switch (ptrDA.NUT)
        {
        case 'L':
            if (ptrDA.ERT <= 50) dw = 60;
            else
                dw = 75;
            break;
        case 'K':
            dw = 100;
            break;
        case 'W':
            dw = 90;
            break;
        case 'D':
            dw = 50;
            break;
        default:
            dw = 50;
            break;
        }

        ptrDA.KR = (int) (dw * kr);
    }

    if (initValues.getBERtoZero() && ptrDA.BER != 0) {
        //*protokollStream << "Erzwinge BER=0 fuer Code: " << codestr << ", Wert war:" << ptrDA.BER << " \r\n";
        totalBERtoZeroForced++;
        ptrDA.BER = 0;
    }
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
void Calculation::getKLIMA(int bez, QString codestr)
{

    /* Effektivitätsparameter */
    float        nd = initValues.getBagdach();
    float        n1 = initValues.getBagbel1();
    float        n2 = initValues.getBagbel2();
    float        n3 = initValues.getBagbel3();
    float        n4 = initValues.getBagbel4();
    float        n, bag, zw;

    float        ep, p, x, y, etr;

    QString bezString;
    bezString.setNum(bez);

    /*
     * später zeizusätzliche Parameter Hier ;
     * ptrDA.P1 = p1;
     * * ptrDA.PS = ps;
     */
    ptrDA.P1 = regenja;
    ptrDA.P1S = regenso;

    /* Parameter für die Bezirke */
    if (ptrDA.NUT == 'G')
    {

        if(initValues.hashEG.contains(bez)) {
            //take from xml
            ptrDA.ETP = initValues.hashEG.value(bez);
        } else {
            //default
            ptrDA.ETP = initValues.hashEG.contains(0) ? initValues.hashEG.value(0) : 775;
            QString egString;
            egString.setNum(ptrDA.ETP);
            protokollStream << "\r\nEG unbekannt für " + codestr + " von Bezirk " + bezString + "\r\nEG=" + egString + " angenommen\r\n";
            protcount++;
        }
    }
    else
    {

        if(initValues.hashETP.contains(bez)) {
            //take from xml
            ptrDA.ETP = initValues.hashETP.value(bez);
        } else {
            //default
            ptrDA.ETP = initValues.hashETP.contains(0) ? initValues.hashETP.value(0) : 660;
            QString etpString;
            etpString.setNum(ptrDA.ETP);
            protokollStream << "\r\nETP unbekannt für " + codestr + " von Bezirk " + bezString + "\r\nETP=" + etpString + " angenommen\r\n";
            protcount++;
        }

        if(initValues.hashETPS.contains(bez)) {
            //take from xml
            ptrDA.ETPS = initValues.hashETPS.value(bez);
        } else {
            //default
            ptrDA.ETPS = initValues.hashETPS.contains(0) ? initValues.hashETPS.value(0) : 530;
            QString etpsString;
            etpsString.setNum(ptrDA.ETPS);
            protokollStream << "\r\nETPS unbekannt für " + codestr + " von Bezirk " + bezString + "\r\nETPS=" + etpsString + " angenommen\r\n";
            protcount++;
        }
    }

    ep = (float) ptrDA.ETP;                /* Korrektur mit 1.1 gestrichen */
    p = (float) ptrDA.P1 * niedKorrFaktor;        /* ptrDA.KF;
                                                                         * */

    /*
     * Berechnung der Abflüsse RDV und R1V bis R4V für versiegelte ;
     * Teilflächen und unterschiedliche Bagrovwerte ND und N1 bis N4
     */
    x = p / ep;

    Bagrov bagrovObj;

    bagrovObj.nbagro(&nd, &y, &x);
    RDV = p - y * ep;
    bagrovObj.nbagro(&n1, &y, &x);
    R1V = p - y * ep;
    bagrovObj.nbagro(&n2, &y, &x);
    R2V = p - y * ep;
    bagrovObj.nbagro(&n3, &y, &x);
    R3V = p - y * ep;
    bagrovObj.nbagro(&n4, &y, &x);
    R4V = p - y * ep;

    /* Berechnung des Abflusses RUV für unversiegelte Teilflächen */
    if (ptrDA.NUT == 'G')
        RUV = p - ep;
    else
    {        /* Ermittlung des Effektivitätsparameters bag für unvers. Flächen */
        n = getNUV(ptrDA);                /* Modul Raster abgespeckt */

        if (ptrDA.P1S > 0 && ptrDA.ETPS > 0)
        {
            zw = (float) (ptrDA.P1S + ptrDA.BER + ptrDA.KR) / ptrDA.ETPS;
            bag = getF(zw) * n; /* Modifikation des Parameters durch "Sommer" */
        }
        else
            bag = n;
        x = (p + ptrDA.KR + ptrDA.BER) / ep;
        bagrovObj.nbagro(&bag, &y, &x);

        if (TAS < 0)
            etr = (ep - y * ep) * (float) exp(ptrDA.FLW / TAS) + y * ep;
        else
            etr = y * ep;

        RUV = p - etr;
    }
}

/*
 =======================================================================================================================
    Bestimmung der Durchwurzelungstiefe TWS
 =======================================================================================================================
 */
float Calculation::getTWS(int ert, char nutz)
{
    float        TWS;
    switch (nutz)
    {
    case 'D':
        TWS = 0.2F;
        break;
    case 'L':
        if (ert <= 50) TWS = 0.6F;
        else
            TWS = 0.7F;
        break;
    case 'K':
        TWS = 0.7F;
        break;
    case 'W':
        TWS = 1.0F;
        break;
    default:
        TWS = 0.2F;
        break;
    }

    return(TWS);
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
float Calculation::min(float x, float y)
{
    return x < y ? x : y;
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
int Calculation::index(float wert, float *feld, int anz)
{
    int                i;
    float        eps = 0.0001;
    for (i = 0; i < anz; i++)
        if (wert <= feld[i] + eps) return(i);
    return(anz - 1);
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
float Calculation::getF(float wa)
{
    int                i, anz;
    float        watab[] =
    {
        0.45F, 0.50F, 0.55F, 0.60F, 0.65F, 0.70F, 0.75F,
        0.80F, 0.85F, 0.90F, 0.95F, 1.00F, 1.05F, 1.10F
    };
    float        Ftab[] =
    {
        0.65F, 0.75F, 0.82F, 0.90F, 1.00F, 1.06F, 1.15F,
        1.22F, 1.30F, 1.38F, 1.47F, 1.55F, 1.63F, 1.70F
    };

    anz = 14;
    if (wa <= watab[0]) return(Ftab[0]);
    if (wa >= watab[anz - 1]) return(Ftab[anz - 1]);
    for (i = 1; i < anz; i++)
        if (wa <= watab[i]) return((Ftab[i - 1] + Ftab[i]) / 2);
    return(0);
}

/*
 =======================================================================================================================
    FIXME:
 =======================================================================================================================
 */
float Calculation::getG02(int nFK)
{
    float G02tab [] = { 0.0F,  0.0F,  0.0F,  0.0F,  0.3F,  0.8F,  1.4F,  2.4F,  3.7F,  5.0F,
                        6.3F,  7.7F,  9.3F, 11.0F, 12.4F, 14.7F, 17.4F, 21.0F, 26.0F, 32.0F,
                        39.4F, 44.7F, 48.0F, 50.7F, 52.7F, 54.0F, 55.0F, 55.0F, 55.0F, 55.0F, 55.0F
                      };

    return(G02tab[nFK]);
}

/**
 =======================================================================================================================
    g e t N U V ( Pointer auf aktuellen DataRecord) Berechnung eines Records (abgespecktes raster
 =======================================================================================================================
 */
float Calculation::getNUV(PDR &B)        /* DataRecord_t *B) */
{
    int                K;
    float        G020, BAG0;

    G020 = getG02((int) (B.nFK + 0.5));

    switch (B.NUT)
    {
    case 'W':
        if (G020 <= 10.0)
            BAG0 = 3.0F;
        else if (G020 <= 25.0)
            BAG0 = 4.0F;
        else
            BAG0 = 8.0F;
        break;

    default:
        K = (int) (B.ERT / 5);
        if (B.ERT > 49) K = (int) (B.ERT / 10 + 5);
        if (K <= 0) K = 1;
        if (K >= 4) K = K - 1;
        if (K > 13) K = 13;
        K = 5 * K - 2;
        BAG0 = EKA[K - 1] + EKA[K] * G020 + EKA[K + 1] * G020 * G020;
        if ((BAG0 >= 2.0) && (B.ERT < 60)) BAG0 = EKA[K - 3] * G020 + EKA[K - 2];
        if ((G020 >= 20.0) && (B.ERT >= 60)) BAG0 = EKA[K - 3] * G020 + EKA[K - 2];

        if (B.BER > 0 && (B.P1S == 0 && B.ETPS == 0)) /* Modifikation, wenn keine Sommerwerte */
            BAG0 = BAG0 * (0.9985F + 0.00284F * B.BER - 0.00000379762F * B.BER * B.BER);
        break;
    }

    return(BAG0);
}
