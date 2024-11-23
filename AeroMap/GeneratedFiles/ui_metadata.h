/********************************************************************************
** Form generated from reading UI file 'metadata.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_METADATA_H
#define UI_METADATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_MetaDataDlg
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *txtMeta;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdClipboard;
    QPushButton *cmdClose;

    void setupUi(QDialog *MetaDataDlg)
    {
        if (MetaDataDlg->objectName().isEmpty())
            MetaDataDlg->setObjectName(QString::fromUtf8("MetaDataDlg"));
        MetaDataDlg->resize(464, 439);
        gridLayout = new QGridLayout(MetaDataDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        txtMeta = new QPlainTextEdit(MetaDataDlg);
        txtMeta->setObjectName(QString::fromUtf8("txtMeta"));
        QFont font;
        font.setFamily(QString::fromUtf8("Consolas"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        txtMeta->setFont(font);

        gridLayout->addWidget(txtMeta, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(288, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        cmdClipboard = new QPushButton(MetaDataDlg);
        cmdClipboard->setObjectName(QString::fromUtf8("cmdClipboard"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cmdClipboard->sizePolicy().hasHeightForWidth());
        cmdClipboard->setSizePolicy(sizePolicy);
        cmdClipboard->setMinimumSize(QSize(111, 31));

        gridLayout->addWidget(cmdClipboard, 1, 1, 1, 1);

        cmdClose = new QPushButton(MetaDataDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        sizePolicy.setHeightForWidth(cmdClose->sizePolicy().hasHeightForWidth());
        cmdClose->setSizePolicy(sizePolicy);
        cmdClose->setMinimumSize(QSize(111, 31));

        gridLayout->addWidget(cmdClose, 1, 2, 1, 1);


        retranslateUi(MetaDataDlg);

        QMetaObject::connectSlotsByName(MetaDataDlg);
    } // setupUi

    void retranslateUi(QDialog *MetaDataDlg)
    {
        MetaDataDlg->setWindowTitle(QCoreApplication::translate("MetaDataDlg", "Metadata", nullptr));
        cmdClipboard->setText(QCoreApplication::translate("MetaDataDlg", "Copy To Clipboard", nullptr));
        cmdClose->setText(QCoreApplication::translate("MetaDataDlg", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MetaDataDlg: public Ui_MetaDataDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_METADATA_H
