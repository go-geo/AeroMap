/********************************************************************************
** Form generated from reading UI file 'config.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_H
#define UI_CONFIG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigDlg
{
public:
    QPushButton *cmdSave;
    QPushButton *cmdClose;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *cmdAppDataPath;
    QLabel *label_2;
    QLineEdit *txtAppDataPath;
    QComboBox *cboStartup;
    QLabel *label_5;

    void setupUi(QDialog *ConfigDlg)
    {
        if (ConfigDlg->objectName().isEmpty())
            ConfigDlg->setObjectName(QString::fromUtf8("ConfigDlg"));
        ConfigDlg->resize(605, 456);
        cmdSave = new QPushButton(ConfigDlg);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(340, 410, 121, 33));
        cmdClose = new QPushButton(ConfigDlg);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(470, 410, 121, 33));
        tabWidget = new QTabWidget(ConfigDlg);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 581, 381));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        cmdAppDataPath = new QPushButton(tab);
        cmdAppDataPath->setObjectName(QString::fromUtf8("cmdAppDataPath"));
        cmdAppDataPath->setGeometry(QRect(440, 20, 75, 21));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 20, 81, 16));
        txtAppDataPath = new QLineEdit(tab);
        txtAppDataPath->setObjectName(QString::fromUtf8("txtAppDataPath"));
        txtAppDataPath->setGeometry(QRect(110, 20, 321, 20));
        cboStartup = new QComboBox(tab);
        cboStartup->setObjectName(QString::fromUtf8("cboStartup"));
        cboStartup->setGeometry(QRect(110, 120, 321, 22));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 120, 81, 16));
        tabWidget->addTab(tab, QString());
        QWidget::setTabOrder(txtAppDataPath, cmdAppDataPath);
        QWidget::setTabOrder(cmdAppDataPath, cboStartup);
        QWidget::setTabOrder(cboStartup, cmdSave);
        QWidget::setTabOrder(cmdSave, cmdClose);
        QWidget::setTabOrder(cmdClose, tabWidget);

        retranslateUi(ConfigDlg);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ConfigDlg);
    } // setupUi

    void retranslateUi(QDialog *ConfigDlg)
    {
        ConfigDlg->setWindowTitle(QCoreApplication::translate("ConfigDlg", "Configuration", nullptr));
        cmdSave->setText(QCoreApplication::translate("ConfigDlg", "Save", nullptr));
        cmdClose->setText(QCoreApplication::translate("ConfigDlg", "Close", nullptr));
        cmdAppDataPath->setText(QCoreApplication::translate("ConfigDlg", "browse ..", nullptr));
        label_2->setText(QCoreApplication::translate("ConfigDlg", "App Data Path:", nullptr));
        label_5->setText(QCoreApplication::translate("ConfigDlg", "At Startup:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("ConfigDlg", "General", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfigDlg: public Ui_ConfigDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_H
