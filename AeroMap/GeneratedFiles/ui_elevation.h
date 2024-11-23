/********************************************************************************
** Form generated from reading UI file 'elevation.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ELEVATION_H
#define UI_ELEVATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ElevationDlg
{
public:
    QPushButton *cmdClose;
    QPushButton *cmdApply;
    QDoubleSpinBox *spinDelta;
    QDoubleSpinBox *spinScale;
    QRadioButton *optDelta;
    QRadioButton *optScale;
    QRadioButton *optSmooth;

    void setupUi(QDialog *ElevationDlg)
    {
        if (ElevationDlg->objectName().isEmpty())
            ElevationDlg->setObjectName(QString::fromUtf8("ElevationDlg"));
        ElevationDlg->resize(212, 171);
        cmdClose = new QPushButton(ElevationDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(110, 120, 91, 33));
        cmdApply = new QPushButton(ElevationDlg);
        cmdApply->setObjectName(QString::fromUtf8("cmdApply"));
        cmdApply->setGeometry(QRect(10, 120, 91, 33));
        spinDelta = new QDoubleSpinBox(ElevationDlg);
        spinDelta->setObjectName(QString::fromUtf8("spinDelta"));
        spinDelta->setEnabled(false);
        spinDelta->setGeometry(QRect(120, 50, 71, 22));
        spinDelta->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinDelta->setDecimals(1);
        spinDelta->setMinimum(-1000.000000000000000);
        spinDelta->setMaximum(10000.000000000000000);
        spinDelta->setSingleStep(10.000000000000000);
        spinDelta->setValue(100.000000000000000);
        spinScale = new QDoubleSpinBox(ElevationDlg);
        spinScale->setObjectName(QString::fromUtf8("spinScale"));
        spinScale->setEnabled(true);
        spinScale->setGeometry(QRect(120, 20, 71, 22));
        spinScale->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinScale->setDecimals(2);
        spinScale->setMinimum(0.000000000000000);
        spinScale->setMaximum(16.000000000000000);
        spinScale->setSingleStep(1.000000000000000);
        spinScale->setValue(1.000000000000000);
        optDelta = new QRadioButton(ElevationDlg);
        optDelta->setObjectName(QString::fromUtf8("optDelta"));
        optDelta->setGeometry(QRect(20, 50, 82, 17));
        optDelta->setChecked(false);
        optScale = new QRadioButton(ElevationDlg);
        optScale->setObjectName(QString::fromUtf8("optScale"));
        optScale->setGeometry(QRect(20, 20, 82, 17));
        optScale->setChecked(true);
        optSmooth = new QRadioButton(ElevationDlg);
        optSmooth->setObjectName(QString::fromUtf8("optSmooth"));
        optSmooth->setGeometry(QRect(20, 80, 82, 17));
        optSmooth->setChecked(false);
        QWidget::setTabOrder(optScale, spinScale);
        QWidget::setTabOrder(spinScale, optDelta);
        QWidget::setTabOrder(optDelta, spinDelta);
        QWidget::setTabOrder(spinDelta, optSmooth);
        QWidget::setTabOrder(optSmooth, cmdApply);
        QWidget::setTabOrder(cmdApply, cmdClose);

        retranslateUi(ElevationDlg);

        QMetaObject::connectSlotsByName(ElevationDlg);
    } // setupUi

    void retranslateUi(QDialog *ElevationDlg)
    {
        ElevationDlg->setWindowTitle(QCoreApplication::translate("ElevationDlg", "Change Elevation", nullptr));
        cmdClose->setText(QCoreApplication::translate("ElevationDlg", "Cancel", nullptr));
        cmdApply->setText(QCoreApplication::translate("ElevationDlg", "Apply", nullptr));
        optDelta->setText(QCoreApplication::translate("ElevationDlg", "Fixed Delta:", nullptr));
        optScale->setText(QCoreApplication::translate("ElevationDlg", "Scale Factor:", nullptr));
        optSmooth->setText(QCoreApplication::translate("ElevationDlg", "Smooth", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ElevationDlg: public Ui_ElevationDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ELEVATION_H
