/********************************************************************************
** Form generated from reading UI file 'drone_proc.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRONE_PROC_H
#define UI_DRONE_PROC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DroneProcDlg
{
public:
    QPushButton *cmdRun;
    QPushButton *cmdCancel;
    QLabel *label_4;
    QCheckBox *chkDTM;
    QCheckBox *chkDSM;
    QLabel *label_6;
    QDoubleSpinBox *spinDemResolution;
    QLabel *label_8;
    QComboBox *cboInitStage;
    QLabel *label_7;
    QDoubleSpinBox *spinOrthoResolution;
    QLabel *label_9;
    QDoubleSpinBox *spinDemGapFillSteps;
    QCheckBox *chkFastOrtho;
    QLabel *label_10;

    void setupUi(QDialog *DroneProcDlg)
    {
        if (DroneProcDlg->objectName().isEmpty())
            DroneProcDlg->setObjectName(QString::fromUtf8("DroneProcDlg"));
        DroneProcDlg->resize(538, 351);
        cmdRun = new QPushButton(DroneProcDlg);
        cmdRun->setObjectName(QString::fromUtf8("cmdRun"));
        cmdRun->setGeometry(QRect(300, 310, 111, 33));
        cmdCancel = new QPushButton(DroneProcDlg);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(420, 310, 111, 33));
        label_4 = new QLabel(DroneProcDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(32, 192, 81, 16));
        chkDTM = new QCheckBox(DroneProcDlg);
        chkDTM->setObjectName(QString::fromUtf8("chkDTM"));
        chkDTM->setGeometry(QRect(96, 192, 271, 17));
        chkDSM = new QCheckBox(DroneProcDlg);
        chkDSM->setObjectName(QString::fromUtf8("chkDSM"));
        chkDSM->setGeometry(QRect(96, 212, 271, 17));
        label_6 = new QLabel(DroneProcDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(24, 56, 89, 16));
        spinDemResolution = new QDoubleSpinBox(DroneProcDlg);
        spinDemResolution->setObjectName(QString::fromUtf8("spinDemResolution"));
        spinDemResolution->setGeometry(QRect(152, 56, 62, 21));
        spinDemResolution->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinDemResolution->setMaximum(1000.000000000000000);
        spinDemResolution->setValue(0.000000000000000);
        label_8 = new QLabel(DroneProcDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(24, 16, 71, 16));
        cboInitStage = new QComboBox(DroneProcDlg);
        cboInitStage->setObjectName(QString::fromUtf8("cboInitStage"));
        cboInitStage->setGeometry(QRect(104, 16, 145, 22));
        label_7 = new QLabel(DroneProcDlg);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(24, 104, 129, 16));
        spinOrthoResolution = new QDoubleSpinBox(DroneProcDlg);
        spinOrthoResolution->setObjectName(QString::fromUtf8("spinOrthoResolution"));
        spinOrthoResolution->setGeometry(QRect(152, 104, 62, 21));
        spinOrthoResolution->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinOrthoResolution->setMaximum(1000.000000000000000);
        spinOrthoResolution->setValue(0.000000000000000);
        label_9 = new QLabel(DroneProcDlg);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(24, 80, 113, 16));
        spinDemGapFillSteps = new QDoubleSpinBox(DroneProcDlg);
        spinDemGapFillSteps->setObjectName(QString::fromUtf8("spinDemGapFillSteps"));
        spinDemGapFillSteps->setGeometry(QRect(152, 80, 62, 21));
        spinDemGapFillSteps->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinDemGapFillSteps->setDecimals(0);
        spinDemGapFillSteps->setMaximum(1000.000000000000000);
        spinDemGapFillSteps->setValue(0.000000000000000);
        chkFastOrtho = new QCheckBox(DroneProcDlg);
        chkFastOrtho->setObjectName(QString::fromUtf8("chkFastOrtho"));
        chkFastOrtho->setGeometry(QRect(152, 136, 113, 17));
        label_10 = new QLabel(DroneProcDlg);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(24, 136, 129, 16));
        QWidget::setTabOrder(cboInitStage, spinDemResolution);
        QWidget::setTabOrder(spinDemResolution, spinDemGapFillSteps);
        QWidget::setTabOrder(spinDemGapFillSteps, spinOrthoResolution);
        QWidget::setTabOrder(spinOrthoResolution, chkFastOrtho);
        QWidget::setTabOrder(chkFastOrtho, chkDTM);
        QWidget::setTabOrder(chkDTM, chkDSM);
        QWidget::setTabOrder(chkDSM, cmdRun);
        QWidget::setTabOrder(cmdRun, cmdCancel);

        retranslateUi(DroneProcDlg);

        QMetaObject::connectSlotsByName(DroneProcDlg);
    } // setupUi

    void retranslateUi(QDialog *DroneProcDlg)
    {
        DroneProcDlg->setWindowTitle(QCoreApplication::translate("DroneProcDlg", "Process Drone Imagery", nullptr));
        cmdRun->setText(QCoreApplication::translate("DroneProcDlg", "Run", nullptr));
        cmdCancel->setText(QCoreApplication::translate("DroneProcDlg", "Cancel", nullptr));
        label_4->setText(QCoreApplication::translate("DroneProcDlg", "Outputs:", nullptr));
#if QT_CONFIG(tooltip)
        chkDTM->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>Create a DTM (Digital Terrain Model, ground only).</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        chkDTM->setText(QCoreApplication::translate("DroneProcDlg", "DTM - Digital Terrain Model (ground)", nullptr));
#if QT_CONFIG(tooltip)
        chkDSM->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>Create a DSM (Digital Surface Model, ground + objects).</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        chkDSM->setText(QCoreApplication::translate("DroneProcDlg", "DSM - Digital Surface Model (canopy)", nullptr));
        label_6->setText(QCoreApplication::translate("DroneProcDlg", "DEM Resolution:", nullptr));
#if QT_CONFIG(tooltip)
        spinDemResolution->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>DSM/DTM resolution in cm/pixel. Note that this value is capped by a ground sampling distance (GSD) estimate.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_8->setText(QCoreApplication::translate("DroneProcDlg", "Initial Stage:", nullptr));
        label_7->setText(QCoreApplication::translate("DroneProcDlg", "Orthophoto Resolution:", nullptr));
#if QT_CONFIG(tooltip)
        spinOrthoResolution->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>Orthophoto resolution in cm / pixel. Note that this value is capped by a ground sampling distance (GSD) estimate.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label_9->setText(QCoreApplication::translate("DroneProcDlg", "DEM Gap Fill Steps:", nullptr));
#if QT_CONFIG(tooltip)
        spinDemGapFillSteps->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>Number of steps used to fill areas with gaps. Set to 0 to disable gap filling. Starting with a radius equal to the output resolution, N different DEMs are generated with progressively bigger radius using the inverse distance weighted (IDW) algorithm and merged together. Remaining gaps are then merged using nearest neighbor interpolation.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        chkFastOrtho->setToolTip(QCoreApplication::translate("DroneProcDlg", "<html><head/><body><p>Skips dense reconstruction and 3D model generation. It generates an orthophoto directly from the sparse reconstruction. If you just need an orthophoto and do not need a full 3D model, turn on this option.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        chkFastOrtho->setText(QCoreApplication::translate("DroneProcDlg", "No", nullptr));
        label_10->setText(QCoreApplication::translate("DroneProcDlg", "Fast Orthophoto:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DroneProcDlg: public Ui_DroneProcDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRONE_PROC_H
