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

#include "saxhandler.h"

SaxHandler::SaxHandler(InitValues &iV):state(0), initValues(iV) {}

bool SaxHandler::startElement(const QString &, const QString &,
                              const QString &qName,
                              const QXmlAttributes &attribs)
{

    if (qName == "section") {
        if (attribs.value("name") == "Infiltrationsfaktoren") {
            state = Infiltrationsfaktoren;
        }

        else if (attribs.value("name") == "Bagrovwerte") {
            state = Bagrovwerte;
        }

        else if (attribs.value("name") == "ErgebnisNachkommaStellen") {
            state = Nachkomma;
        }

        else if (attribs.value("name") == "Diverse") {
            state = Diverse;
        }
        else if (attribs.value("name") == "Gewaesserverdunstung") {
            state = GewVerd;
        }
        else if (attribs.value("name") == "PotentielleVerdunstung") {
            state = PotVerd;
        }
    }
    if (qName == "item") {
        switch (state) {
        case Infiltrationsfaktoren :
            if (attribs.value("key") == "Dachflaechen")
                initValues.setInfdach(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse1")
                initValues.setInfbel1(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse2")
                initValues.setInfbel2(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse3")
                initValues.setInfbel3(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse4")
                initValues.setInfbel4(attribs.value("value").toFloat());
            break;
        case Bagrovwerte :
            if (attribs.value("key") == "Dachflaechen")
                initValues.setBagdach(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse1")
                initValues.setBagbel1(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse2")
                initValues.setBagbel2(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse3")
                initValues.setBagbel3(attribs.value("value").toFloat());
            else if (attribs.value("key") == "Belaglsklasse4")
                initValues.setBagbel4(attribs.value("value").toFloat());
            break;
        case Nachkomma :
            if (attribs.value("key") == "R")
                initValues.setDecR(attribs.value("value").toInt());
            else if (attribs.value("key") == "ROW")
                initValues.setDecROW(attribs.value("value").toInt());
            else if (attribs.value("key") == "RI")
                initValues.setDecRI(attribs.value("value").toInt());
            else if (attribs.value("key") == "RVOL")
                initValues.setDecRVOL(attribs.value("value").toInt());
            else if (attribs.value("key") == "ROWVOL")
                initValues.setDecROWVOL(attribs.value("value").toInt());
            else if (attribs.value("key") == "RIVOL")
                initValues.setDecRIVOL(attribs.value("value").toInt());
            else if (attribs.value("key") == "FLAECHE")
                initValues.setDecFLAECHE(attribs.value("value").toInt());
            else if (attribs.value("key") == "VERDUNSTUNG")
                initValues.setDecVERDUNSTUNG(attribs.value("value").toInt());
            break;
        case Diverse :
            if (attribs.value("key") == "BERtoZero")
                initValues.setBERtoZero(attribs.value("value")=="true");
            else if (attribs.value("key") == "NIEDKORRF")
                initValues.setNiedKorrF(attribs.value("value").toFloat());
            break;
        case GewVerd :
            gewVerdEntry(attribs);
            break;
        case PotVerd :
            potVerdEntry(attribs);
            break;
        }
    }
    return true;
}

void SaxHandler::gewVerdEntry(const QXmlAttributes &attribs)
{
    initValues.putToHash(attribs.value("bezirke"), attribs.value("eg").toInt(), 13);
}



void SaxHandler::potVerdEntry(const QXmlAttributes &attribs)
{
    QString etpString = attribs.value("etp");
    if (etpString.length() > 0) {
        initValues.putToHash(attribs.value("bezirke"), etpString.toInt(), 11);
    }

    QString etpsString = attribs.value("etps");
    if (etpsString.length() > 0) {
        initValues.putToHash(attribs.value("bezirke"), etpsString.toInt(), 12);
    }
}

bool SaxHandler::fatalError(const QXmlParseException &exception)
{
    return false;
}
