/********************************************************************************
** Form generated from reading UI file 'MarkTheErrorNXDKdh.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MARKTHEERRORNXDKDH_H
#define MARKTHEERRORNXDKDH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MarkTheErrorClass
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *frame;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QLabel *frame_2;
    QGridLayout *gridLayout_3;
    QLabel *label_2;

    void setupUi(QWidget *MarkTheErrorClass)
    {
        if (MarkTheErrorClass->objectName().isEmpty())
            MarkTheErrorClass->setObjectName(QString::fromUtf8("MarkTheErrorClass"));
        MarkTheErrorClass->resize(730, 630);
        MarkTheErrorClass->setMinimumSize(QSize(0, 0));
        MarkTheErrorClass->setMaximumSize(QSize(730, 630));
        gridLayout_4 = new QGridLayout(MarkTheErrorClass);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(MarkTheErrorClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QLabel(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMinimumSize(QSize(350, 600));
        frame->setMaximumSize(QSize(1000, 1000));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(350, 600));
        label->setMaximumSize(QSize(1000, 1000));
        label->setContextMenuPolicy(Qt::DefaultContextMenu);
        label->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
"font: 11pt \"\345\256\213\344\275\223\";\n"
"qproperty-alignment: 'AlignTop | AlignLeft';\n"
"qproperty-wordWrap: true;\n"
"background-color: transparent;"));
        label->setFrameShape(QFrame::Box);
        label->setLineWidth(0);
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        label->setWordWrap(true);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        frame_2 = new QLabel(groupBox);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(350, 600));
        frame_2->setMaximumSize(QSize(1000, 1000));
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(frame_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(350, 600));
        label_2->setMaximumSize(QSize(1000, 1000));
        label_2->setContextMenuPolicy(Qt::DefaultContextMenu);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
"font: 11pt \"\345\256\213\344\275\223\";\n"
"qproperty-alignment: 'AlignTop | AlignLeft';\n"
"qproperty-wordWrap: true;\n"
"background-color: transparent;"));
        label_2->setFrameShape(QFrame::Box);
        label_2->setLineWidth(0);
        label_2->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        gridLayout_3->addWidget(label_2, 0, 0, 1, 1);


        gridLayout->addWidget(frame_2, 0, 1, 1, 1);


        gridLayout_4->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(MarkTheErrorClass);

        QMetaObject::connectSlotsByName(MarkTheErrorClass);
    } // setupUi

    void retranslateUi(QWidget *MarkTheErrorClass)
    {
        MarkTheErrorClass->setWindowTitle(QApplication::translate("MarkTheErrorClass", "\346\230\276\347\244\272\351\224\231\350\257\257\346\240\207\350\256\260\347\202\271", nullptr));
        groupBox->setTitle(QString());
        label->setText(QString());
        label_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MarkTheErrorClass: public Ui_MarkTheErrorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MARKTHEERRORNXDKDH_H
