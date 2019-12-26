#ifndef FL_SETTINGS_H_
#define FL_SETTINGS_H_

#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QString>
#include <QCheckBox>
#include <QThread>
#include "const.h"

class Settings:public QGroupBox
{
  QString com_name;
  int flash_size;
  int ram_size;
  bool auto_size;
  QComboBox *com_combo;
  QComboBox *ctype_combo;
  QComboBox *roms_combo;
  QComboBox *memty_combo;
  QComboBox *rsize_combo;
  QLabel *ctype_label;
  QLabel *roms_label;
  QLabel *memty_label;
  QLabel *rsize_label;
  QCheckBox *GB_check;
  QCheckBox *GBA_check;
  QVBoxLayout *labels, *combo_boxes;
  QHBoxLayout *north, *down;
  QVBoxLayout *all;


public:
  static bool commanual;
  static bool showbbl;
  static bool darkmode;
  static QThread::Priority priority;



    Settings (QWidget * parent = nullptr);

  int getFlash ()
  {
    return flash_size;
  }

  int getRam ()
  {
    return ram_size;
  }

  QString getCom (void)
  {
    return com_name;
  }

  QString getCom (int index)
  {
    return com_combo->itemText (index);
  }

  bool isAuto ()
  {
    return auto_size;
  }

  bool isRamDisabled ()
  {

    return (memty_combo->currentText () == "");
  }

  void ram_types (int type);
  void flash_types (int type, double newParameter = 2.0);


  void flash_types(int type);
public slots:
  void setMbc (int mbc_nr);
  void setFlash (int index);
  void setRam (int index);
  void setAuto (int state);
  void gbToggled ();
  void gbaToggled ();
signals:
  void refresh_ram_buttons ();

};

#endif
