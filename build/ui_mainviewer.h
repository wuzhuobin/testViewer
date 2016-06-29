/********************************************************************************
** Form generated from reading UI file 'mainviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEWER_H
#define UI_MAINVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "qvtkwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainViewerClass
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QVTKWidget *viewerWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainViewerClass)
    {
        if (MainViewerClass->objectName().isEmpty())
            MainViewerClass->setObjectName(QStringLiteral("MainViewerClass"));
        MainViewerClass->resize(600, 400);
        centralWidget = new QWidget(MainViewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        viewerWidget = new QVTKWidget(centralWidget);
        viewerWidget->setObjectName(QStringLiteral("viewerWidget"));

        horizontalLayout->addWidget(viewerWidget);

        MainViewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainViewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        MainViewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainViewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainViewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainViewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainViewerClass->setStatusBar(statusBar);

        retranslateUi(MainViewerClass);

        QMetaObject::connectSlotsByName(MainViewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainViewerClass)
    {
        MainViewerClass->setWindowTitle(QApplication::translate("MainViewerClass", "MainViewer", 0));
    } // retranslateUi

};

namespace Ui {
    class MainViewerClass: public Ui_MainViewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEWER_H
