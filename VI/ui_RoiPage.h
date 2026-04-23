/********************************************************************************
** Form generated from reading UI file 'RoiPagegMStBd.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ROIPAGEGMSTBD_H
#define ROIPAGEGMSTBD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_7;
    QHBoxLayout *horizontalLayout_26;
    QLabel *label_26;
    QSpinBox *spinBox_26;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label_27;
    QSpinBox *spinBox_27;
    QPushButton *pushButton_5;
    QHBoxLayout *horizontalLayout_25;
    QLabel *label_25;
    QSpinBox *spinBox_25;
    QHBoxLayout *horizontalLayout_28;
    QLabel *label_28;
    QSpinBox *spinBox_28;
    QGroupBox *groupBox_8;
    QGridLayout *gridLayout_8;
    QHBoxLayout *horizontalLayout_32;
    QLabel *label_32;
    QSpinBox *spinBox_32;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label_29;
    QSpinBox *spinBox_29;
    QHBoxLayout *horizontalLayout_30;
    QLabel *label_30;
    QSpinBox *spinBox_30;
    QHBoxLayout *horizontalLayout_31;
    QLabel *label_31;
    QSpinBox *spinBox_31;
    QPushButton *pushButton_6;
    QGroupBox *groupBox_9;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_34;
    QLabel *label_34;
    QSpinBox *spinBox_34;
    QHBoxLayout *horizontalLayout_35;
    QLabel *label_35;
    QSpinBox *spinBox_35;
    QHBoxLayout *horizontalLayout_36;
    QLabel *label_36;
    QSpinBox *spinBox_36;
    QHBoxLayout *horizontalLayout_33;
    QLabel *label_33;
    QSpinBox *spinBox_33;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QLabel *label_3;
    QButtonGroup *buttonGroup;

    void setupUi(QWidget *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QString::fromUtf8("ControlPanel"));
        ControlPanel->setEnabled(true);
        ControlPanel->resize(159, 553);
        gridLayout_2 = new QGridLayout(ControlPanel);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        groupBox_7 = new QGroupBox(ControlPanel);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setMinimumSize(QSize(139, 167));
        groupBox_7->setMaximumSize(QSize(139, 167));
        groupBox_7->setFlat(false);
        gridLayout_7 = new QGridLayout(groupBox_7);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QString::fromUtf8("horizontalLayout_26"));
        label_26 = new QLabel(groupBox_7);
        label_26->setObjectName(QString::fromUtf8("label_26"));

        horizontalLayout_26->addWidget(label_26);

        spinBox_26 = new QSpinBox(groupBox_7);
        spinBox_26->setObjectName(QString::fromUtf8("spinBox_26"));
        spinBox_26->setEnabled(false);
        spinBox_26->setToolTipDuration(-1);
        spinBox_26->setMaximum(3072);
        spinBox_26->setSingleStep(2);

        horizontalLayout_26->addWidget(spinBox_26);


        gridLayout_7->addLayout(horizontalLayout_26, 0, 0, 1, 1);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QString::fromUtf8("horizontalLayout_27"));
        label_27 = new QLabel(groupBox_7);
        label_27->setObjectName(QString::fromUtf8("label_27"));

        horizontalLayout_27->addWidget(label_27);

        spinBox_27 = new QSpinBox(groupBox_7);
        spinBox_27->setObjectName(QString::fromUtf8("spinBox_27"));
        spinBox_27->setEnabled(false);
        spinBox_27->setMinimum(188);
        spinBox_27->setMaximum(3072);
        spinBox_27->setSingleStep(1);
        spinBox_27->setValue(376);

        horizontalLayout_27->addWidget(spinBox_27);


        gridLayout_7->addLayout(horizontalLayout_27, 2, 0, 1, 1);

        pushButton_5 = new QPushButton(groupBox_7);
        buttonGroup = new QButtonGroup(ControlPanel);
        buttonGroup->setObjectName(QString::fromUtf8("buttonGroup"));
        buttonGroup->addButton(pushButton_5);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setEnabled(false);
        pushButton_5->setCheckable(true);
        pushButton_5->setChecked(false);
        pushButton_5->setAutoExclusive(true);

        gridLayout_7->addWidget(pushButton_5, 4, 0, 1, 1);

        horizontalLayout_25 = new QHBoxLayout();
        horizontalLayout_25->setObjectName(QString::fromUtf8("horizontalLayout_25"));
        label_25 = new QLabel(groupBox_7);
        label_25->setObjectName(QString::fromUtf8("label_25"));

        horizontalLayout_25->addWidget(label_25);

        spinBox_25 = new QSpinBox(groupBox_7);
        spinBox_25->setObjectName(QString::fromUtf8("spinBox_25"));
        spinBox_25->setEnabled(false);
        spinBox_25->setMaximum(2048);
        spinBox_25->setSingleStep(2);

        horizontalLayout_25->addWidget(spinBox_25);


        gridLayout_7->addLayout(horizontalLayout_25, 1, 0, 1, 1);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName(QString::fromUtf8("horizontalLayout_28"));
        label_28 = new QLabel(groupBox_7);
        label_28->setObjectName(QString::fromUtf8("label_28"));

        horizontalLayout_28->addWidget(label_28);

        spinBox_28 = new QSpinBox(groupBox_7);
        spinBox_28->setObjectName(QString::fromUtf8("spinBox_28"));
        spinBox_28->setEnabled(false);
        spinBox_28->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        spinBox_28->setMinimum(160);
        spinBox_28->setMaximum(2048);
        spinBox_28->setSingleStep(1);
        spinBox_28->setValue(320);

        horizontalLayout_28->addWidget(spinBox_28);


        gridLayout_7->addLayout(horizontalLayout_28, 3, 0, 1, 1);


        verticalLayout->addWidget(groupBox_7);

        groupBox_8 = new QGroupBox(ControlPanel);
        groupBox_8->setObjectName(QString::fromUtf8("groupBox_8"));
        groupBox_8->setMinimumSize(QSize(139, 167));
        groupBox_8->setMaximumSize(QSize(139, 167));
        gridLayout_8 = new QGridLayout(groupBox_8);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        horizontalLayout_32 = new QHBoxLayout();
        horizontalLayout_32->setObjectName(QString::fromUtf8("horizontalLayout_32"));
        label_32 = new QLabel(groupBox_8);
        label_32->setObjectName(QString::fromUtf8("label_32"));

        horizontalLayout_32->addWidget(label_32);

        spinBox_32 = new QSpinBox(groupBox_8);
        spinBox_32->setObjectName(QString::fromUtf8("spinBox_32"));
        spinBox_32->setEnabled(false);
        spinBox_32->setMinimum(160);
        spinBox_32->setMaximum(2048);
        spinBox_32->setSingleStep(1);

        horizontalLayout_32->addWidget(spinBox_32);


        gridLayout_8->addLayout(horizontalLayout_32, 3, 0, 1, 1);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setObjectName(QString::fromUtf8("horizontalLayout_29"));
        label_29 = new QLabel(groupBox_8);
        label_29->setObjectName(QString::fromUtf8("label_29"));

        horizontalLayout_29->addWidget(label_29);

        spinBox_29 = new QSpinBox(groupBox_8);
        spinBox_29->setObjectName(QString::fromUtf8("spinBox_29"));
        spinBox_29->setEnabled(false);
        spinBox_29->setMaximum(3072);
        spinBox_29->setSingleStep(2);

        horizontalLayout_29->addWidget(spinBox_29);


        gridLayout_8->addLayout(horizontalLayout_29, 0, 0, 1, 1);

        horizontalLayout_30 = new QHBoxLayout();
        horizontalLayout_30->setObjectName(QString::fromUtf8("horizontalLayout_30"));
        label_30 = new QLabel(groupBox_8);
        label_30->setObjectName(QString::fromUtf8("label_30"));

        horizontalLayout_30->addWidget(label_30);

        spinBox_30 = new QSpinBox(groupBox_8);
        spinBox_30->setObjectName(QString::fromUtf8("spinBox_30"));
        spinBox_30->setEnabled(false);
        spinBox_30->setMaximum(2048);
        spinBox_30->setSingleStep(2);

        horizontalLayout_30->addWidget(spinBox_30);


        gridLayout_8->addLayout(horizontalLayout_30, 1, 0, 1, 1);

        horizontalLayout_31 = new QHBoxLayout();
        horizontalLayout_31->setObjectName(QString::fromUtf8("horizontalLayout_31"));
        label_31 = new QLabel(groupBox_8);
        label_31->setObjectName(QString::fromUtf8("label_31"));

        horizontalLayout_31->addWidget(label_31);

        spinBox_31 = new QSpinBox(groupBox_8);
        spinBox_31->setObjectName(QString::fromUtf8("spinBox_31"));
        spinBox_31->setEnabled(false);
        spinBox_31->setMinimum(188);
        spinBox_31->setMaximum(3072);
        spinBox_31->setSingleStep(1);

        horizontalLayout_31->addWidget(spinBox_31);


        gridLayout_8->addLayout(horizontalLayout_31, 2, 0, 1, 1);

        pushButton_6 = new QPushButton(groupBox_8);
        buttonGroup->addButton(pushButton_6);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setEnabled(false);
        pushButton_6->setCheckable(true);
        pushButton_6->setAutoExclusive(true);

        gridLayout_8->addWidget(pushButton_6, 4, 0, 1, 1);


        verticalLayout->addWidget(groupBox_8);

        groupBox_9 = new QGroupBox(ControlPanel);
        groupBox_9->setObjectName(QString::fromUtf8("groupBox_9"));
        groupBox_9->setMinimumSize(QSize(139, 167));
        groupBox_9->setMaximumSize(QSize(139, 167));
        gridLayout = new QGridLayout(groupBox_9);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_34 = new QHBoxLayout();
        horizontalLayout_34->setObjectName(QString::fromUtf8("horizontalLayout_34"));
        label_34 = new QLabel(groupBox_9);
        label_34->setObjectName(QString::fromUtf8("label_34"));

        horizontalLayout_34->addWidget(label_34);

        spinBox_34 = new QSpinBox(groupBox_9);
        spinBox_34->setObjectName(QString::fromUtf8("spinBox_34"));
        spinBox_34->setEnabled(false);
        spinBox_34->setMaximum(3072);
        spinBox_34->setSingleStep(2);

        horizontalLayout_34->addWidget(spinBox_34);


        gridLayout->addLayout(horizontalLayout_34, 0, 0, 1, 1);

        horizontalLayout_35 = new QHBoxLayout();
        horizontalLayout_35->setObjectName(QString::fromUtf8("horizontalLayout_35"));
        label_35 = new QLabel(groupBox_9);
        label_35->setObjectName(QString::fromUtf8("label_35"));

        horizontalLayout_35->addWidget(label_35);

        spinBox_35 = new QSpinBox(groupBox_9);
        spinBox_35->setObjectName(QString::fromUtf8("spinBox_35"));
        spinBox_35->setEnabled(false);
        spinBox_35->setMaximum(2048);
        spinBox_35->setSingleStep(2);

        horizontalLayout_35->addWidget(spinBox_35);


        gridLayout->addLayout(horizontalLayout_35, 1, 0, 1, 1);

        horizontalLayout_36 = new QHBoxLayout();
        horizontalLayout_36->setObjectName(QString::fromUtf8("horizontalLayout_36"));
        label_36 = new QLabel(groupBox_9);
        label_36->setObjectName(QString::fromUtf8("label_36"));

        horizontalLayout_36->addWidget(label_36);

        spinBox_36 = new QSpinBox(groupBox_9);
        spinBox_36->setObjectName(QString::fromUtf8("spinBox_36"));
        spinBox_36->setEnabled(false);
        spinBox_36->setMinimum(376);
        spinBox_36->setMaximum(3072);
        spinBox_36->setSingleStep(8);

        horizontalLayout_36->addWidget(spinBox_36);


        gridLayout->addLayout(horizontalLayout_36, 2, 0, 1, 1);

        horizontalLayout_33 = new QHBoxLayout();
        horizontalLayout_33->setObjectName(QString::fromUtf8("horizontalLayout_33"));
        label_33 = new QLabel(groupBox_9);
        label_33->setObjectName(QString::fromUtf8("label_33"));

        horizontalLayout_33->addWidget(label_33);

        spinBox_33 = new QSpinBox(groupBox_9);
        spinBox_33->setObjectName(QString::fromUtf8("spinBox_33"));
        spinBox_33->setEnabled(false);
        spinBox_33->setMinimum(320);
        spinBox_33->setMaximum(2048);
        spinBox_33->setSingleStep(8);

        horizontalLayout_33->addWidget(spinBox_33);


        gridLayout->addLayout(horizontalLayout_33, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton_7 = new QPushButton(groupBox_9);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setEnabled(false);
        pushButton_7->setCheckable(true);
        pushButton_7->setChecked(true);
        pushButton_7->setAutoExclusive(false);

        horizontalLayout->addWidget(pushButton_7);

        pushButton_8 = new QPushButton(groupBox_9);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setEnabled(false);
        pushButton_8->setCheckable(false);
        pushButton_8->setAutoExclusive(false);

        horizontalLayout->addWidget(pushButton_8);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 1);

        gridLayout->addLayout(horizontalLayout, 4, 0, 1, 1);


        verticalLayout->addWidget(groupBox_9);


        gridLayout_2->addLayout(verticalLayout, 0, 0, 1, 1);

        label_3 = new QLabel(ControlPanel);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(139, 0));
        label_3->setMaximumSize(QSize(139, 9999));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);


        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QWidget *ControlPanel)
    {
        ControlPanel->setWindowTitle(QApplication::translate("ControlPanel", "ControlPanel", nullptr));
        groupBox_7->setTitle(QApplication::translate("ControlPanel", "\345\214\272\345\237\2370", nullptr));
        label_26->setText(QApplication::translate("ControlPanel", "Offset X", nullptr));
#ifndef QT_NO_TOOLTIP
        spinBox_26->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        label_27->setText(QApplication::translate("ControlPanel", "\345\256\275\345\272\246", nullptr));
        pushButton_5->setText(QApplication::translate("ControlPanel", "\347\273\230\345\210\266ROI", nullptr));
        label_25->setText(QApplication::translate("ControlPanel", "Offset Y", nullptr));
        label_28->setText(QApplication::translate("ControlPanel", "\351\253\230\345\272\246", nullptr));
        groupBox_8->setTitle(QApplication::translate("ControlPanel", "\345\214\272\345\237\2371", nullptr));
        label_32->setText(QApplication::translate("ControlPanel", "\351\253\230\345\272\246", nullptr));
        label_29->setText(QApplication::translate("ControlPanel", "Offset X", nullptr));
        label_30->setText(QApplication::translate("ControlPanel", "Offset Y", nullptr));
        label_31->setText(QApplication::translate("ControlPanel", "\345\256\275\345\272\246", nullptr));
        pushButton_6->setText(QApplication::translate("ControlPanel", "\347\273\230\345\210\266ROI", nullptr));
        groupBox_9->setTitle(QApplication::translate("ControlPanel", "\350\201\224\345\220\210\345\214\272\345\237\237", nullptr));
        label_34->setText(QApplication::translate("ControlPanel", "Offset X", nullptr));
        label_35->setText(QApplication::translate("ControlPanel", "Offset Y", nullptr));
        label_36->setText(QApplication::translate("ControlPanel", "\345\256\275\345\272\246", nullptr));
        label_33->setText(QApplication::translate("ControlPanel", "\351\253\230\345\272\246", nullptr));
        pushButton_7->setText(QApplication::translate("ControlPanel", "\345\210\240\351\231\244ROI", nullptr));
        pushButton_8->setText(QApplication::translate("ControlPanel", "\344\277\235\345\255\230", nullptr));
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // ROIPAGEGMSTBD_H
