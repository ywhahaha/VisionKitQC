/********************************************************************************
** Form generated from reading UI file 'HistoryDataRecordAPFUXN.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef HISTORYDATARECORDAPFUXN_H
#define HISTORYDATARECORDAPFUXN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HistoryDataRecord
{
public:
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_8;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QVBoxLayout *verticalLayout_2;
    QDateTimeEdit *dateTimeEdit;
    QDateTimeEdit *dateTimeEdit_2;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_6;
    QLabel *label_7;
    QVBoxLayout *verticalLayout_5;
    QComboBox *comboBox_5;
    QComboBox *comboBox_6;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_5;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_6;
    QTextBrowser *textBrowser;
    QLabel *frame_2;
    QGridLayout *gridLayout_5;
    QLabel *label_8;

    void setupUi(QWidget *HistoryDataRecord)
    {
        if (HistoryDataRecord->objectName().isEmpty())
            HistoryDataRecord->setObjectName(QString::fromUtf8("HistoryDataRecord"));
        HistoryDataRecord->resize(1092, 631);
        gridLayout_3 = new QGridLayout(HistoryDataRecord);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        groupBox = new QGroupBox(HistoryDataRecord);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(400, 16777215));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QString::fromUtf8("verticalLayout_8"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);


        horizontalLayout->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        dateTimeEdit = new QDateTimeEdit(groupBox);
        dateTimeEdit->setObjectName(QString::fromUtf8("dateTimeEdit"));
        dateTimeEdit->setCurrentSection(QDateTimeEdit::YearSection);
        dateTimeEdit->setCalendarPopup(false);

        verticalLayout_2->addWidget(dateTimeEdit);

        dateTimeEdit_2 = new QDateTimeEdit(groupBox);
        dateTimeEdit_2->setObjectName(QString::fromUtf8("dateTimeEdit_2"));
        dateTimeEdit_2->setCalendarPopup(false);

        verticalLayout_2->addWidget(dateTimeEdit_2);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout_8->addLayout(horizontalLayout);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        verticalLayout_4->addWidget(label_6);

        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        verticalLayout_4->addWidget(label_7);


        gridLayout->addLayout(verticalLayout_4, 0, 0, 1, 1);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        comboBox_5 = new QComboBox(groupBox_3);
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName(QString::fromUtf8("comboBox_5"));

        verticalLayout_5->addWidget(comboBox_5);

        comboBox_6 = new QComboBox(groupBox_3);
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->addItem(QString());
        comboBox_6->setObjectName(QString::fromUtf8("comboBox_6"));

        verticalLayout_5->addWidget(comboBox_6);


        gridLayout->addLayout(verticalLayout_5, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        pushButton = new QPushButton(groupBox_3);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        horizontalLayout_2->addWidget(pushButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 2);


        verticalLayout_8->addWidget(groupBox_3);


        gridLayout_4->addLayout(verticalLayout_8, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_5, 0, 1, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 4)
            tableWidget->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->horizontalHeader()->setDefaultSectionSize(100);

        verticalLayout_3->addWidget(tableWidget);


        gridLayout_4->addLayout(verticalLayout_3, 1, 0, 1, 2);


        horizontalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(HistoryDataRecord);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QString::fromUtf8("verticalLayout_6"));
        textBrowser = new QTextBrowser(groupBox_2);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        verticalLayout_6->addWidget(textBrowser);

        frame_2 = new QLabel(groupBox_2);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setMinimumSize(QSize(768, 512));
        frame_2->setMaximumSize(QSize(3072, 2048));
        QFont font;
        font.setPointSize(20);
        frame_2->setFont(font);
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);\n"
"color: rgb(255, 255, 255);"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_2);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_5->setContentsMargins(0, 0, 0, 0);
        label_8 = new QLabel(frame_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(768, 512));
        label_8->setMaximumSize(QSize(3072, 2048));
        label_8->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
"qproperty-alignment: 'AlignTop | AlignLeft';\n"
"qproperty-wordWrap: true;\n"
"background-color: transparent;"));

        gridLayout_5->addWidget(label_8, 0, 0, 1, 1);


        verticalLayout_6->addWidget(frame_2);

        verticalLayout_6->setStretch(0, 1);
        verticalLayout_6->setStretch(1, 2);

        gridLayout_2->addLayout(verticalLayout_6, 0, 0, 1, 1);


        horizontalLayout_3->addWidget(groupBox_2);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);


        retranslateUi(HistoryDataRecord);
        QObject::connect(pushButton, SIGNAL(clicked()), HistoryDataRecord, SLOT(QueryingHistory()));
        QObject::connect(tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), HistoryDataRecord, SLOT(show_picture(QTableWidgetItem*)));
        QObject::connect(pushButton, SIGNAL(clicked()), HistoryDataRecord, SLOT(QueryingStatistics()));

        QMetaObject::connectSlotsByName(HistoryDataRecord);
    } // setupUi

    void retranslateUi(QWidget *HistoryDataRecord)
    {
        HistoryDataRecord->setWindowTitle(QApplication::translate("HistoryDataRecord", "HistoryDataRecord", nullptr));
        groupBox->setTitle(QString());
        label->setText(QApplication::translate("HistoryDataRecord", "\350\265\267\345\247\213\346\227\245\346\234\237", nullptr));
        label_2->setText(QApplication::translate("HistoryDataRecord", "\347\273\223\346\235\237\346\227\245\346\234\237", nullptr));
        dateTimeEdit->setDisplayFormat(QApplication::translate("HistoryDataRecord", "yyyy/MM/dd HH:mm", nullptr));
        dateTimeEdit_2->setDisplayFormat(QApplication::translate("HistoryDataRecord", "yyyy/MM/dd HH:mm", nullptr));
        groupBox_3->setTitle(QApplication::translate("HistoryDataRecord", "\350\256\260\345\275\225\346\237\245\350\257\242", nullptr));
        label_6->setText(QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\202\271\351\200\211\346\213\251", nullptr));
        label_7->setText(QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\273\223\346\236\234\351\200\211\346\213\251", nullptr));
        comboBox_5->setItemText(0, QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\202\2711\357\274\210\345\210\207\347\272\270\357\274\211", nullptr));
        comboBox_5->setItemText(1, QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\202\2712\357\274\210\345\216\213\347\211\207\357\274\211", nullptr));
        comboBox_5->setItemText(2, QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\202\2713\357\274\210\345\220\210\347\233\226\357\274\211", nullptr));

        comboBox_6->setItemText(0, QApplication::translate("HistoryDataRecord", "\345\205\250\351\203\250\345\220\210\346\240\274", nullptr));
        comboBox_6->setItemText(1, QApplication::translate("HistoryDataRecord", "A\345\267\245\344\275\215\345\220\210\346\240\274B\345\267\245\344\275\215\344\270\215\345\220\210\346\240\274", nullptr));
        comboBox_6->setItemText(2, QApplication::translate("HistoryDataRecord", "A\345\267\245\344\275\215\344\270\215\345\220\210\346\240\274B\345\267\245\344\275\215\345\220\210\346\240\274", nullptr));
        comboBox_6->setItemText(3, QApplication::translate("HistoryDataRecord", "\345\205\250\351\203\250\344\270\215\345\220\210\346\240\274", nullptr));

        pushButton->setText(QApplication::translate("HistoryDataRecord", "\346\237\245\350\257\242", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("HistoryDataRecord", "\346\227\245\346\234\237", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\202\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QApplication::translate("HistoryDataRecord", "\346\243\200\346\265\213\347\273\223\346\236\234", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QApplication::translate("HistoryDataRecord", "\346\226\207\344\273\266\345\220\215", nullptr));
        groupBox_2->setTitle(QString());
        textBrowser->setHtml(QApplication::translate("HistoryDataRecord", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", nullptr));
        frame_2->setText(QString());
        label_8->setText(QApplication::translate("HistoryDataRecord", "\345\233\276\347\211\207\345\214\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HistoryDataRecord: public Ui_HistoryDataRecord {};
} // namespace Ui

QT_END_NAMESPACE

#endif // HISTORYDATARECORDAPFUXN_H
