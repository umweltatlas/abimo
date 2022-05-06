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

#include "dbaseReader.h"

DbaseReader::DbaseReader(const QString &i_file):
    file(i_file),
    vals(0),
    numberOfRecords(0),
    lengthOfHeader(0),
    lengthOfEachRecord(0),
    countFields(0)

{}

DbaseReader::~DbaseReader()
{
    if (vals != 0) {
        delete[] vals;
    }

}

QString DbaseReader::getError()
{
    return error;
}

bool DbaseReader::isAbimoFile()
{
    return (
               hash.contains("NUTZUNG") &&
               hash.contains("CODE") &&
               hash.contains("REGENJA") &&
               hash.contains("REGENSO") &&
               hash.contains("FLUR") &&
               hash.contains("TYP") &&
               hash.contains("FELD_30") &&
               hash.contains("FELD_150") &&
               hash.contains("BEZIRK") &&
               hash.contains("PROBAU") &&
               hash.contains("PROVGU") &&
               hash.contains("VGSTRASSE") &&
               hash.contains("KAN_BEB") &&
               hash.contains("KAN_VGU") &&
               hash.contains("KAN_STR") &&
               hash.contains("BELAG1") &&
               hash.contains("BELAG2") &&
               hash.contains("BELAG3") &&
               hash.contains("BELAG4") &&
               hash.contains("STR_BELAG1") &&
               hash.contains("STR_BELAG2") &&
               hash.contains("STR_BELAG3") &&
               hash.contains("STR_BELAG4") &&
               hash.contains("FLGES") &&
               hash.contains("STR_FLGES") &&
               hash.contains("NUTZUNG")
           );

}

bool DbaseReader::read()
{

    if (!file.open(QIODevice::ReadOnly)) {
        error = "kann die Datei nicht oeffnen\n" + file.error();
        return false;
    }

    if (file.size() < 32) {
        error = "Datei unbekannten Formats.";
        return false;
    }

    QByteArray info = file.QIODevice::read(32);
    version =(checkVersion((quint8)info[0]));
    date = (checkDate((quint8)info[1], (quint8)info[2], (quint8)info[3]));
    numberOfRecords = check32((quint8)info[4],(quint8)info[5],(quint8)info[6],(quint8)info[7]);
    lengthOfHeader = check16((quint8)info[8],(quint8)info[9]);
    lengthOfEachRecord = check16((quint8)info[10],(quint8)info[11]);
    countFields = computeCountFields(lengthOfHeader);

    if (lengthOfHeader + ( numberOfRecords * lengthOfEachRecord ) + 1 != file.size()) {
        QString s;
        s.setNum(file.size());
        QString s1;
        s1.setNum(lengthOfHeader + ( numberOfRecords * lengthOfEachRecord ) + 1);
        error = "Datei unbekannten Formats, falsche Grösse.\nSoll:" + s1 + "\nIst: " + s;
        return false;
    }

    if (numberOfRecords <= 0) {
        error = "keine Records in der datei vorhanden.";
        return false;
    }

    if (lengthOfEachRecord <= 1 || lengthOfEachRecord > 4000) {
        error = "Grösse der records ist zu klein oder zu gross.";
        return false;
    }

    if (countFields < 1) {
        error = "keine Felder gefunden.";
        return false;
    }

    //line[12], line[13] reserved - filled with '00h'
    //line[14] - transaction flag
    //line[15] - encryption flag
    //line[16 to 19] - free record thread reserved for LAN
    //line[20 to 27] - reserved for multiuser dbase
    //line[28] MDX-flag
    languageDriver = (checkLanguageDriver((quint8)info[29]));
    //line[30 - 31] reserved

    //rest of header are field information
    DbaseField fields[countFields];

    for (int i = 0; i<countFields; i++) {
        fields[i] = DbaseField(file.QIODevice::read(32));
        hash[fields[i].getName()] = i;
    }

    //Terminator
    file.QIODevice::read(2);

    QByteArray arr = file.QIODevice::read(lengthOfEachRecord * numberOfRecords);

    QBuffer buffer(&arr);
    buffer.open(QIODevice::ReadOnly);

    vals = new QString[numberOfRecords*countFields];

    for (int i=0; i< numberOfRecords; i++) {
        for (int j =0; j<countFields;j++) {
            int l = fields[j].getFieldLength();
            QString strg = buffer.read(l);
            strg = strg.trimmed();
            if (strg.size()<=0) {
                strg = "0";
            }
            vals[i*countFields + j] = strg;
        }
        buffer.read(1);
    }

    buffer.close();
    file.close();
    return true;
}

