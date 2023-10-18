#include "mainwnd.h"
#include <cstdlib>

MainWindow::MainWindow()
: wxFrame(nullptr, wxID_ANY, "Section Properties"){

    m_statusBar = this->CreateStatusBar();
    m_statusBar->SetFieldsCount(2);

    // Event Bindings
    Bind(UPDATE_DATA, &MainWindow::Update, this, 1000);

    // Dark mode
    bool isDark = wxSystemSettings::GetAppearance().IsDark();
    // Load icon image
    #ifdef __APPLE__
        isDark ?
           m_logo.LoadFile("../assets/mac/taskbar_dark.icns", wxBITMAP_TYPE_ICON_RESOURCE) :
           m_logo.LoadFile("../assets/mac/taskbar_light.icns", wxBITMAP_TYPE_ICON_RESOURCE) ;
        this->SetIcon(m_logo);
    #else
        isDark ?
           m_logo.LoadFile("../assets/taskbar_dark.png", wxBITMAP_TYPE_PNG) :
           m_logo.LoadFile("../assets/taskbar_light.png", wxBITMAP_TYPE_PNG) ;
        this->SetIcon(m_logo);
    #endif


   // Set Background Color
    isDark ?
    this->SetBackgroundColour(*wxBLACK) :
    this->SetBackgroundColour(*wxWHITE);

    // Set 2D Canvas
    m_canvas = new Graphics2D(this);
    m_canvas->SetMinSize(ToDIP(wxSize(400,300)));

    // Output
    m_output = new Output(this);

    // Input
    m_input = new Input(this);

    // Sizer settings
    wxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    //mainSizer->SetMinSize(FromDIP(1024), FromDIP(400));
    mainSizer->Add(m_input, 0, wxEXPAND);
    mainSizer->Add(m_output, 0, wxEXPAND);
    mainSizer->Add(m_canvas, 1, wxEXPAND | wxALL, 10);

    SetSizerAndFit(mainSizer);

}

void MainWindow::Update(UpdateEvent& event) {

    // Update Data
    std::vector<Point> mainPolygon = event.getPoints();
    m_section.setCoordinates(mainPolygon);
    m_section.computeProperties();
    //auto [area, centroid, Q, I] = m_section.computeProperties(190 * M_PI/180, -1.0);
    auto [area, centroid, Q, I] = m_section.computeProperties(event.getTheta() * M_PI/180, event.getDistance());

    SetStatusText("c = " + wxString::Format("%f", event.getDistance()),1);
    SetStatusText(wxString::FromUTF8("\u03E9 =") + wxString::Format("%i", event.getTheta()));

    // Update Graphics View
    m_cutPoints = m_section.getCutPoints();
    m_canvas->setMainPolygon(mainPolygon);
    m_canvas->setCutPolygon(m_cutPoints);

    // Update Output
    m_mainPolygonData.m_area = m_section.area();
    m_mainPolygonData.m_xCentroid = m_section.centroid()(1,1);
    m_mainPolygonData.m_yCentroid = m_section.centroid()(2,1);
    m_mainPolygonData.m_xQ = m_section.firstMoment()(1,1);
    m_mainPolygonData.m_yQ = m_section.firstMoment()(2,1);
    m_mainPolygonData.m_iXX = m_section.Ix();
    m_mainPolygonData.m_iYY = m_section.Iy();
    m_mainPolygonData.m_iXY = m_section.Ixy();

    m_cutPolygonData.m_area = area;
    m_cutPolygonData.m_xCentroid = centroid(1,1);
    m_cutPolygonData.m_yCentroid = centroid(2,1);
    m_cutPolygonData.m_xQ = Q(1,1);
    m_cutPolygonData.m_yQ = Q(2,1);
    m_cutPolygonData.m_iXX = I(1,1);
    m_cutPolygonData.m_iYY = I(2,2);
    m_cutPolygonData.m_iXY = I(1,2);
    m_output->UpdateOutput(m_mainPolygonData, m_cutPolygonData);

}
void MainWindow::UpdateCutSection(UpdateEvent &event) {
    std::vector<Point> cutPolygon = event.getPoints();
    m_canvas->setCutPolygon(cutPolygon);
    m_output->UpdateCutOutput(m_cutPolygonData);
}
