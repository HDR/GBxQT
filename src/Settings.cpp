#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QSettings>
#include "Settings.h"
#include "Gui.h"
#include <math.h>

bool Settings::darkmode = 0;
QThread::Priority Settings::priority = QThread::NormalPriority;

Settings::Settings (QWidget * parent):QGroupBox (tr ("Settings"), parent)
{
  QSettings
    set;

  labels = new QVBoxLayout ();
  combo_boxes = new QVBoxLayout ();
  down = new QHBoxLayout ();
  north = new QHBoxLayout ();
  all = new QVBoxLayout ();
  ctype_label = new QLabel ("Cartridge Type:", this);

  labels->addWidget (ctype_label);
  ctype_combo = new QComboBox (this);
  combo_boxes->addWidget (ctype_combo);
  ctype_combo->insertItem (0, "Regular Rom Cart");
  ctype_combo->insertItem (1, "Flash Cart");

  roms_label = new QLabel ("Rom Size:", this);
  labels->addWidget (roms_label);
  roms_combo = new QComboBox (this);
  combo_boxes->addWidget (roms_combo);
  roms_combo->insertItem (0, "4 MByte");
  roms_combo->insertItem (1, "8 MByte");
  roms_combo->insertItem (2, "16 MByte");
  roms_combo->insertItem (3, "32 MByte");
  roms_combo->setCurrentIndex (0);

  memty_label = new QLabel ("Memory Type:", this);
  labels->addWidget (memty_label);
  memty_combo = new QComboBox (this);
  combo_boxes->addWidget (memty_combo);
  memty_combo->insertItem (0, "SRAM");
  memty_combo->insertItem (1, "FLASH");
  memty_combo->insertItem (2, "EEPROM");
  memty_combo->setCurrentIndex (0);

  rsize_label = new QLabel ("RAM Size:", this);
  labels->addWidget (rsize_label);
  rsize_combo = new QComboBox (this);
  combo_boxes->addWidget (rsize_combo);
  rsize_combo->insertItem (0, "256 Kbit");
  rsize_combo->insertItem (1, "512 Kbit");
  rsize_combo->insertItem (2, "1 Mbit");
  rsize_combo->setCurrentIndex (0);

  GB_check = new QCheckBox (tr ("GB"), this);
  GB_check->setCheckState (Qt::Unchecked);
  down->addWidget (GB_check);

  GBA_check = new QCheckBox (tr ("GBA"), this);
  GBA_check->setCheckState (Qt::Checked);
  GBA_check->setEnabled(false);
  down->addWidget (GBA_check);


  north->addLayout (labels);
  north->addLayout (combo_boxes);
  all->addLayout (north);
  all->addLayout (down);
  setLayout (all);

  connect(ctype_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::setCtype);
  connect(roms_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::setRsize);
  connect(memty_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::setMsize);
  connect(rsize_combo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Settings::setMType);
  connect(GBA_check, &QPushButton::clicked, this, &Settings::gbaToggled);
  connect(GB_check, &QPushButton::clicked, this, &Settings::gbToggled);
}

void
Settings::setCtype (int index)
{
    /*
    for (int i = fct_combo->count() - 1; i >= 0; i--)
      fct_combo->removeItem (i);
    combo_boxes->removeWidget(fct_combo);
    labels->removeWidget(fct_label);
    combo_boxes->removeWidget(pcb_combo);
    labels->removeWidget(pcb_label);
    combo_boxes->removeWidget(we_combo);
    labels->removeWidget(we_label);
    */

    switch (index)
    {
        case 0:
          //Regular Cart or MBC1
          break;
        case 1:
          //Flash Cart or MBC2
          break;
        case 2:
          //MBC3
          break;
        case 3:
          //MBC5
          break;
        case 4:
          //Flash Cart
          if(GB_check->checkState() == Qt::Checked){

              fct_label = new QLabel ("Flash Chip/Type:", this);
              labels->addWidget(fct_label);
              fct_combo = new QComboBox (this);
              combo_boxes->addWidget (fct_combo);
              fct_combo->insertItem(0, "iG 32KB Cart");
              fct_combo->insertItem(1, "iG 2MB 128KB SRAM Cart");
              fct_combo->insertItem(2, "iG 2MB 32KB FRAM Cart");
              fct_combo->insertItem(3, "iG 4MB 128KB SRAM Cart");
              fct_combo->insertItem(4, "32KB AM29F010B");
              fct_combo->insertItem(5, "32KB SST39SF010A");
              fct_combo->insertItem(6, "32KB AT49F040");
              fct_combo->insertItem(7, "512KB SST39SF040");
              fct_combo->insertItem(8, "1MB ES29LV160");
              fct_combo->insertItem(9, "2MB BV5");
              fct_combo->insertItem(10, "2MB AM29LV160DB");
              fct_combo->insertItem(11, "2MB 29LV160CTTC");
              fct_combo->insertItem(12, "2MB 29LV160TE");
              fct_combo->insertItem(13, "2MB AM29F016");
              fct_combo->insertItem(14, "2MB GB Smart 16M");
              fct_combo->insertItem(15, "4MB M29W640");
              fct_combo->insertItem(16, "4MB 29DL32BF");
              fct_combo->insertItem(17, "4MB MX29LV320");
              fct_combo->insertItem(18, "4MB S29GL032");
              fct_combo->insertItem(19, "32MB 256M29");
              fct_combo->insertItem(20, "32MB M29W256");
              fct_combo->insertItem(21, "32MB MX29GL256");

              pcb_label = new QLabel ("PCB:", this);
              labels->addWidget(pcb_label);
              pcb_combo = new QComboBox (this);
              combo_boxes->addWidget(pcb_combo);

              we_label = new QLabel ("WE:", this);
              labels->addWidget(we_label);
              we_combo = new QComboBox (this);
              combo_boxes->addWidget(we_combo);
          }
          break;


    }

}

