/********************************************************************************
** Form generated from reading UI file 'scalecolor.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCALECOLOR_H
#define UI_SCALECOLOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <scalecolorwidget.h>

QT_BEGIN_NAMESPACE

class Ui_ScaleColorDlg
{
public:
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *txtScaleFile;
    QPushButton *cmdScaleFile;
    QLabel *label_4;
    QLineEdit *txtScaleDesc;
    QLabel *label_3;
    ScaleColorWidget *graphicsViewScale;
    QPushButton *cmdDigitize;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdSave;
    QPushButton *cmdSaveAs;
    QPushButton *cmdClear;
    QPushButton *cmdClose;
    QSpinBox *spinBandCount;

    void setupUi(QDialog *ScaleColorDlg)
    {
        if (ScaleColorDlg->objectName().isEmpty())
            ScaleColorDlg->setObjectName(QString::fromUtf8("ScaleColorDlg"));
        ScaleColorDlg->setWindowModality(Qt::NonModal);
        ScaleColorDlg->resize(708, 596);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScaleColorDlg->sizePolicy().hasHeightForWidth());
        ScaleColorDlg->setSizePolicy(sizePolicy);
        ScaleColorDlg->setModal(true);
        gridLayout = new QGridLayout(ScaleColorDlg);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_2 = new QLabel(ScaleColorDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        txtScaleFile = new QLineEdit(ScaleColorDlg);
        txtScaleFile->setObjectName(QString::fromUtf8("txtScaleFile"));

        gridLayout->addWidget(txtScaleFile, 0, 1, 1, 6);

        cmdScaleFile = new QPushButton(ScaleColorDlg);
        cmdScaleFile->setObjectName(QString::fromUtf8("cmdScaleFile"));

        gridLayout->addWidget(cmdScaleFile, 0, 7, 1, 1);

        label_4 = new QLabel(ScaleColorDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 1, 0, 1, 1);

        txtScaleDesc = new QLineEdit(ScaleColorDlg);
        txtScaleDesc->setObjectName(QString::fromUtf8("txtScaleDesc"));

        gridLayout->addWidget(txtScaleDesc, 1, 1, 1, 6);

        label_3 = new QLabel(ScaleColorDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        graphicsViewScale = new ScaleColorWidget(ScaleColorDlg);
        graphicsViewScale->setObjectName(QString::fromUtf8("graphicsViewScale"));
        graphicsViewScale->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewScale->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(graphicsViewScale, 3, 0, 1, 8);

        cmdDigitize = new QPushButton(ScaleColorDlg);
        cmdDigitize->setObjectName(QString::fromUtf8("cmdDigitize"));
        sizePolicy.setHeightForWidth(cmdDigitize->sizePolicy().hasHeightForWidth());
        cmdDigitize->setSizePolicy(sizePolicy);
        cmdDigitize->setMinimumSize(QSize(111, 31));
        cmdDigitize->setMaximumSize(QSize(111, 31));

        gridLayout->addWidget(cmdDigitize, 4, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 4, 2, 1, 1);

        cmdSave = new QPushButton(ScaleColorDlg);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        sizePolicy.setHeightForWidth(cmdSave->sizePolicy().hasHeightForWidth());
        cmdSave->setSizePolicy(sizePolicy);
        cmdSave->setMinimumSize(QSize(111, 31));
        cmdSave->setMaximumSize(QSize(111, 31));

        gridLayout->addWidget(cmdSave, 4, 3, 1, 1);

        cmdSaveAs = new QPushButton(ScaleColorDlg);
        cmdSaveAs->setObjectName(QString::fromUtf8("cmdSaveAs"));
        sizePolicy.setHeightForWidth(cmdSaveAs->sizePolicy().hasHeightForWidth());
        cmdSaveAs->setSizePolicy(sizePolicy);
        cmdSaveAs->setMinimumSize(QSize(111, 31));
        cmdSaveAs->setMaximumSize(QSize(111, 31));

        gridLayout->addWidget(cmdSaveAs, 4, 4, 1, 1);

        cmdClear = new QPushButton(ScaleColorDlg);
        cmdClear->setObjectName(QString::fromUtf8("cmdClear"));
        sizePolicy.setHeightForWidth(cmdClear->sizePolicy().hasHeightForWidth());
        cmdClear->setSizePolicy(sizePolicy);
        cmdClear->setMinimumSize(QSize(111, 31));
        cmdClear->setMaximumSize(QSize(111, 31));

        gridLayout->addWidget(cmdClear, 4, 5, 1, 1);

        cmdClose = new QPushButton(ScaleColorDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        sizePolicy.setHeightForWidth(cmdClose->sizePolicy().hasHeightForWidth());
        cmdClose->setSizePolicy(sizePolicy);
        cmdClose->setMinimumSize(QSize(111, 31));
        cmdClose->setMaximumSize(QSize(111, 31));

        gridLayout->addWidget(cmdClose, 4, 6, 1, 2);

        spinBandCount = new QSpinBox(ScaleColorDlg);
        spinBandCount->setObjectName(QString::fromUtf8("spinBandCount"));
        spinBandCount->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinBandCount->setMinimum(3);
        spinBandCount->setMaximum(9);
        spinBandCount->setValue(5);

        gridLayout->addWidget(spinBandCount, 2, 1, 1, 1);

        QWidget::setTabOrder(txtScaleFile, cmdScaleFile);
        QWidget::setTabOrder(cmdScaleFile, txtScaleDesc);
        QWidget::setTabOrder(txtScaleDesc, spinBandCount);
        QWidget::setTabOrder(spinBandCount, graphicsViewScale);
        QWidget::setTabOrder(graphicsViewScale, cmdDigitize);
        QWidget::setTabOrder(cmdDigitize, cmdSave);
        QWidget::setTabOrder(cmdSave, cmdSaveAs);
        QWidget::setTabOrder(cmdSaveAs, cmdClear);
        QWidget::setTabOrder(cmdClear, cmdClose);

        retranslateUi(ScaleColorDlg);

        QMetaObject::connectSlotsByName(ScaleColorDlg);
    } // setupUi

    void retranslateUi(QDialog *ScaleColorDlg)
    {
        ScaleColorDlg->setWindowTitle(QCoreApplication::translate("ScaleColorDlg", "Edit Color Scales", nullptr));
        label_2->setText(QCoreApplication::translate("ScaleColorDlg", "Scale File:", nullptr));
        cmdScaleFile->setText(QCoreApplication::translate("ScaleColorDlg", "browse ..", nullptr));
        label_4->setText(QCoreApplication::translate("ScaleColorDlg", "Description:", nullptr));
        label_3->setText(QCoreApplication::translate("ScaleColorDlg", "Band Count:", nullptr));
        cmdDigitize->setText(QCoreApplication::translate("ScaleColorDlg", "Digitize ...", nullptr));
        cmdSave->setText(QCoreApplication::translate("ScaleColorDlg", "Save", nullptr));
        cmdSaveAs->setText(QCoreApplication::translate("ScaleColorDlg", "Save As ...", nullptr));
        cmdClear->setText(QCoreApplication::translate("ScaleColorDlg", "Clear", nullptr));
        cmdClose->setText(QCoreApplication::translate("ScaleColorDlg", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScaleColorDlg: public Ui_ScaleColorDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCALECOLOR_H
