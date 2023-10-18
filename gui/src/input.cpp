#include "input.h"

wxDEFINE_EVENT(UPDATE_DATA, UpdateEvent);

Input::Input(wxWindow *parent) :wxPanel(parent){

    // Font Settings
    wxString faceName = "Arial";
    int size = 10;
    wxFont font(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, faceName);

    // Choose a cross-section type
    std::array<wxString, 1> choices;
    choices[0] = "Rectangle";

    // Panel
    auto* selectionPanel = new wxPanel(this);
    selectionPanel->SetBackgroundColour("#424242");
    auto* dataPanel = new wxPanel(this);
    dataPanel->SetBackgroundColour("#424242");
    wxPanel* cutSectionPanel = new wxPanel(this);
    cutSectionPanel->SetBackgroundColour("#424242");

    // Sizers
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* selectionSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer* dataSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* cutSectionSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* cutSectionSubSizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxSizer* cutSectionSubSizer2 = new wxBoxSizer(wxHORIZONTAL);

    // Text box size
    wxSize textBoxSize = ToDIP(wxSize(100, wxDefaultSize.GetHeight()));

    // Selection Menu
    auto* selectionStaticText = new wxStaticText(selectionPanel, wxID_ANY, wxT("Section Type: "), wxDefaultPosition, textBoxSize);
    selectionStaticText->SetFont(font);
    selectionStaticText->SetForegroundColour(*wxWHITE);
    selectionStaticText->SetBackgroundColour(wxColour("#424242"));
    selectionSizer->Add(selectionStaticText, 0, wxLEFT, 5);

    auto* selectionComboBox = new wxComboBox(selectionPanel, wxID_ANY, "Rectangle", wxDefaultPosition, textBoxSize, 1, choices.data(), wxCB_READONLY);
    selectionComboBox->Bind(wxEVT_COMBOBOX, &Input::OnComboBoxSelect, this);
    selectionComboBox->SetSelection(0);
    selectionComboBox->SetFont(font);
    selectionComboBox->SetForegroundColour(wxColour(*wxWHITE));
    selectionComboBox->SetBackgroundColour(wxColour("#424242"));
    selectionSizer->Add(selectionComboBox, 1, wxRIGHT | wxEXPAND, 5);

    selectionPanel->SetSizerAndFit(selectionSizer);
    mainSizer->Add(selectionPanel, 0, wxALL | wxEXPAND, 10);

    // Data Entry
    for (auto& [id, text, value] : m_section.getFields())
    {
        wxSizer* lineSizer = new wxBoxSizer(wxHORIZONTAL);
        auto* paramName = new wxStaticText(dataPanel, wxID_ANY, text, wxDefaultPosition, textBoxSize);
        paramName->SetFont(font);
        paramName->SetForegroundColour(*wxWHITE);
        paramName->SetBackgroundColour(wxColour("#424242"));
        auto* paramValue = new wxTextCtrl(dataPanel, static_cast<long>(id), wxString::Format("%.2f", value), wxDefaultPosition, textBoxSize, wxTE_RIGHT,
                                          wxFloatingPointValidator<double>(2, &value));
        paramValue->SetFont(font);
        paramValue->SetForegroundColour(*wxWHITE);
        paramValue->SetBackgroundColour(wxColour("#424242"));
        lineSizer->Add(paramName, 0, wxLEFT | wxBOTTOM, 5);
        lineSizer->Add(paramValue, 0, wxRIGHT | wxBOTTOM | wxEXPAND, 5);
        dataSizer->Add(lineSizer, 0, wxTOP | wxEXPAND, 5);
     }
     dataPanel->SetSizerAndFit(dataSizer);
     mainSizer->Add(dataPanel, 0, wxALL | wxEXPAND, 10);

     // Event Handlers
     Bind(wxEVT_TEXT, [this](wxCommandEvent& event) {
                 TransferDataFromWindow();
                 populateCoordinateList();
                 sendUpdateEvent();
     });

    // Coordinate List
    m_coordinateList = new wxListView(this);
    m_coordinateList->AppendColumn("ID");
    m_coordinateList->AppendColumn("X");
    m_coordinateList->AppendColumn("Y");
    m_coordinateList->SetColumnWidth(0, 50);
    m_coordinateList->SetColumnWidth(1, 80);
    m_coordinateList->SetColumnWidth(2, 80);
    m_coordinateList->SetFont(font);
    m_coordinateList->SetForegroundColour(wxColour(*wxWHITE));
    m_coordinateList->SetBackgroundColour(wxColour("#424242"));
    populateCoordinateList();
    mainSizer->Add(m_coordinateList, 0, wxALL | wxEXPAND, 10);

    // Text box size
    textBoxSize = ToDIP(wxSize(60, wxDefaultSize.GetHeight()));

    // Cut Section Controller
    wxStaticText* topText = new wxStaticText(cutSectionPanel, wxID_ANY, "Top", wxDefaultPosition, textBoxSize);
    topText->SetFont(font);
    topText->SetForegroundColour(*wxWHITE);
    topText->SetBackgroundColour(wxColour("#424242"));
    cutDepthAdjuster = new wxSlider(cutSectionPanel, wxID_ANY, 0, -100, 100);
    wxStaticText* bottomText = new wxStaticText(cutSectionPanel, wxID_ANY, "Bottom", wxDefaultPosition, textBoxSize, wxTE_RIGHT);
    bottomText->SetFont(font);
    bottomText->SetForegroundColour(*wxWHITE);
    bottomText->SetBackgroundColour(wxColour("#424242"));
    cutSectionSubSizer1->Add(topText, 0, wxLEFT, 5);
    cutSectionSubSizer1->Add(cutDepthAdjuster);
    cutSectionSubSizer1->Add(bottomText, 0, wxRIGHT, 5);
    cutSectionSizer->Add(cutSectionSubSizer1);

    wxStaticText* minAngleText = new wxStaticText(cutSectionPanel, wxID_ANY, "0\u00B0", wxDefaultPosition, textBoxSize);
    minAngleText->SetFont(font);
    minAngleText->SetForegroundColour(*wxWHITE);
    minAngleText->SetBackgroundColour(wxColour("#424242"));
    cutAngleAdjuster = new wxSlider(cutSectionPanel, wxID_ANY, 0, 0, 360);
    wxStaticText* maxAngleText = new wxStaticText(cutSectionPanel, wxID_ANY, "360\u00B0", wxDefaultPosition, textBoxSize, wxTE_RIGHT);
    maxAngleText->SetFont(font);
    maxAngleText->SetForegroundColour(*wxWHITE);
    maxAngleText->SetBackgroundColour(wxColour("#424242"));
    cutSectionSubSizer2->Add(minAngleText, 0, wxLEFT, 5);
    cutSectionSubSizer2->Add(cutAngleAdjuster);
    cutSectionSubSizer2->Add(maxAngleText, 0, wxRIGHT, 5);
    cutSectionSizer->Add(cutSectionSubSizer2);

    cutSectionPanel->SetSizerAndFit(cutSectionSizer);
    mainSizer->Add(cutSectionPanel,0, wxALL | wxEXPAND, 10);

    // Event Handlers
    cutDepthAdjuster->Bind(wxEVT_SLIDER, [this](wxCommandEvent& event) {
        sendUpdateEvent();
    });

    cutAngleAdjuster->Bind(wxEVT_SLIDER, [this](wxCommandEvent& event) {
        sendUpdateEvent();
            });

    //this->SetMinSize(FromDIP(wxSize(200, 200)));
    SetSizerAndFit(mainSizer);

    sendUpdateEvent();

}

