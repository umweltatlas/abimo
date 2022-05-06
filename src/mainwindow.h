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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtGui>
#include <QLabel>
#include <QXmlSimpleReader>
#include "saxhandler.h"
#include "dbaseReader.h"
#include "calculation.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QApplication*);
    ~MainWindow();

private slots:
    void processEvent(int, QString);
    void about();
    void computeFile();
    void userCancel();

private:
    void setText(QString);
    void critical(QString);
    QAction *openAct;
    QAction *aboutAct;
    QLabel *textfield;
    QProgressDialog * progress;
    bool userStop;
    Calculation* calc;
    QApplication* app;
    QString folder;
    QWidget *w;
};
#endif


