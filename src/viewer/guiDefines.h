/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
*
* This framework is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This framework is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this framework; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* For further information please contact Dieter Schmalstieg under
* <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the guiDefines.
*
* @author Denis Kalkofen
*
* $Id: guiDefines.h 25 2005-11-28 16:11:59Z denis $
* @file                                                                   */
/* ======================================================================= */

#ifndef _GUIEDFINES_H_
#define _GUIDEFINES_H_

#ifdef USE_SOQT

#ifdef WIN32
#define SOQT_DLL 1
#endif

#define GuiWidget_H <qwidget.h> 
#define SOGUI_H <Inventor/Qt/SoQt.h>
#define SOGUI_CURSOR_H <Inventor/Qt/SoQtCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Qt/viewers/SoQtExaminerViewer.h>

#define SoGui SoQt
#define GuiWidget QWidget*
#define SoGuiCursor SoQtCursor
#define SoGuiViewer SoQtViewer
#define SoGuiFullViewer SoQtFullViewer
#define SoGuiRenderArea SoQtRenderArea
#define SoGuiExaminerViewer SoQtExaminerViewer


#elif USE_SOGTK

#define SOGUI_H <Inventor/Gtk/SoGtk.h>
#define SOGUI_CURSOR_H <Inventor/Gtk/SoGtkCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Gtk/viewers/SoGtkExaminerViewer.h>

#define SoGui SoGtk
#define GuiWidget GtkWidget*
#define SoGuiCursor SoGtkCursor
#define SoGuiViewer SoGtkViewer
#define SoGuiFullViewer SoGtkFullViewer
#define SoGuiRenderArea SoGtkRenderArea
#define SoGuiExaminerViewer SoGtkExaminerViewer

#elif USE_SOXT

#define SOGUI_H <Inventor/Xt/SoXt.h>
#define SOGUI_CURSOR_H <Inventor/Xt/SoXtCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Xt/viewers/SoXtExaminerViewer.h>

#define SoGui SoXt
#define GuiWidget Widget
#define SoGuiCursor SoXtCursor
#define SoGuiViewer SoXtViewer
#define SoGuiFullViewer SoXtFullViewer
#define SoGuiRenderArea SoXtRenderArea
#define SoGuiExaminerViewer SoXtExaminerViewer

#elif USE_SOWIN

#ifdef WIN32
#define SOWIN_DLL 1
#endif

#define SOGUI_H <Inventor/Win/SoWin.h>
#define SOGUI_CURSOR_H <Inventor/Win/SoWinCursor.h>
#define SOGUI_EXAMINERVIEWER_H <Inventor/Win/viewers/SoWinExaminerViewer.h>

#define SoGui SoWin
#define GuiWidget HWND
#define SoGuiCursor SoWinCursor
#define SoGuiViewer SoWinViewer
#define SoGuiFullViewer SoWinFullViewer
#define SoGuiRenderArea SoWinRenderArea
#define SoGuiExaminerViewer SoWinExaminerViewer

#endif

#endif //GUIDEFINES_H
