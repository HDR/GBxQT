#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QSettings>
#include "Device.h"
#include "Gui.h"
#include "Console.h"
#include <math.h>
#include <QStyleFactory>

Device::Device (QWidget * parent):QGroupBox (tr ("Device Info"), parent)
{
    labels = new QVBoxLayout ();
    north = new QHBoxLayout ();
    down = new QHBoxLayout ();
    all = new QHBoxLayout ();

    if (Settings::darkmode == false){
        #ifdef Q_OS_WIN
            Device::setStyle(QStyleFactory::create("windows"));
        #endif
    }


    com_label = new QLabel ("Port : Not Connected", this);
    labels->addWidget(com_label);
    pcb_label = new QLabel ("PCB Version: Not Connected", this);
    labels->addWidget(pcb_label);
    firm_label = new QLabel ("Firmware Version: Not Connected", this);
    labels->addWidget(firm_label);
    north->addLayout (labels);
    all->addLayout (north);
    all->addLayout (down);
    setLayout (all);
}