void
Settings::setRsize (int index)
{
    switch (index)
    {
      case 0:
        //4MByte ROM
        break;
      case 1:
        //8MByte ROM
        break;
      case 2:
        //16MByte ROM
        break;
      case 3:
        //32MByte ROM
        break;
    }
}

void
Settings::setMsize (int index)
{
  for (int i = rsize_combo->count() - 1; i >= 0; i--)
    rsize_combo->removeItem (i);
  switch (index)
  {
    case 0:
      //SRAM
      rsize_combo->insertItem (0, "256 Kbit");
      rsize_combo->insertItem (1, "512 Kbit");
      rsize_combo->insertItem (2, "1 Mbit");
      rsize_combo->setCurrentIndex (0);
      break;
    case 1:
      //FLASH
      rsize_combo->insertItem (0, "256 Kbit");
      rsize_combo->insertItem (1, "512 Kbit");
      rsize_combo->insertItem (2, "1 Mbit");
      rsize_combo->setCurrentIndex (0);
      break;
    case 2:
      //EEPROM
      rsize_combo->insertItem (0, "4 Kbit");
      rsize_combo->insertItem (1, "64 Kbit");
      rsize_combo->setCurrentIndex (0);
      break;
  }

}

void
Settings::setMType (int type)
{
  if (memty_combo->currentIndex() == 2){
      mType = 3;
      switch(type){
        case 0:
          cSize = 1;
          break;
        case 1:
          cSize = 2;
          break;
      }
  } else {
      mType = 2;
      switch(type){
        case 0:
          cSize = 1;
          break;
        case 1:
          cSize = 2;
          break;
        case 2:
          cSize = 3;
          break;
        }
  }
}

void
Settings::gbToggled ()
{
    if(GB_check->checkState() == Qt::Checked){

        combo_boxes->removeWidget(memty_combo);
        labels->removeWidget(memty_label);

        Gui::set_mode(VOLTAGE_5V);
        GBA_check->setEnabled(true);
        GB_check->setEnabled(false);
        GBA_check->setCheckState(Qt::Unchecked);

        for (int i = ctype_combo->count() - 1; i >= 0; i--)
          ctype_combo->removeItem (i);

        ctype_combo->insertItem (0, "MBC1");
        ctype_combo->insertItem (1, "MBC2");
        ctype_combo->insertItem (2, "MBC3");
        ctype_combo->insertItem (3, "MBC5");
        ctype_combo->insertItem (4, "Flash Cart");
        ctype_combo->setCurrentIndex (0);

        for (int i = roms_combo->count() - 1; i >= 0; i--)
          roms_combo->removeItem (i);

        roms_combo->insertItem (0, " 32KByte");
        roms_combo->insertItem (1, " 64KByte");
        roms_combo->insertItem (2, " 128KByte");
        roms_combo->insertItem (3, " 256KByte");
        roms_combo->insertItem (4, " 512KByte");
        roms_combo->insertItem (5, "1 MByte");
        roms_combo->insertItem (6, "2 MByte");
        roms_combo->insertItem (7, "4 MByte");
        roms_combo->insertItem (8, "8 MByte");
        roms_combo->setCurrentIndex (0);

        for (int i = rsize_combo->count() - 1; i >= 0; i--)
            rsize_combo->removeItem (i);

        rsize_combo->insertItem (0, "None");
        rsize_combo->insertItem (1, "512 bytes");
        rsize_combo->insertItem (2, "2 KBytes");
        rsize_combo->insertItem (3, "8 KBytes");
        rsize_combo->insertItem (4, "32 KBytes");
        rsize_combo->insertItem (5, "64 KBytes");
        rsize_combo->insertItem (6, "128 KBytes");
        rsize_combo->setCurrentIndex (0);
    }
}

void
Settings::gbaToggled ()
{
    if(GBA_check->checkState() == Qt::Checked){
        Gui::set_mode(VOLTAGE_3_3V);
        GB_check->setEnabled(true);
        GBA_check->setEnabled(false);
        GB_check->setCheckState(Qt::Unchecked);


        /*
        for (int i = fct_combo->count() - 1; i >= 0; i--)
          fct_combo->removeItem (i);
        combo_boxes->removeWidget(fct_combo);
        labels->removeWidget(fct_label);
        combo_boxes->removeWidget(pcb_combo);
        labels->removeWidget(pcb_label);
        combo_boxes->removeWidget(we_combo);
        labels->removeWidget(we_label);
        */

        for (int i = ctype_combo->count() - 1; i >= 0; i--)
          ctype_combo->removeItem (i);

        ctype_combo->insertItem (0, "Regular Rom Cart");
        ctype_combo->insertItem (1, "Flash Cart");
        ctype_combo->setCurrentIndex (0);

        for (int i = roms_combo->count() - 1; i >= 0; i--)
          roms_combo->removeItem (i);

        roms_combo->insertItem (0, "4 MByte");
        roms_combo->insertItem (1, "8 MByte");
        roms_combo->insertItem (2, "16 MByte");
        roms_combo->insertItem (3, "32 MByte");
        roms_combo->setCurrentIndex (0);

        labels->insertWidget(2, memty_label);
        combo_boxes->insertWidget(2, memty_combo);

        for (int i = rsize_combo->count() - 1; i >= 0; i--)
            rsize_combo->removeItem (i);

        rsize_combo->insertItem (0, "256 Kbit");
        rsize_combo->insertItem (1, "512 Kbit");
        rsize_combo->insertItem (2, "1 Mbit");
        rsize_combo->setCurrentIndex (0);

    }
}
