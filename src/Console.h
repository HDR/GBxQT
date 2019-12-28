#ifndef FL_CONSOLE_H_
#define FL_CONSOLE_H_

#include <QTextEdit>
#include <QString>
#include <QPalette>
#include <QColor>
#include "const.h"

class Console:public QTextEdit
{
public:
  Console (QWidget * parent):QTextEdit (parent)
  {
    this->setMinimumWidth (300);
    this->setReadOnly (true);
    QPalette palette;
      palette.setColor (QPalette::Base, QColor (0, 0, 0));
      this->setPalette (palette);
    QFont font ("Arial");
      font.setPointSize (8);
      this->setFont (font);
  }

  void print (QString string)
  {
    append (string);
  }

  void samePrint (QString string)
  {
      QTextCursor prev_cursor = this->textCursor();
      this->moveCursor (QTextCursor::End);
      this->insertPlainText(string);
      this->setTextCursor(prev_cursor);
  }

  void line ()
  {
    print ("<hr><br>");
  }

  void clearConsole ()
  {
      this->clear();
      this->print (tr ("GBxQT version ") + VER + tr (" started."));
      this->line();
  }
};

#endif
