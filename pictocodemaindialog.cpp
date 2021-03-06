#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "pictocodemaindialog.h"

#include <cassert>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <QFile>
#include <QImage>

#include "pictocodemenudialog.h"
#pragma GCC diagnostic pop

ribi::p2c::MainDialog::MainDialog()
  : m_class_name{"Pic"},
    m_graphics_library{GraphicsLibrary::nds},
    m_header_filename{"pic.h"},
    m_image{}
{
  #ifndef NDEBUG
  Test();
  #endif
}

void ribi::p2c::MainDialog::SetInputFile(const std::string& filename)
{
  m_image = ImageToImage(QImage(filename.c_str()));
}

void ribi::p2c::MainDialog::SetInputImage(const QImage& qimage)
{
  m_image = ImageToImage(qimage);
}


#ifndef NDEBUG
void ribi::p2c::MainDialog::Test() noexcept
{
  {
    static bool is_tested{false};
    if (is_tested) return;
    is_tested = true;
  }
  MainDialog d;
  d.SetInputImage(QImage(":/p2c/images/R.png"));
  d.SetGraphicsLibrary(GraphicsLibrary::nds);
  assert(!d.ToHeaderFile().empty());
  assert(!d.ToImplementationFile().empty());
  d.SetGraphicsLibrary(GraphicsLibrary::qt);
  assert(!d.ToHeaderFile().empty());
  assert(!d.ToImplementationFile().empty());
}
#endif

std::vector<std::string> ribi::p2c::MainDialog::ToHeaderFile() const
{
  switch (m_graphics_library)
  {
    case GraphicsLibrary::nds: return ToNdsHeaderFile();
    case GraphicsLibrary::qt : return ToQtHeaderFile();
  }
  assert(!"Should not get here");
  throw std::logic_error("Unimplemented GraphicsLibrary");
}

std::vector<std::string> ribi::p2c::MainDialog::ToImplementationFile() const
{
  switch (m_graphics_library)
  {
    case GraphicsLibrary::nds: return ToNdsImplementationFile();
    case GraphicsLibrary::qt : return ToQtImplementationFile();
  }
  assert(!"Should not get here");
  throw std::logic_error("Unimplemented GraphicsLibrary");
}

std::vector<std::string> ribi::p2c::MainDialog::ToNdsHeaderFile() const
{
  const int width  = std::min(256,m_image.width());
  const int height = std::min(192,m_image.height());

  std::string upper_classname = boost::algorithm::to_upper_copy(m_class_name);

  std::vector<std::string> v;
  v.push_back("#ifndef PIC_TO_NDS_" + upper_classname + "_H");
  v.push_back("#define PIC_TO_NDS_" + upper_classname + "_H");
  v.push_back("");
  v.push_back("#include <nds.h>");
  v.push_back("");
  v.push_back("//Header file");
  v.push_back("//Code generated by PicToCode version " + MenuDialog().GetVersion());
  v.push_back("//From http://www.richelbilderbeek.nl/ToolPicToCode.htm");
  v.push_back("struct " + m_class_name);
  v.push_back("{");
  v.push_back("  void Draw(u16 * const buffer, const int x, const int y) const;");
  v.push_back(
    "  int GetWidth() const { return "
    + boost::lexical_cast<std::string>(width)
    + "; }"
  );
  v.push_back(
    "  int GetHeight() const { return "
    + boost::lexical_cast<std::string>(height)
    + "; }");

  v.push_back("};");
  v.push_back("");
  v.push_back("#endif // PIC_TO_NDS_" + upper_classname + "_H");
  return v;
}

