#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#include <wx/wx.h>
#include <wx/settings.h>

#include "xprop/section.h"

#include "input.h"
#include "output.h"

class MainWindow : public wxFrame{
private:
    wxIcon m_logo;
    Input* m_input;
    Graphics2D* m_canvas;
    Output* m_output;

    // Status Bar
    wxStatusBar* m_statusBar;

    // Data Variables
    Section m_section;
    std::vector<Point> m_cutPoints;
    OutputData m_mainPolygonData;
    OutputData m_cutPolygonData;

public:
    MainWindow();

    // Event Handlers
    void Update(UpdateEvent& event);
    void UpdateCutSection(UpdateEvent& event);
};
