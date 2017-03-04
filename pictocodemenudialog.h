#ifndef PICTOCODEMENUDIALOG_H
#define PICTOCODEMENUDIALOG_H

#include "menudialog.h"

namespace ribi {
namespace p2c {

struct MenuDialog final : public ::ribi::MenuDialog
{
  About GetAbout() const noexcept override;
  Help GetHelp() const noexcept override;
  std::string GetVersion() const noexcept override;
  std::vector<std::string> GetVersionHistory() const noexcept override;

  private:
  int ExecuteSpecific(const std::vector<std::string>& argv) noexcept override;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif
};

} //~namespace p2c
} //~namespace ribi

#endif // PICTOCODEMENUDIALOG_H
