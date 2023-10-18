#include "output.h"

Output::Output(wxWindow *parent) : wxPanel(parent){

    // Output Fields
    m_fields = {
            {"Area", m_data.m_area},
            {"Xc",  m_data.m_xCentroid},
            {"Yc", m_data.m_yCentroid},
            {"Qx", m_data.m_xQ},
            {"Qy", m_data.m_yQ},
            {"Ixx", m_data.m_iXX},
            {"Iyy", m_data.m_iYY},
            {"Ixy", m_data.m_iXY}
    };

    m_fieldsCut = {
            {"Area", m_cutData.m_area},
            {"Xc",  m_cutData.m_xCentroid},
            {"Yc", m_cutData.m_yCentroid},
            {"Qx", m_cutData.m_xQ},
            {"Qy", m_cutData.m_yQ},
            {"Ixx", m_cutData.m_iXX},
            {"Iyy", m_cutData.m_iYY},
            {"Ixy", m_cutData.m_iXY}
    };

    // Font Settings
    wxString faceName = "Arial";
    int size = 10;
    wxFont font(size, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, faceName);

    // Text box size
    wxSize textBoxSize = ToDIP(wxSize(100, wxDefaultSize.GetHeight()));

    // Panel
    auto* outputPanel = new wxPanel(this);
    outputPanel->SetBackgroundColour("#424242");
    auto* outputCutPanel = new wxPanel(this);
    outputCutPanel->SetBackgroundColour("#424242");

    // Output Data (Main)
    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxSizer* outputSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* titleText1 = new wxStaticText(outputPanel, wxID_ANY, "Section Properties");
    titleText1->SetFont(font);
    titleText1->SetForegroundColour(*wxWHITE);
    titleText1->SetBackgroundColour(wxColour("#424242"));
    outputSizer->Add(titleText1, 0, wxCENTER | wxALL, 5);

    for (auto& [key, value] : m_fields)
    {
        wxSizer* outputLineSizer = new wxBoxSizer(wxHORIZONTAL);
        auto* paramName = new wxStaticText(outputPanel, wxID_ANY, key, wxDefaultPosition, textBoxSize);
        paramName->SetFont(font);
        paramName->SetForegroundColour(*wxWHITE);
        paramName->SetBackgroundColour(wxColour("#424242"));
        auto* paramValue = new wxTextCtrl(outputPanel, wxID_ANY, wxString::Format("%.2f", value),
                                          wxDefaultPosition, textBoxSize, wxTE_RIGHT | wxTE_READONLY,
                                          wxFloatingPointValidator<double>(2, &value));
        paramValue->SetFont(font);
        paramValue->SetForegroundColour(*wxWHITE);
        paramValue->SetBackgroundColour(wxColour("#424242"));
        outputLineSizer->Add(paramName, 0, wxLEFT | wxBOTTOM , 5);
        outputLineSizer->Add(paramValue, 0, wxRIGHT | wxBOTTOM , 5);
        outputSizer->Add(outputLineSizer, 0, wxTOP | wxEXPAND, 5);
    }
    outputPanel->SetSizerAndFit(outputSizer);

    // Output Data (Cut Section)
    wxSizer* outputCutSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* titleText2 = new wxStaticText(outputCutPanel, wxID_ANY, "Cut-Section Properties");
    titleText2->SetFont(font);
    titleText2->SetForegroundColour(*wxWHITE);
    titleText2->SetBackgroundColour(wxColour("#424242"));
    outputCutSizer->Add(titleText2, 0, wxCENTER | wxALL, 5);

    for (auto& [key, value] : m_fieldsCut)
    {
        wxSizer* outputLineSizer = new wxBoxSizer(wxHORIZONTAL);
        auto* paramName = new wxStaticText(outputCutPanel, wxID_ANY, key, wxDefaultPosition, textBoxSize);
        paramName->SetFont(font);
        paramName->SetForegroundColour(*wxWHITE);
        paramName->SetBackgroundColour(wxColour("#424242"));
        auto* paramValue = new wxTextCtrl(outputCutPanel, wxID_ANY, wxString::Format("%.2f", value),
                                          wxDefaultPosition, textBoxSize, wxTE_RIGHT | wxTE_READONLY,
                                          wxFloatingPointValidator<double>(2, &value));
        paramValue->SetFont(font);
        paramValue->SetForegroundColour(*wxWHITE);
        paramValue->SetBackgroundColour(wxColour("#424242"));
        outputLineSizer->Add(paramName, 0, wxLEFT | wxBOTTOM, 5);
        outputLineSizer->Add(paramValue, 0, wxRIGHT | wxBOTTOM, 5);
        outputCutSizer->Add(outputLineSizer, 0, wxTOP | wxEXPAND, 5);
    }
    outputCutPanel->SetSizerAndFit(outputCutSizer);

    mainSizer->Add(outputPanel, 0, wxALL | wxEXPAND, 10);
    mainSizer->Add(outputCutPanel, 0, wxALL | wxEXPAND, 10);
    SetSizerAndFit(mainSizer);

}
void Output::UpdateOutput(OutputData& mainPolygon, OutputData& cutPolygon) {

    m_data = mainPolygon;
    m_cutData = cutPolygon;
    TransferDataToWindow();

}
void Output::UpdateCutOutput(OutputData& cutPolygon) {
    m_cutData = cutPolygon;
    TransferDataToWindow();
}