void Input::OnComboBoxSelect(wxCommandEvent &event) {

    switch (event.GetSelection()) {
        case 0:
            break;

    }
}

void Input::sendUpdateEvent() {
    this->SetId(1000);
    double cutDepth = (static_cast<double>(cutDepthAdjuster->GetValue()) - cutDepthAdjuster->GetMin())/(cutDepthAdjuster->GetMax() - cutDepthAdjuster->GetMin()) * 2 - 1;
    //wxMessageBox(std::to_string(cutAngleAdjuster->GetValue()));
    UpdateEvent event(UPDATE_DATA, this->GetId(), m_section.generatePointData(),
                      cutAngleAdjuster->GetValue(),cutDepth);
    event.SetEventObject(this);
    ProcessWindowEvent(event);
}

void Input::populateCoordinateList() {
    m_coordinateList->DeleteAllItems();
    std::vector<Point> points = m_section.generatePointData();
    for (size_t i = 0; i < points.size()-1; i++)
    {
        m_coordinateList->InsertItem(i, wxString(std::to_string(i+1)));
        m_coordinateList->SetItem(i, 1,  wxString::Format("%0.2f", points[i].x()));
        m_coordinateList->SetItem(i, 2,  wxString::Format("%0.2f", points[i].y()));
    }
}

XS::XS(double width, double depth) : m_width(width), m_depth(depth) {

    // Input Fields
    m_fields = {
        {ID::WIDTH, "Width", m_width},
        {ID::DEPTH, "Depth", m_depth}
    };
}

void XS::setWidth(double width) {

    m_width = width;
}

void XS::setDepth(double depth) {

    m_depth = depth;
}

std::vector<std::tuple<XS::ID, wxString, double&>> XS::getFields() const {

    return  m_fields;;
}

std::vector<Point> XS::generatePointData() {
    // Generate coordinates
    std::vector<Point> pointData;
    pointData.emplace_back(0, 0);
    pointData.emplace_back(m_width, 0);
    pointData.emplace_back(m_width, m_depth);
    pointData.emplace_back(0, m_depth);
    pointData.emplace_back(0, 0);
    return pointData;
}

