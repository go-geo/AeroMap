/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AboutDlg
{
public:
    QPushButton *cmdClose;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *AboutDlg)
    {
        if (AboutDlg->objectName().isEmpty())
            AboutDlg->setObjectName(QString::fromUtf8("AboutDlg"));
        AboutDlg->resize(510, 359);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AboutDlg->sizePolicy().hasHeightForWidth());
        AboutDlg->setSizePolicy(sizePolicy);
        cmdClose = new QPushButton(AboutDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(370, 310, 121, 33));
        label_2 = new QLabel(AboutDlg);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 74, 471, 217));
        QFont font;
        font.setFamily(QString::fromUtf8("Sitka Text"));
        label_2->setFont(font);
        label_2->setFrameShape(QFrame::Panel);
        label_2->setFrameShadow(QFrame::Sunken);
        label_2->setTextFormat(Qt::RichText);
        label_2->setMargin(4);
        label_2->setIndent(-1);
        label_2->setOpenExternalLinks(true);
        label_3 = new QLabel(AboutDlg);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 20, 201, 51));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Sitka Text"));
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setWeight(50);
        label_3->setFont(font1);

        retranslateUi(AboutDlg);

        QMetaObject::connectSlotsByName(AboutDlg);
    } // setupUi

    void retranslateUi(QDialog *AboutDlg)
    {
        AboutDlg->setWindowTitle(QCoreApplication::translate("AboutDlg", "About AeroMap", nullptr));
        cmdClose->setText(QCoreApplication::translate("AboutDlg", "Close", nullptr));
        label_2->setText(QCoreApplication::translate("AboutDlg", "<html><head/><body><p align=\"center\"><span style=\" font-size:9pt; font-weight:600;\">The following libraries or tools are used in the development of AeroMap:</span></p><p align=\"center\"><a href=\"https://www.opendronemap.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">OpenDroneMap</span></a></p><p align=\"center\"><a href=\"https://www.qt.io/\"><span style=\" text-decoration: underline; color:#0000ff;\">Qt</span></a></p><p align=\"center\"><a href=\"https://gdal.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">GDAL</span></a></p><p align=\"center\"><a href=\"http://pdal.io/\"><span style=\" text-decoration: underline; color:#0000ff;\">PDAL</span></a></p><p align=\"center\"><a href=\"https://proj.org/\"><span style=\" text-decoration: underline; color:#0000ff;\">Proj</span></a></p><p align=\"center\"><a href=\" https://rapidlasso.com/lastools\"><span style=\" text-decoration: underline; color:#0000ff;\">LasTools</span></a></p><p align=\"center\"><br/></p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("AboutDlg", "<html><head/><body><p><span style=\" font-weight:600;\">AeroMap</span></p><p><br/></p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDlg: public Ui_AboutDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