std::vector<std::string> ribi::p2c::MainDialog::ToNdsImplementationFile() const
{
  const int width{m_image.width()};
  const int height{m_image.height()};
  //const int width  = std::min(256,m_image.width());
  //const int height = std::min(192,m_image.height());

  std::vector<std::string> v;
  v.push_back("//Implementation file");
  v.push_back("//Code generated by PicToCode version " + MenuDialog().GetVersion());
  v.push_back("//From http://www.richelbilderbeek.nl/ToolPicToCode.htm");
  v.push_back("#include \"" + m_header_filename + "\"");
  v.push_back("");
  v.push_back("void " + m_class_name + "::Draw(u16 * const buffer, const int x, const int y) const");
  v.push_back("{");
  v.push_back("  const int maxx = 256;");
  v.push_back("  //const int maxy = 192;");


  for (int y=0; y!=height; ++y)
  {
    for (int x=0; x!=width; ++x)
    {
      const int a = m_image.m_v[y][x].a;
      if (!a) continue;
      //NDS uses 5 bit colors (32), where Pixel uses 8 bit (256)
      const int r = m_image.m_v[y][x].r / 8;
      const int g = m_image.m_v[y][x].g / 8;
      const int b = m_image.m_v[y][x].b / 8;
      v.push_back(
          "  buffer[((y+"
        + boost::lexical_cast<std::string>(y)
        + ")*maxx)+(x+"
        + boost::lexical_cast<std::string>(x)
        + ")] = RGB15("
        + boost::lexical_cast<std::string>(r)
        + ","
        + boost::lexical_cast<std::string>(g)
        + ","
        + boost::lexical_cast<std::string>(b)
        + ");");
    }
  }
  v.push_back("}");

  return v;
}


std::vector<std::string> ribi::p2c::MainDialog::ToQtHeaderFile() const
{
  const int width{m_image.width()};
  const int height{m_image.height()};

  std::string upper_classname = boost::algorithm::to_upper_copy(m_class_name);

  std::vector<std::string> v;

  v.push_back("#ifndef PIC_TO_QT_" + upper_classname + "_H");
  v.push_back("#define PIC_TO_QT_" + upper_classname + "_H");
  v.push_back("");
  v.push_back("#include <nds.h>");
  v.push_back("");
  v.push_back("//Header file");
  v.push_back("//Code generated by PicToCode version " + MenuDialog().GetVersion());
  v.push_back("//From http://www.richelbilderbeek.nl/ToolPicToCode.htm");
  v.push_back("struct " + m_class_name);
  v.push_back("{");
  v.push_back("  void Draw(u16 * const buffer, const int x, const int y) const;");
  v.push_back(
    "  int GetWidth() const { return "
    + boost::lexical_cast<std::string>(width)
    + "; }"
  );
  v.push_back(
    "  int GetHeight() const { return "
    + boost::lexical_cast<std::string>(height)
    + "; }");

  v.push_back("};");
  v.push_back("");
  v.push_back("#endif // PIC_TO_QT_" + upper_classname + "_H");

  return v;
}

std::vector<std::string> ribi::p2c::MainDialog::ToQtImplementationFile() const
{
  const int width  = m_image.width();
  const int height = m_image.height();

  std::vector<std::string> v;
  //Function declaration
  v.push_back("//Code generated by PicToCode version " + MenuDialog().GetVersion());
  v.push_back("//From http://www.richelbilderbeek.nl/ToolPicToCode.htm");
  v.push_back("void Draw(const QImage& source, QImage& target, const int x, const int y)");
  v.push_back("{");
  v.push_back("  const int width  = " + std::to_string(width) + ";");
  v.push_back("  const int height = " + std::to_string(height) + ";");
  for (int y=0; y!=height; ++y)
  {

    v.push_back("  {");
    v.push_back("    unsigned char * const line_target");
    v.push_back("      = static_cast<unsigned char *>(target.scanLine("
      + boost::lexical_cast<std::string>(y)
      + "));");
    for (int x=0; x!=width; ++x)
    {
      v.push_back("    line["
        + boost::lexical_cast<std::string>((x*4)+0)
        + "] = "
        + boost::lexical_cast<std::string>(m_image.m_v[y][x].b)
        + "; line["
        + boost::lexical_cast<std::string>((x*4)+1)
        + "] = "
        + boost::lexical_cast<std::string>(m_image.m_v[y][x].g)
        +"; line["
        + boost::lexical_cast<std::string>((x*4)+2)
        + "] = "
        + boost::lexical_cast<std::string>(m_image.m_v[y][x].r)
        +"; line["
        + boost::lexical_cast<std::string>((x*4)+3)
        + "] = "
        + boost::lexical_cast<std::string>(m_image.m_v[y][x].a)
        +";");
    }
    v.push_back("  }");
  }
  v.push_back("  pixmap = pixmap.fromImage(image);");
  v.push_back("  //Add transparency");
  v.push_back("  const QBitmap mask = pixmap.createMaskFromColor(QColor(0,0,0,0).rgb());");
  v.push_back("  pixmap.setMask(mask);");
  v.push_back("  return pixmap;");
  v.push_back("}");
  return v;
}

