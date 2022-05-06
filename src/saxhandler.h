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

#ifndef SAXHANDLER_H
#define SAXHANDLER_H

#include <QXmlDefaultHandler>
#include <QXmlAttributes>
#include <QXmlParseException>
#include "initvalues.h"

#define Infiltrationsfaktoren 1
#define Bagrovwerte 2
#define Nachkomma 3
#define Diverse 4
#define GewVerd 5
#define PotVerd 6

class SaxHandler : public QXmlDefaultHandler
{

public:
    SaxHandler(InitValues &initValue);
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attribs);
    bool fatalError(const QXmlParseException &exception);

private:
    int state;
    InitValues &initValues;
    void gewVerdEntry(const QXmlAttributes &attribs);
    void potVerdEntry(const QXmlAttributes &attribs);

};

#endif
