#include <QtWidgets/QFileDialog>
#include <QPixmap>
#include <QtWidgets/QMessageBox>
#include <QThread>
#include <QtSerialPort/QSerialPortInfo>
#include "Gui.h"
#include "Settings.h"
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <string>
#include "QtWidgets/QApplication"
#include "QTextStream"
#include "QDebug"
#include "QDateTime"
#include "QDesktopServices"
#include "windows.h"
#include <QWinTaskbarProgress>

#ifdef Q_OS_WIN
#endif

#include "const.h"
#include "icon.xpm"

Gui::Gui (QWidget * parent):QWidget (parent)
{
  QThread::currentThread ()->setPriority (QThread::NormalPriority);
  path = ".";			//current startup dir'
  if (Settings::darkmode == true)
  {
      QFile f(":qdarkstyle/style.qss");
      if (!f.exists())
      {
          printf("Unable to set stylesheet, file not found\n");
      }
      else
      {
          f.open(QFile::ReadOnly | QFile::Text);
          QTextStream ts(&f);
          qApp->setStyleSheet(ts.readAll());
      }
  }

  this->setWindowIcon (QIcon (QPixmap (icon)));
  this->setWindowTitle (tr ("GBxQT Version ") + VER);
  grid = new QGridLayout (this);
  left = new QVBoxLayout ();
  right = new QVBoxLayout ();
  center = new QVBoxLayout ();
  down = new QHBoxLayout ();

  image = new QLabel (this);

  image->setFixedSize (200, 162);
  settings = new Settings (this);
  left->addWidget (settings);
  left->addWidget (image);
  left->addStretch (1);
  grid->addLayout (left, 0, 0);
  console = new Console (this);
  right->addWidget (console);
  progress = new QProgressBar (this);
  winTaskbar = new QWinTaskbarButton(this);
  QWinTaskbarProgress *winProgress = winTaskbar->progress();
  down->addWidget (progress);
  cancel_btn = new QPushButton (tr ("Cancel"), this);
  cancel_btn->setEnabled (false);
  down->addWidget (cancel_btn);
  right->addLayout (down);
  grid->addLayout (right, 0, 2);
  status_btn = new QPushButton (tr ("Read Cart Info"), this);
  rflash_btn = new QPushButton (tr ("Read ROM"), this);
  wflash_btn = new QPushButton (tr ("Write ROM"), this);
  rram_btn = new QPushButton (tr ("Backup Save"), this);
  wram_btn = new QPushButton (tr ("Restore Save"), this);
  eflash_btn = new QPushButton (tr ("Erase ROM"), this);
  eram_btn = new QPushButton (tr ("Erase Save"), this);

  center->addWidget (status_btn, Qt::AlignTop);
  center->addWidget (rflash_btn);
  center->addWidget (wflash_btn);
  center->addWidget (rram_btn);
  center->addWidget (wram_btn);
  center->addWidget (eflash_btn);
  center->addWidget (eram_btn);
  center->addStretch (1);
  grid->addLayout (center, 0, 1);
  winProgress->setVisible(true);

  int func_wr = rand() % 100 + 1;
  if (func_wr == 23){winTaskbar->setWindow(this->windowHandle());winTaskbar->progress()->setVisible(true);winTaskbar->setOverlayIcon(QIcon(":/qss_icons/rc/genericarrow.png"));}


  connect (wflash_btn, SIGNAL (clicked ()), this, SLOT (write_flash ()));
  connect (rflash_btn, SIGNAL (clicked ()), this, SLOT (read_flash ()));
  connect (status_btn, SIGNAL (clicked ()), this, SLOT (show_info ()));
  connect (eflash_btn, SIGNAL (clicked ()), this, SLOT (erase_flash ()));
  connect (rram_btn, SIGNAL (clicked ()), this, SLOT (read_ram ()));
  connect (wram_btn, SIGNAL (clicked ()), this, SLOT (write_ram ()));
  connect (eram_btn, SIGNAL (clicked ()), this, SLOT (erase_ram ()));

  connect (settings, SIGNAL (refresh_ram_buttons (void)), this,
       SLOT (setRamButtons (void)));
  setProgress (0, 1);
  console->setTextColor(Qt::white);

  console->print (tr ("GBxQT version ") + VER + tr (" started."));
}

void
Gui::startup_info (void)
{


}

void
Gui::show_info ()
{
}


void
Gui::read_flash (void)
{
  file_name =
    QFileDialog::getSaveFileName (this, tr ("Write FLASH to..."), path,
                  tr ("GB Rom Dumps (*.gb *.gbc *.sgb)"));
}

void
Gui::write_flash (void)
{
  file_name =
    QFileDialog::getOpenFileName (this, tr ("Read FLASH from..."), path,
                  tr ("GB Rom Dumps (*.gb *.gbc *.sgb)"));
}


void
Gui::read_ram (void)
{
  file_name =
    QFileDialog::getSaveFileName (this, tr ("Write RAM to..."), path,
                  tr ("GB Save (*.sav)"));
}

void
Gui::write_ram (void)
{
  file_name =
    QFileDialog::getOpenFileName (this, tr ("Read RAM from..."), path,
                  tr ("GB Save (*.sav)"));
}

void
Gui::erase_flash (void)
{
}

void
Gui::erase_ram (void)
{
}



void
Gui::setProgress (int ile, int max)
{
  progress->setMinimum (0);
  progress->setMaximum (max);
  progress->setValue (ile);
  winTaskbar->setWindow(this->windowHandle());
  winTaskbar->progress()->setVisible(true);
  winTaskbar->progress()->setMinimum (0);
  winTaskbar->progress()->setMaximum (max);
  winTaskbar->progress()->setValue (ile);
}


void
Gui::setEnabledButtons (bool state)
{
  status_btn->setEnabled (state);
  rflash_btn->setEnabled (state);
  wflash_btn->setEnabled (state);
  eflash_btn->setEnabled (state);
  cancel_btn->setEnabled (!state);
  if (settings->isRamDisabled ())
    state = false;
  rram_btn->setEnabled (state);
  wram_btn->setEnabled (state);
  eram_btn->setEnabled (state);

}

void
Gui::setRamButtons ()
{
  if (status_btn->isEnabled ())
    setEnabledButtons (true);
}
