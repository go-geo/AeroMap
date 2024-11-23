/********************************************************************************
** Form generated from reading UI file 'contour.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTOUR_H
#define UI_CONTOUR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ContourDlg
{
public:
    QPushButton *cmdClose;
    QPushButton *cmdCreate;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *spinInterval;
    QDoubleSpinBox *spinStart;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *labMinElevation;
    QLabel *labMaxElevation;
    QPushButton *cmdColor;
    QGraphicsView *graphicsViewColor;
    QLabel *label_8;
    QLabel *label_9;
    QRadioButton *optColorSolid;
    QRadioButton *optColorScale;
    QComboBox *cboColorScale;

    void setupUi(QDialog *ContourDlg)
    {
        if (ContourDlg->objectName().isEmpty())
            ContourDlg->setObjectName(QString::fromUtf8("ContourDlg"));
        ContourDlg->resize(245, 371);
        ContourDlg->setContextMenuPolicy(Qt::NoContextMenu);
        cmdClose = new QPushButton(ContourDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(130, 330, 101, 33));
        cmdCreate = new QPushButton(ContourDlg);
        cmdCreate->setObjectName(QString::fromUtf8("cmdCreate"));
        cmdCreate->setGeometry(QRect(10, 330, 101, 33));
        label_2 = new QLabel(ContourDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 40, 91, 16));
        label_3 = new QLabel(ContourDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(30, 70, 91, 16));
        spinInterval = new QDoubleSpinBox(ContourDlg);
        spinInterval->setObjectName(QString::fromUtf8("spinInterval"));
        spinInterval->setGeometry(QRect(140, 40, 91, 22));
        spinInterval->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinInterval->setDecimals(1);
        spinInterval->setMinimum(1.000000000000000);
        spinInterval->setMaximum(10000.000000000000000);
        spinInterval->setSingleStep(10.000000000000000);
        spinInterval->setValue(100.000000000000000);
        spinStart = new QDoubleSpinBox(ContourDlg);
        spinStart->setObjectName(QString::fromUtf8("spinStart"));
        spinStart->setGeometry(QRect(140, 70, 91, 22));
        spinStart->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinStart->setDecimals(1);
        spinStart->setMinimum(-1000.000000000000000);
        spinStart->setMaximum(10000.000000000000000);
        spinStart->setSingleStep(10.000000000000000);
        spinStart->setValue(100.000000000000000);
        label_4 = new QLabel(ContourDlg);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 250, 91, 16));
        label_5 = new QLabel(ContourDlg);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(30, 270, 31, 16));
        label_6 = new QLabel(ContourDlg);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(30, 290, 41, 16));
        labMinElevation = new QLabel(ContourDlg);
        labMinElevation->setObjectName(QString::fromUtf8("labMinElevation"));
        labMinElevation->setGeometry(QRect(110, 270, 91, 16));
        labMaxElevation = new QLabel(ContourDlg);
        labMaxElevation->setObjectName(QString::fromUtf8("labMaxElevation"));
        labMaxElevation->setGeometry(QRect(110, 290, 91, 16));
        cmdColor = new QPushButton(ContourDlg);
        cmdColor->setObjectName(QString::fromUtf8("cmdColor"));
        cmdColor->setGeometry(QRect(200, 140, 31, 21));
        graphicsViewColor = new QGraphicsView(ContourDlg);
        graphicsViewColor->setObjectName(QString::fromUtf8("graphicsViewColor"));
        graphicsViewColor->setGeometry(QRect(120, 140, 71, 21));
        graphicsViewColor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsViewColor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        label_8 = new QLabel(ContourDlg);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 10, 91, 16));
        label_9 = new QLabel(ContourDlg);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 110, 91, 16));
        optColorSolid = new QRadioButton(ContourDlg);
        optColorSolid->setObjectName(QString::fromUtf8("optColorSolid"));
        optColorSolid->setGeometry(QRect(30, 140, 82, 17));
        optColorSolid->setChecked(true);
        optColorScale = new QRadioButton(ContourDlg);
        optColorScale->setObjectName(QString::fromUtf8("optColorScale"));
        optColorScale->setGeometry(QRect(30, 170, 82, 17));
        cboColorScale = new QComboBox(ContourDlg);
        cboColorScale->setObjectName(QString::fromUtf8("cboColorScale"));
        cboColorScale->setGeometry(QRect(30, 200, 201, 22));
        QWidget::setTabOrder(spinInterval, spinStart);
        QWidget::setTabOrder(spinStart, optColorSolid);
        QWidget::setTabOrder(optColorSolid, graphicsViewColor);
        QWidget::setTabOrder(graphicsViewColor, cmdColor);
        QWidget::setTabOrder(cmdColor, optColorScale);
        QWidget::setTabOrder(optColorScale, cboColorScale);
        QWidget::setTabOrder(cboColorScale, cmdCreate);
        QWidget::setTabOrder(cmdCreate, cmdClose);

        retranslateUi(ContourDlg);

        QMetaObject::connectSlotsByName(ContourDlg);
    } // setupUi

    void retranslateUi(QDialog *ContourDlg)
    {
        ContourDlg->setWindowTitle(QCoreApplication::translate("ContourDlg", "Create Contours", nullptr));
        cmdClose->setText(QCoreApplication::translate("ContourDlg", "Cancel", nullptr));
        cmdCreate->setText(QCoreApplication::translate("ContourDlg", "Create", nullptr));
        label_2->setText(QCoreApplication::translate("ContourDlg", "Contour Interval:", nullptr));
        label_3->setText(QCoreApplication::translate("ContourDlg", "Starting Contour:", nullptr));
        label_4->setText(QCoreApplication::translate("ContourDlg", "Terrain Elevation:", nullptr));
        label_5->setText(QCoreApplication::translate("ContourDlg", "Min:", nullptr));
        label_6->setText(QCoreApplication::translate("ContourDlg", "Max:", nullptr));
        labMinElevation->setText(QCoreApplication::translate("ContourDlg", "0.0", nullptr));
        labMaxElevation->setText(QCoreApplication::translate("ContourDlg", "0.0", nullptr));
        cmdColor->setText(QCoreApplication::translate("ContourDlg", "...", nullptr));
        label_8->setText(QCoreApplication::translate("ContourDlg", "Contours:", nullptr));
        label_9->setText(QCoreApplication::translate("ContourDlg", "Colors:", nullptr));
        optColorSolid->setText(QCoreApplication::translate("ContourDlg", "Solid Color:", nullptr));
        optColorScale->setText(QCoreApplication::translate("ContourDlg", "Color Scale:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ContourDlg: public Ui_ContourDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTOUR_H
