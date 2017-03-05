#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "qtpictocodemenudialog.h"

#include <cassert>

#include "about.h"
#include "pictocodemenudialog.h"
#include "qtaboutdialog.h"
#include "testtimer.h"
#include "qtpictocodemaindialog.h"
#include "trace.h"
#include "ui_qtpictocodemenudialog.h"
#pragma GCC diagnostic pop

ribi::p2c::QtMenuDialog::QtMenuDialog(QWidget *parent) :
    QtHideAndShowDialog(parent),
    ui(new Ui::QtPicToCodeMenuDialog)
{
  ui->setupUi(this);
}

ribi::p2c::QtMenuDialog::~QtMenuDialog() noexcept
{
  delete ui;
}

void ribi::p2c::QtMenuDialog::on_button_start_clicked()
{
  QtMainDialog d;
  this->ShowChild(&d);
}

void ribi::p2c::QtMenuDialog::on_button_about_clicked()
{
  QtAboutDialog d(MenuDialog().GetAbout());
  this->ShowChild(&d);
}

void ribi::p2c::QtMenuDialog::on_button_quit_clicked()
{
  close();
}
