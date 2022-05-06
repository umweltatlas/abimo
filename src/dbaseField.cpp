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

#include "dbaseField.h"

DbaseField::DbaseField()
{
    fieldLength = 0;
    decimalCount = 0;
}

DbaseField::DbaseField(QByteArray array)
{
    this->name = QString(array.left(10));
    this->type = QString((QChar)array[11]);
    this->fieldLength = (quint8)array[16];
    this->decimalCount = (quint8)array[17];
}

DbaseField::DbaseField(QString name, QString type, int decimalCount)
{
    this->name = QString(name);
    this->type = QString(type);
    this->fieldLength = 0;
    this->decimalCount = decimalCount;
}

void DbaseField::set(QString name, QString type, int decimalCount)
{
    this->name = QString(name);
    this->type = QString(type);
    this->fieldLength = 0;
    this->decimalCount = decimalCount;
}

DbaseField::~DbaseField()
{
}

QString DbaseField::getName()
{
    return name;
}

QString DbaseField::getType()
{
    return type;
}

int DbaseField::getFieldLength()
{
    return fieldLength;
}

void DbaseField::setFieldLength(int fl)
{
    fieldLength = fl;
}

int DbaseField::getDecimalCount()
{
    return decimalCount;
}
