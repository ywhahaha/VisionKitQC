/********************************************************************************
** Form generated from reading UI file 'MainRoiPageQoZHLK.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINROIPAGEQOZHLK_H
#define MAINROIPAGEQOZHLK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainRoiPage
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_2;

    void setupUi(QWidget *MainRoiPage)
    {
        if (MainRoiPage->objectName().isEmpty())
            MainRoiPage->setObjectName(QString::fromUtf8("MainRoiPage"));
        MainRoiPage->resize(400, 300);
        gridLayout = new QGridLayout(MainRoiPage);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(MainRoiPage);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setCheckable(true);

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(MainRoiPage);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setCheckable(true);

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(MainRoiPage);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setCheckable(true);

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);

        stackedWidget = new QStackedWidget(MainRoiPage);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);


        retranslateUi(MainRoiPage);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainRoiPage);
    } // setupUi

    void retranslateUi(QWidget *MainRoiPage)
    {
        MainRoiPage->setWindowTitle(QApplication::translate("MainRoiPage", "\347\273\230\345\210\266\347\233\270\346\234\272ROI\345\214\272\345\237\237", nullptr));
        pushButton->setText(QApplication::translate("MainRoiPage", "\347\233\270\346\234\2721", nullptr));
        pushButton_2->setText(QApplication::translate("MainRoiPage", "\347\233\270\346\234\2722", nullptr));
        pushButton_3->setText(QApplication::translate("MainRoiPage", "\347\233\270\346\234\2723", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainRoiPage: public Ui_MainRoiPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINROIPAGEQOZHLK_H
