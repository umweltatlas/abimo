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

#ifndef DBASEREADER_H
#define DBASEREADER_H
#include <QFile>
#include <QDate>
#include <QBuffer>
#include <QHash>
#include "dbaseField.h"

class DbaseReader
{
public:
    DbaseReader(const QString&);
    ~DbaseReader();
    bool read();

    QString getVersion();
    QString getLanguageDriver();
    QDate getDate();
    int getNumberOfRecords();
    int getLengthOfHeader();
    int getLengthOfEachRecord();
    int getCountFields();
    QString getRecord(int num, int field);
    QString getRecord(int num, const QString & name);
    QString getError();
    bool isAbimoFile();


private:

    //VARIABLES:
    ////////////
    QFile file;
    QString version;
    QString languageDriver;
    QDate date;
    QHash<QString, int> hash;
    QString error;

    QString* vals;

    //count of records in file
    int numberOfRecords;

    //length of the header in byte
    int lengthOfHeader;

    //length of a record in byte
    int lengthOfEachRecord;

    //count of fields
    int countFields;

    //FUNCTIONS:
    ////////////

    //1 byte unsigned give the version
    QString checkVersion(quint8);

    //1 byte unsigned give the Language Driver (code page)
    QString checkLanguageDriver(quint8 i_byte);

    //3 byte unsigned char give the date of last edit
    QDate checkDate(quint8 i_year, quint8 i_month, quint8 i_day);

    //32 bit unsigned char to int
    int check32(quint8 i1, quint8 i2, quint8 i3, quint8 i4);

    //16 bit unsigned char to int
    int check16(quint8 i1, quint8 i2);

    //compute the count of fields
    int computeCountFields(int);


};
#endif
