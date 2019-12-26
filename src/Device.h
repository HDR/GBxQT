#ifndef DEVICE_H
#define DEVICE_H

#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QString>
#include <QCheckBox>
#include <QThread>
#include "const.h"

class Device:public QGroupBox
{
    QString com_port;
    QString firm_ver;
    QVBoxLayout *labels;
    QHBoxLayout *north, *down;
    QBoxLayout *all;


public:

    static QThread::Priority priority;
    QLabel *com_label;
    QLabel *pcb_label;
    QLabel *firm_label;

    void setInfo();

    Device (QWidget * parent = nullptr);

    QString getCom ()
    {
        return com_port;
    }

    QString getFirm ()
    {
        return firm_ver;
    }


};

#endif // DEVICE_H
