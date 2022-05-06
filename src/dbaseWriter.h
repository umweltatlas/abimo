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

#ifndef DBASEWRITER_H
#define DBASEWRITER_H
#include <QString>
#include <QDate>
#include <QByteArray>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QHash>
#include <math.h>
#include <QStringList>
#include "dbaseField.h"
#include "initvalues.h"

const int countFields = 9;
const int lengthOfHeader = countFields*32 + 32 + 1;

class DbaseWriter
{
public:
    DbaseWriter(QString &file,
                InitValues &initValues);
    bool write();

    void addRecord();
    void setRecordField(int num, QString value);
    void setRecordField(QString name, QString value);
    void setRecordField(int num, float value);
    void setRecordField(QString name, float value);
    QString getError();

private:

    //VARIABLES:
    ////////////
    QString fileName;
    QVector< QVector<QString> > record;
    QDate date;
    QHash<QString, int> hash;
    QString error;
    int lengthOfEachRecord;
    int recNum;

    DbaseField fields[countFields];


};
#endif
