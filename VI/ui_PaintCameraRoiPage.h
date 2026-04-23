/********************************************************************************
** Form generated from reading UI file 'PaintCameraRoiPagekFfaxk.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef PAINTCAMERAROIPAGEKFFAXK_H
#define PAINTCAMERAROIPAGEKFFAXK_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaintCameraRoiPage
{
public:
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QWidget *PaintCameraRoiPage)
    {
        if (PaintCameraRoiPage->objectName().isEmpty())
            PaintCameraRoiPage->setObjectName(QString::fromUtf8("PaintCameraRoiPage"));
        PaintCameraRoiPage->resize(503, 390);
        PaintCameraRoiPage->setFocusPolicy(Qt::NoFocus);
        gridLayout = new QGridLayout(PaintCameraRoiPage);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(PaintCameraRoiPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(PaintCameraRoiPage);

        QMetaObject::connectSlotsByName(PaintCameraRoiPage);
    } // setupUi

    void retranslateUi(QWidget *PaintCameraRoiPage)
    {
        PaintCameraRoiPage->setWindowTitle(QApplication::translate("PaintCameraRoiPage", "PaintCameraRoiPage", nullptr));
        label->setText(QApplication::translate("PaintCameraRoiPage", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PaintCameraRoiPage: public Ui_PaintCameraRoiPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // PAINTCAMERAROIPAGEKFFAXK_H
