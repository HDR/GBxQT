#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QMessageBox>
#include <QSettings>
#include "Settings.h"
#include "Gui.h"
#include <math.h>

bool
  Settings::commanual = 0;
bool
  Settings::darkmode = 0;
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

  rsize_label = new QLabel ("RAM Size", this);
  labels->addWidget (rsize_label);
  rsize_combo = new QComboBox (this);
  combo_boxes->addWidget (rsize_combo);
  rsize_combo->insertItem (0, "256Kbit");
  rsize_combo->insertItem (1, "512Kbit");
  rsize_combo->insertItem (2, "1Mbit");
  rsize_combo->setCurrentIndex (0);

  GB_check = new QCheckBox (tr ("GB"), this);
  GB_check->setCheckState (Qt::Checked);
  GB_check->setEnabled(false);
  down->addWidget (GB_check);

  GBA_check = new QCheckBox (tr ("GBA"), this);
  GBA_check->setCheckState (Qt::Unchecked);
  down->addWidget (GBA_check);


  north->addLayout (labels);
  north->addLayout (combo_boxes);
  all->addLayout (north);
  all->addLayout (down);
  setLayout (all);

  auto_size = 1;
  ram_size = 8;
  flash_size = 512;


  //connect (ctype_combo, SIGNAL (activated (int)), this, SLOT (setMbc (int)));
  //connect (roms_combo, SIGNAL (activated (int)), this, SLOT (setFlash (int)));
  //connect (memty_combo, SIGNAL (activated (int)), this, SLOT (setRam (int)));
  connect(GBA_check, &QPushButton::clicked, this, &Settings::gbaToggled);
  connect(GB_check, &QPushButton::clicked, this, &Settings::gbToggled);

}

void
Settings::setFlash (int index)
{
  /* size = 32 * 2 ^ index */
  flash_size = 32 * (int) pow (2.0, (double) index);
}

void
Settings::setRam (int index)
{
  switch (index)
    {
    case 0:
      ram_size = 2;
      break;
    case 1:
      ram_size = 8;
      break;
    case 2:
      ram_size = 32;
      break;
    case 3:
      ram_size = 64;
      break;
    case 4:
      ram_size = 128;
      break;
    }

}

void
Settings::setMbc (int)
{
  //emit refresh_ram_buttons ();
}

//fill ram_combo depending on cart type
void
Settings::ram_types (int type)
{
  int ile = memty_combo->count ();
  for (int i = ile - 1; i >= 0; i--)
    memty_combo->removeItem (i);
  switch (type)
    {
    case 0:
      break;
    case 1:
      memty_combo->insertItem (0, "2 KB");
      memty_combo->insertItem (1, "8 KB");
      memty_combo->insertItem (2, "32 KB");
      break;
    case 2:
      memty_combo->insertItem (0, "2 KB");
      memty_combo->insertItem (1, "8 KB");
      memty_combo->insertItem (2, "32 KB");
      memty_combo->insertItem (3, "64 KB");
      break;
    }
}

void
Settings::flash_types (int type)
{
  int ile = roms_combo->count ();
  for (int i = ile - 1; i >= 0; i--)
    roms_combo->removeItem (i);

  for (int i = 0; i <= type; i++)
    {
      int pojemnosc = 32 * (int) pow (2.0, (double) i);
      roms_combo->insertItem (i, QString::number (pojemnosc) + " KB");
    }
}

void
Settings::setAuto (int state)
{
  if (state == Qt::Checked)
    auto_size = 1;
  else
    auto_size = 0;
}

void
Settings::gbToggled ()
{
    if(GB_check->checkState() == Qt::Checked){
        GBA_check->setEnabled(true);
        GB_check->setEnabled(false);
        GBA_check->setCheckState(Qt::Unchecked);
    }
}

void
Settings::gbaToggled ()
{
    if(GBA_check->checkState() == Qt::Checked){
        GB_check->setEnabled(true);
        GBA_check->setEnabled(false);
        GB_check->setCheckState(Qt::Unchecked);
    }
}
