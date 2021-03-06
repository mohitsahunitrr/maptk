/*ckwg +29
 * Copyright 2016-2018 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither the name Kitware, Inc. nor the names of any contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MainWindow.h"
#include "tools/AbstractTool.h"
#include "VideoImport.h"

#include <maptk/version.h>

#include <vital/plugin_loader/plugin_manager.h>

#include <qtCliArgs.h>
#include <qtStlUtil.h>
#include <qtUtil.h>

#include <QApplication>
#include <QDir>
#include <QMetaType>
#include <QSurfaceFormat>

#include <memory>

#include <QVTKOpenGLWidget.h>
#include <vtkOpenGLRenderWindow.h>

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  // Set the default surface format for the OpenGL view
  vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
  // Set application information
  QApplication::setApplicationName("MAP-Tk TeleSculptor");
  QApplication::setOrganizationName("Kitware");
  QApplication::setOrganizationDomain("kitware.com");
  QApplication::setApplicationVersion(MAPTK_VERSION);

  // Register meta types
  using map_metadata_t = kwiver::vital::metadata_map::map_metadata_t;
  qRegisterMetaType<std::shared_ptr<ToolData>>();
  qRegisterMetaType<std::shared_ptr<map_metadata_t>>();

  // Set up command line options
  qtCliArgs args(argc, argv);
  qtCliOptions options;

  options.add("project <file>", "Load specified project file")
         .add("p", qtCliOption::Short);
  options.add("imagery <file>", "Load imagery from 'file'")
         .add("i", qtCliOption::Short | qtCliOption::NamedList);
  options.add("mask <file>", "Load mask imagery from 'file'")
         .add("m", qtCliOption::Short | qtCliOption::NamedList);
  options.add("camera <file>", "Load camera(s) from 'file'")
         .add("c", qtCliOption::Short | qtCliOption::NamedList);
  options.add("tracks <file>", "Load feature tracks from 'file'")
         .add("t", qtCliOption::Short | qtCliOption::NamedList);
  options.add("landmarks <file>", "Load landmarks from 'file'")
         .add("l", qtCliOption::Short | qtCliOption::NamedList);
  args.addOptions(options);

  // Parse arguments
  args.parseOrDie();

  // Create application instance and set icon
  QApplication app(args.qtArgc(), args.qtArgv());
  qtUtil::setApplicationIcon("TeleSculptor");

  // Load KWIVER plugins
  auto const exeDir = QDir{QApplication::applicationDirPath()};
  auto& vpm = kwiver::vital::plugin_manager::instance();
  vpm.add_search_path(stdString(exeDir.absoluteFilePath("../lib/kwiver/modules")));
  vpm.add_search_path(stdString(exeDir.absoluteFilePath("../lib/kwiver/processes")));
  vpm.load_all_plugins();

  // Tell PROJ where to find its data files
  auto projDataDir = exeDir.absoluteFilePath("../share/proj");
  if (QFileInfo{projDataDir}.isDir())
  {
    qputenv("PROJ_LIB", projDataDir.toLocal8Bit());
  }

  // Create and show main window
  MainWindow window;
  window.show();

  if (args.isSet("project"))
  {
    window.loadProject(args.value("project"));
  }
  for (auto const& path : args.values("imagery"))
  {
    window.loadImagery(path);
  }
  for (auto const& path : args.values("mask"))
  {
    window.loadMaskImagery(path);
  }
  for (auto const& path : args.values("camera"))
  {
    window.loadCamera(path);
  }
  for (auto const& path : args.values("tracks"))
  {
    window.loadTracks(path);
  }
  for (auto const& path : args.values("landmarks"))
  {
    window.loadLandmarks(path);
  }

  // Hand off to event loop
  return app.exec();
}
