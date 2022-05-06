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
 
#include "dbaseWriter.h"
#include <QTextStream>
DbaseWriter::DbaseWriter(QString &file,
                         InitValues &initValues):
  fileName(file), 
  recNum(0)
{

    //Felder mit Namen, Typ, Nachkommastellen
    fields[0].set("CODE", "C", 0);
    fields[1].set("R", "N", initValues.getDecR());
    fields[2].set("ROW", "N", initValues.getDecROW());
    fields[3].set("RI", "N", initValues.getDecRI());
    fields[4].set("RVOL", "N", initValues.getDecRVOL());
    fields[5].set("ROWVOL", "N", initValues.getDecROWVOL());
    fields[6].set("RIVOL", "N", initValues.getDecRIVOL());
    fields[7].set("FLAECHE", "N", initValues.getDecFLAECHE());
    fields[8].set("VERDUNSTUN", "N", initValues.getDecVERDUNSTUNG());

    this->date = QDateTime::currentDateTime().date();
        
    hash = QHash<QString, int>();
    for(int i=0; i< countFields; i++){            
            //name
            QString name = fields[i].getName();
            hash[name] = i;
    }                                
}

QString DbaseWriter::getError()
{
      return error;  
}

bool DbaseWriter::write()
{
     QByteArray data;
     data.resize(lengthOfHeader);
     data[0] = (quint8)0x03;
        
     int year = date.year();
     int year2 = year % 100;
     if(year > 2000){
                year2 += 100;
     }
     int month = date.month();
     int day = date.day();
        
     data[1] = (quint8)year2;//jahr
     data[2] = (quint8)month;//monat
     data[3] = (quint8)day;//tag
        
     data[4] = (quint8)recNum;
     data[5] = (quint8)(recNum >> 8);
     data[6] = (quint8)(recNum >> 16);
     data[7] = (quint8)(recNum >> 24);
        
     data[8] = (quint8)lengthOfHeader;
     data[9] = (quint8)(lengthOfHeader >> 8);
     
     lengthOfEachRecord = 1;
     for(int i=0; i< countFields; i++){
             lengthOfEachRecord += fields[i].getFieldLength();        
     }
        
     data[10] = (quint8)lengthOfEachRecord;
     data[11] = (quint8)(lengthOfEachRecord >> 8);

     for(int i=12; i<=28; i++){
                data[i] = (quint8)0x00;        
     }
        
     data[29] = (quint8)0x57; //wie in der input-date ???
     data[30] = (quint8)0x00;
     data[31] = (quint8)0x00;
        
     int headCounter = 32;
     for(int i=0; i< countFields; i++){
                //name
                QString name = fields[i].getName();
                QString typ = fields[i].getType();
                for(int k=0; k<11; k++){
                      if(k<name.size()){
                              data[headCounter++]= (quint8)(name[k].toAscii());         
                      }else{
                              data[headCounter++]= (quint8)0x00;  
                      }  
                }
                data[headCounter++] = (quint8)(typ[0].toAscii());
                for(int k=12; k<16; k++){
                        data[headCounter++]= (quint8)0x00;
                }
                data[headCounter++]=(quint8)fields[i].getFieldLength();                  
                data[headCounter++]=(quint8)fields[i].getDecimalCount();
                for(int k=18; k<32; k++){
                        data[headCounter++]= (quint8)0x00;
                }
     }
     data[headCounter++]= (quint8)0x0D; 
        
     for(int rec=0; rec< recNum; rec++){
             QVector<QString> vec = record.at(rec);
             data.append(QChar(0x20));
             for(int field=0; field<countFields; field++){
                     int fieldLength = fields[field].getFieldLength();
                     if(fields[field].getDecimalCount() > 0) {
                         QString str = vec.at(field);
                         QStringList strlist = str.split(".");
                         int frontLength = fieldLength - 1 - fields[field].getDecimalCount();

                         if(strlist.at(0).contains('-')) {
                             data.append(QString("-"));
                             data.append(strlist.at(0).right(strlist.at(0).length() - 1).rightJustified(frontLength-1, QChar(0x30)));
                         } else {

                             data.append(strlist.at(0).rightJustified(frontLength, QChar(0x30)));
                         }
                         data.append(".");
                         data.append(strlist.at(1).leftJustified(fields[field].getDecimalCount(), QChar(0x30)));
                     } else {
                         data.append(vec.at(field).rightJustified(fieldLength, QChar(0x30)));
                     }

             }        
     }
     
     data.append(QChar(0x1A));
     
     QFile o_file(fileName);
     if (!o_file.open(QIODevice::WriteOnly)){
        error = "kann Out-Datei: '" + fileName + "' nicht oeffnen\n Grund: " + o_file.error();
        return false;
     }
     o_file.write(data);
     o_file.close();
     return true;
}

void DbaseWriter::addRecord()
{
     QVector<QString> v(countFields);
     record.append(v);
     recNum ++;
}

void DbaseWriter::setRecordField(int num, QString value)
{
     ((record.last()))[num] = QString(value);
     if(value.size() > fields[num].getFieldLength()){
             fields[num].setFieldLength(value.size());        
     }
}

void DbaseWriter::setRecordField(QString name, QString value)
{
     if(hash.contains(name)){
           int field = hash[name];
           setRecordField(field, value);                         
     }
}

void DbaseWriter::setRecordField(int num, float value)
{    
     int decimalCount = fields[num].getDecimalCount();
     
     //round
     value *= pow( 10, decimalCount);
     value = round(value);
     value *= pow(10, -decimalCount);
          
     QString valueStr;
     valueStr.setNum(value, 'f', decimalCount);
     setRecordField(num, valueStr);
}

void DbaseWriter::setRecordField(QString name, float value)
{
     if(hash.contains(name)){
           int field = hash[name];
           setRecordField(field, value);                         
     }
}

