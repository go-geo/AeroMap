/********************************************************************************
** Form generated from reading UI file 'light.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIGHT_H
#define UI_LIGHT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LightForm
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cmdClose;
    QTabWidget *tabWidget;
    QWidget *tabLight;
    QLabel *label_2;
    QComboBox *cboLight;
    QLabel *label;
    QLabel *label_3;
    QDoubleSpinBox *spinLightR;
    QLabel *label_12;
    QDoubleSpinBox *spinLightB;
    QDoubleSpinBox *spinLightG;
    QLabel *labLightPos;
    QSlider *sliderLightR;
    QSlider *sliderLightG;
    QSlider *sliderLightB;
    QCheckBox *chkLightSync;
    QWidget *tabMaterial;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *spinMatShininess;
    QDoubleSpinBox *spinMatAmbientR;
    QDoubleSpinBox *spinMatAmbientG;
    QDoubleSpinBox *spinMatAmbientB;
    QDoubleSpinBox *spinMatDiffuseR;
    QDoubleSpinBox *spinMatDiffuseB;
    QDoubleSpinBox *spinMatDiffuseG;
    QDoubleSpinBox *spinMatSpecularG;
    QDoubleSpinBox *spinMatSpecularB;
    QDoubleSpinBox *spinMatSpecularR;
    QComboBox *cboMaterial;
    QLabel *label_14;
    QSlider *sliderMatAmbientR;
    QSlider *sliderMatAmbientG;
    QSlider *sliderMatAmbientB;
    QSlider *sliderMatDiffuseR;
    QSlider *sliderMatDiffuseG;
    QSlider *sliderMatDiffuseB;
    QSlider *sliderMatSpecularR;
    QSlider *sliderMatSpecularG;
    QSlider *sliderMatSpecularB;
    QCheckBox *chkMatAmbientSync;
    QCheckBox *chkMatDiffuseSync;
    QCheckBox *chkMatSpecularSync;

    void setupUi(QWidget *LightForm)
    {
        if (LightForm->objectName().isEmpty())
            LightForm->setObjectName(QString::fromUtf8("LightForm"));
        LightForm->resize(585, 483);
        gridLayout = new QGridLayout(LightForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(414, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        cmdClose = new QPushButton(LightForm);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(cmdClose->sizePolicy().hasHeightForWidth());
        cmdClose->setSizePolicy(sizePolicy);
        cmdClose->setMinimumSize(QSize(111, 31));

        gridLayout->addWidget(cmdClose, 1, 1, 1, 1);

        tabWidget = new QTabWidget(LightForm);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabLight = new QWidget();
        tabLight->setObjectName(QString::fromUtf8("tabLight"));
        label_2 = new QLabel(tabLight);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 16, 61, 21));
        cboLight = new QComboBox(tabLight);
        cboLight->setObjectName(QString::fromUtf8("cboLight"));
        cboLight->setGeometry(QRect(70, 20, 69, 22));
        label = new QLabel(tabLight);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 61, 16));
        label_3 = new QLabel(tabLight);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(230, 40, 161, 16));
        spinLightR = new QDoubleSpinBox(tabLight);
        spinLightR->setObjectName(QString::fromUtf8("spinLightR"));
        spinLightR->setGeometry(QRect(70, 80, 51, 22));
        spinLightR->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinLightR->setMaximum(1.000000000000000);
        spinLightR->setSingleStep(0.010000000000000);
        spinLightR->setValue(1.000000000000000);
        label_12 = new QLabel(tabLight);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(20, 80, 61, 16));
        spinLightB = new QDoubleSpinBox(tabLight);
        spinLightB->setObjectName(QString::fromUtf8("spinLightB"));
        spinLightB->setGeometry(QRect(70, 140, 51, 22));
        spinLightB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinLightB->setMaximum(1.000000000000000);
        spinLightB->setSingleStep(0.010000000000000);
        spinLightB->setValue(1.000000000000000);
        spinLightG = new QDoubleSpinBox(tabLight);
        spinLightG->setObjectName(QString::fromUtf8("spinLightG"));
        spinLightG->setGeometry(QRect(70, 110, 51, 22));
        spinLightG->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinLightG->setMaximum(1.000000000000000);
        spinLightG->setSingleStep(0.010000000000000);
        spinLightG->setValue(1.000000000000000);
        labLightPos = new QLabel(tabLight);
        labLightPos->setObjectName(QString::fromUtf8("labLightPos"));
        labLightPos->setGeometry(QRect(230, 20, 231, 16));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        labLightPos->setFont(font);
        sliderLightR = new QSlider(tabLight);
        sliderLightR->setObjectName(QString::fromUtf8("sliderLightR"));
        sliderLightR->setGeometry(QRect(130, 80, 371, 19));
        sliderLightR->setMaximum(100);
        sliderLightR->setOrientation(Qt::Horizontal);
        sliderLightG = new QSlider(tabLight);
        sliderLightG->setObjectName(QString::fromUtf8("sliderLightG"));
        sliderLightG->setGeometry(QRect(130, 110, 371, 19));
        sliderLightG->setMaximum(100);
        sliderLightG->setOrientation(Qt::Horizontal);
        sliderLightB = new QSlider(tabLight);
        sliderLightB->setObjectName(QString::fromUtf8("sliderLightB"));
        sliderLightB->setGeometry(QRect(130, 140, 371, 19));
        sliderLightB->setMaximum(100);
        sliderLightB->setOrientation(Qt::Horizontal);
        chkLightSync = new QCheckBox(tabLight);
        chkLightSync->setObjectName(QString::fromUtf8("chkLightSync"));
        chkLightSync->setGeometry(QRect(510, 110, 51, 21));
        tabWidget->addTab(tabLight, QString());
        tabMaterial = new QWidget();
        tabMaterial->setObjectName(QString::fromUtf8("tabMaterial"));
        label_8 = new QLabel(tabMaterial);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(20, 70, 131, 21));
        label_9 = new QLabel(tabMaterial);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 180, 131, 21));
        label_10 = new QLabel(tabMaterial);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(20, 280, 131, 21));
        label_11 = new QLabel(tabMaterial);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 370, 131, 21));
        spinMatShininess = new QDoubleSpinBox(tabMaterial);
        spinMatShininess->setObjectName(QString::fromUtf8("spinMatShininess"));
        spinMatShininess->setGeometry(QRect(140, 370, 62, 22));
        spinMatShininess->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatShininess->setDecimals(1);
        spinMatShininess->setMaximum(1000.000000000000000);
        spinMatShininess->setSingleStep(10.000000000000000);
        spinMatAmbientR = new QDoubleSpinBox(tabMaterial);
        spinMatAmbientR->setObjectName(QString::fromUtf8("spinMatAmbientR"));
        spinMatAmbientR->setGeometry(QRect(140, 70, 51, 22));
        spinMatAmbientR->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatAmbientR->setMaximum(1.000000000000000);
        spinMatAmbientR->setSingleStep(0.100000000000000);
        spinMatAmbientR->setValue(1.000000000000000);
        spinMatAmbientG = new QDoubleSpinBox(tabMaterial);
        spinMatAmbientG->setObjectName(QString::fromUtf8("spinMatAmbientG"));
        spinMatAmbientG->setGeometry(QRect(140, 100, 51, 22));
        spinMatAmbientG->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatAmbientG->setMaximum(1.000000000000000);
        spinMatAmbientG->setSingleStep(0.100000000000000);
        spinMatAmbientG->setValue(1.000000000000000);
        spinMatAmbientB = new QDoubleSpinBox(tabMaterial);
        spinMatAmbientB->setObjectName(QString::fromUtf8("spinMatAmbientB"));
        spinMatAmbientB->setGeometry(QRect(140, 130, 51, 22));
        spinMatAmbientB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatAmbientB->setMaximum(1.000000000000000);
        spinMatAmbientB->setSingleStep(0.100000000000000);
        spinMatAmbientB->setValue(1.000000000000000);
        spinMatDiffuseR = new QDoubleSpinBox(tabMaterial);
        spinMatDiffuseR->setObjectName(QString::fromUtf8("spinMatDiffuseR"));
        spinMatDiffuseR->setGeometry(QRect(140, 180, 51, 22));
        spinMatDiffuseR->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatDiffuseR->setMaximum(1.000000000000000);
        spinMatDiffuseR->setSingleStep(0.100000000000000);
        spinMatDiffuseR->setValue(1.000000000000000);
        spinMatDiffuseB = new QDoubleSpinBox(tabMaterial);
        spinMatDiffuseB->setObjectName(QString::fromUtf8("spinMatDiffuseB"));
        spinMatDiffuseB->setGeometry(QRect(140, 240, 51, 22));
        spinMatDiffuseB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatDiffuseB->setMaximum(1.000000000000000);
        spinMatDiffuseB->setSingleStep(0.100000000000000);
        spinMatDiffuseB->setValue(1.000000000000000);
        spinMatDiffuseG = new QDoubleSpinBox(tabMaterial);
        spinMatDiffuseG->setObjectName(QString::fromUtf8("spinMatDiffuseG"));
        spinMatDiffuseG->setGeometry(QRect(140, 210, 51, 22));
        spinMatDiffuseG->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatDiffuseG->setMaximum(1.000000000000000);
        spinMatDiffuseG->setSingleStep(0.100000000000000);
        spinMatDiffuseG->setValue(1.000000000000000);
        spinMatSpecularG = new QDoubleSpinBox(tabMaterial);
        spinMatSpecularG->setObjectName(QString::fromUtf8("spinMatSpecularG"));
        spinMatSpecularG->setGeometry(QRect(140, 310, 51, 22));
        spinMatSpecularG->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatSpecularG->setMaximum(1.000000000000000);
        spinMatSpecularG->setSingleStep(0.100000000000000);
        spinMatSpecularG->setValue(1.000000000000000);
        spinMatSpecularB = new QDoubleSpinBox(tabMaterial);
        spinMatSpecularB->setObjectName(QString::fromUtf8("spinMatSpecularB"));
        spinMatSpecularB->setGeometry(QRect(140, 340, 51, 22));
        spinMatSpecularB->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatSpecularB->setMaximum(1.000000000000000);
        spinMatSpecularB->setSingleStep(0.100000000000000);
        spinMatSpecularB->setValue(1.000000000000000);
        spinMatSpecularR = new QDoubleSpinBox(tabMaterial);
        spinMatSpecularR->setObjectName(QString::fromUtf8("spinMatSpecularR"));
        spinMatSpecularR->setGeometry(QRect(140, 280, 51, 22));
        spinMatSpecularR->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        spinMatSpecularR->setMaximum(1.000000000000000);
        spinMatSpecularR->setSingleStep(0.100000000000000);
        spinMatSpecularR->setValue(1.000000000000000);
        cboMaterial = new QComboBox(tabMaterial);
        cboMaterial->setObjectName(QString::fromUtf8("cboMaterial"));
        cboMaterial->setGeometry(QRect(70, 20, 151, 22));
        label_14 = new QLabel(tabMaterial);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 20, 61, 16));
        sliderMatAmbientR = new QSlider(tabMaterial);
        sliderMatAmbientR->setObjectName(QString::fromUtf8("sliderMatAmbientR"));
        sliderMatAmbientR->setGeometry(QRect(200, 70, 301, 19));
        sliderMatAmbientR->setMaximum(100);
        sliderMatAmbientR->setOrientation(Qt::Horizontal);
        sliderMatAmbientG = new QSlider(tabMaterial);
        sliderMatAmbientG->setObjectName(QString::fromUtf8("sliderMatAmbientG"));
        sliderMatAmbientG->setGeometry(QRect(200, 100, 301, 19));
        sliderMatAmbientG->setMaximum(100);
        sliderMatAmbientG->setOrientation(Qt::Horizontal);
        sliderMatAmbientB = new QSlider(tabMaterial);
        sliderMatAmbientB->setObjectName(QString::fromUtf8("sliderMatAmbientB"));
        sliderMatAmbientB->setGeometry(QRect(200, 130, 301, 19));
        sliderMatAmbientB->setMaximum(100);
        sliderMatAmbientB->setOrientation(Qt::Horizontal);
        sliderMatDiffuseR = new QSlider(tabMaterial);
        sliderMatDiffuseR->setObjectName(QString::fromUtf8("sliderMatDiffuseR"));
        sliderMatDiffuseR->setGeometry(QRect(200, 180, 301, 19));
        sliderMatDiffuseR->setMaximum(100);
        sliderMatDiffuseR->setOrientation(Qt::Horizontal);
        sliderMatDiffuseG = new QSlider(tabMaterial);
        sliderMatDiffuseG->setObjectName(QString::fromUtf8("sliderMatDiffuseG"));
        sliderMatDiffuseG->setGeometry(QRect(200, 210, 301, 19));
        sliderMatDiffuseG->setMaximum(100);
        sliderMatDiffuseG->setOrientation(Qt::Horizontal);
        sliderMatDiffuseB = new QSlider(tabMaterial);
        sliderMatDiffuseB->setObjectName(QString::fromUtf8("sliderMatDiffuseB"));
        sliderMatDiffuseB->setGeometry(QRect(200, 240, 301, 19));
        sliderMatDiffuseB->setMaximum(100);
        sliderMatDiffuseB->setOrientation(Qt::Horizontal);
        sliderMatSpecularR = new QSlider(tabMaterial);
        sliderMatSpecularR->setObjectName(QString::fromUtf8("sliderMatSpecularR"));
        sliderMatSpecularR->setGeometry(QRect(200, 280, 301, 19));
        sliderMatSpecularR->setMaximum(100);
        sliderMatSpecularR->setOrientation(Qt::Horizontal);
        sliderMatSpecularG = new QSlider(tabMaterial);
        sliderMatSpecularG->setObjectName(QString::fromUtf8("sliderMatSpecularG"));
        sliderMatSpecularG->setGeometry(QRect(200, 310, 301, 19));
        sliderMatSpecularG->setMaximum(100);
        sliderMatSpecularG->setOrientation(Qt::Horizontal);
        sliderMatSpecularB = new QSlider(tabMaterial);
        sliderMatSpecularB->setObjectName(QString::fromUtf8("sliderMatSpecularB"));
        sliderMatSpecularB->setGeometry(QRect(200, 340, 301, 19));
        sliderMatSpecularB->setMaximum(100);
        sliderMatSpecularB->setOrientation(Qt::Horizontal);
        chkMatAmbientSync = new QCheckBox(tabMaterial);
        chkMatAmbientSync->setObjectName(QString::fromUtf8("chkMatAmbientSync"));
        chkMatAmbientSync->setGeometry(QRect(510, 100, 41, 21));
        chkMatDiffuseSync = new QCheckBox(tabMaterial);
        chkMatDiffuseSync->setObjectName(QString::fromUtf8("chkMatDiffuseSync"));
        chkMatDiffuseSync->setGeometry(QRect(510, 210, 41, 21));
        chkMatSpecularSync = new QCheckBox(tabMaterial);
        chkMatSpecularSync->setObjectName(QString::fromUtf8("chkMatSpecularSync"));
        chkMatSpecularSync->setGeometry(QRect(510, 310, 41, 21));
        tabWidget->addTab(tabMaterial, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 2);

        QWidget::setTabOrder(cboLight, spinLightR);
        QWidget::setTabOrder(spinLightR, spinLightG);
        QWidget::setTabOrder(spinLightG, spinLightB);
        QWidget::setTabOrder(spinLightB, cboMaterial);
        QWidget::setTabOrder(cboMaterial, spinMatAmbientR);
        QWidget::setTabOrder(spinMatAmbientR, spinMatAmbientG);
        QWidget::setTabOrder(spinMatAmbientG, spinMatAmbientB);
        QWidget::setTabOrder(spinMatAmbientB, spinMatDiffuseR);
        QWidget::setTabOrder(spinMatDiffuseR, spinMatDiffuseG);
        QWidget::setTabOrder(spinMatDiffuseG, spinMatDiffuseB);
        QWidget::setTabOrder(spinMatDiffuseB, spinMatSpecularR);
        QWidget::setTabOrder(spinMatSpecularR, spinMatSpecularG);
        QWidget::setTabOrder(spinMatSpecularG, spinMatSpecularB);
        QWidget::setTabOrder(spinMatSpecularB, spinMatShininess);
        QWidget::setTabOrder(spinMatShininess, cmdClose);
        QWidget::setTabOrder(cmdClose, sliderLightG);
        QWidget::setTabOrder(sliderLightG, sliderLightB);
        QWidget::setTabOrder(sliderLightB, sliderLightR);
        QWidget::setTabOrder(sliderLightR, tabWidget);
        QWidget::setTabOrder(tabWidget, sliderMatAmbientR);
        QWidget::setTabOrder(sliderMatAmbientR, sliderMatAmbientG);
        QWidget::setTabOrder(sliderMatAmbientG, sliderMatAmbientB);
        QWidget::setTabOrder(sliderMatAmbientB, sliderMatDiffuseR);
        QWidget::setTabOrder(sliderMatDiffuseR, sliderMatDiffuseG);
        QWidget::setTabOrder(sliderMatDiffuseG, sliderMatDiffuseB);
        QWidget::setTabOrder(sliderMatDiffuseB, sliderMatSpecularR);
        QWidget::setTabOrder(sliderMatSpecularR, sliderMatSpecularG);
        QWidget::setTabOrder(sliderMatSpecularG, sliderMatSpecularB);

        retranslateUi(LightForm);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(LightForm);
    } // setupUi

    void retranslateUi(QWidget *LightForm)
    {
        LightForm->setWindowTitle(QCoreApplication::translate("LightForm", "Edit Lights & Materials", nullptr));
        cmdClose->setText(QCoreApplication::translate("LightForm", "Close", nullptr));
        label_2->setText(QCoreApplication::translate("LightForm", "Position:", nullptr));
        label->setText(QCoreApplication::translate("LightForm", "Light:", nullptr));
        label_3->setText(QCoreApplication::translate("LightForm", "Use toolbar to move lights.", nullptr));
        label_12->setText(QCoreApplication::translate("LightForm", "Color:", nullptr));
        labLightPos->setText(QCoreApplication::translate("LightForm", "(0,0,0)", nullptr));
        chkLightSync->setText(QCoreApplication::translate("LightForm", "Sync", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabLight), QCoreApplication::translate("LightForm", "Lights", nullptr));
        label_8->setText(QCoreApplication::translate("LightForm", "Ambient Reflectivity:", nullptr));
        label_9->setText(QCoreApplication::translate("LightForm", "Diffuse  Reflectivity:", nullptr));
        label_10->setText(QCoreApplication::translate("LightForm", "Specular Reflectivity:", nullptr));
        label_11->setText(QCoreApplication::translate("LightForm", "Specular Shininess:", nullptr));
        label_14->setText(QCoreApplication::translate("LightForm", "Material:", nullptr));
        chkMatAmbientSync->setText(QCoreApplication::translate("LightForm", "Sync", nullptr));
        chkMatDiffuseSync->setText(QCoreApplication::translate("LightForm", "Sync", nullptr));
        chkMatSpecularSync->setText(QCoreApplication::translate("LightForm", "Sync", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tabMaterial), QCoreApplication::translate("LightForm", "Materials", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LightForm: public Ui_LightForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIGHT_H