QString DbaseReader::getRecord(int num, const QString & name)
{
    if (!hash.contains(name))return 0;
    int field = hash[name];
    return getRecord(num, field);
}


QString DbaseReader::getRecord(int num, int field)
{
    if (num >= numberOfRecords || field >= countFields) {
        return 0;
    }
    return vals[num * countFields + field];
}

int DbaseReader::getCountFields()
{
    return countFields;
}

QDate DbaseReader::getDate()
{
    return date;
}

QString DbaseReader::getLanguageDriver()
{
    return languageDriver;
}

QString DbaseReader::getVersion()
{
    return version;
}

int DbaseReader::getNumberOfRecords()
{
    return numberOfRecords;
}

int DbaseReader::getLengthOfHeader()
{
    return lengthOfHeader;
}

int DbaseReader::getLengthOfEachRecord()
{
    return lengthOfEachRecord;
}

int DbaseReader::check16(quint8 i1, quint8 i2)
{
    int result = 0;
    result = (quint8)i1;
    result += ((quint8)i2) << 8;
    return result;
}

int DbaseReader::check32(quint8 i1, quint8 i2, quint8 i3, quint8 i4)
{
    int result = 0;
    result = (quint8)i1;
    result += ((quint8)i2) << 8;
    result += ((quint8)i3) << 16;
    result += ((quint8)i4) << 24;
    return result;
}

QDate DbaseReader::checkDate(quint8 i_year, quint8 i_month, quint8 i_day)
{
    int year = (int)i_year;
    if (year >= 100) {
        year = 1900 + year;
    }
    QDate d(year, (int)i_month, (int)i_day);
    return d;
}

QString DbaseReader::checkVersion(quint8 i_byte)
{
    switch (i_byte) {
    case 0x02 :
        return "FoxBase";
    case 0x03 :
        return "File without DBT (dBASE III w/o memo file)";
    case 0x04 :
        return "dBASE IV w/o memo file";
    case 0x05 :
        return "dBASE V w/o memo file";
    case 0x30 :
        return "Visual FoxPro w. DBC";
    case 0x31 :
        return "Visual FoxPro w. AutoIncrement field";
    case 0x43 :
        return ".dbv memo var size (Flagship)";
    case 0x7B :
        return "dBASE IV with memo";
    case 0x83 :
        return "dBASE III+ with memo file";
    case 0x8B :
        return "dBASE IV w. memo";
    case 0x8E :
        return "dBASE IV w. SQL table";
    case 0xB3 :
        return ".dbv and .dbt memo (Flagship)";
    case 0xE5 :
        return "Clipper SIX driver w. SMT memo file";
    case 0xF5 :
        return "FoxPro w. memo file";
    case 0xFB :
        return "FoxPro ???";
    }
    return "unknown version";
}

QString DbaseReader::checkLanguageDriver(quint8 i_byte)
{
    switch (i_byte) {
    case 0x01 :
        return "DOS USA code page 437";
    case 0x02 :
        return "DOS Multilingual code page 850";
    case 0x03 :
        return "Windows ANSI code page 1252";
    case 0x04 :
        return "Standard Macintosh";
    case 0x64 :
        return "EE MS-DOS code page 852";
    case 0x65 :
        return "Nordic MS-DOS code page 865";
    case 0x66 :
        return "Russian MS-DOS code page 866";
    case 0x67 :
        return "Icelandic MS-DOS";
    case 0x68 :
        return "Kamenicky (Czech) MS-DOS";
    case 0x69 :
        return "Mazovia (Polish) MS-DOS";
    case 0x6A :
        return "Greek MS-DOS (437G)";
    case 0x6B :
        return "Turkish MS-DOS";
    case 0x96 :
        return "Russian Macintosh";
    case 0x97 :
        return "Eastern European Macintosh";
    case 0x98 :
        return "Greek Macintosh";
    case 0xC8 :
        return "Windows EE code page 1250";
    case 0xC9 :
        return "Russian Windows";
    case 0xCA :
        return "Turkish Windows";
    case 0xCB :
        return "Greek Windows";
    }
    return "unknown language driver";
}

int DbaseReader::computeCountFields(int headerLength)
{
    //32 byte file information, 1 byte Terminator (0Dh), und jedes Feld hat 32 byte im header
    return (headerLength - 32 - 1)/32;
}
