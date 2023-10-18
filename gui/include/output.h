#pragma once
#include "wx/wx.h"
#include "wx/valnum.h"

#include  "xprop/section.h"

class OutputData
{
public:
    double m_area, m_xCentroid, m_yCentroid, m_xQ, m_yQ, m_iXX, m_iYY, m_iXY;
    OutputData() {
        m_area = m_xCentroid = m_yCentroid = m_xQ = m_yQ = m_iXX = m_iYY = m_iXY = 0.0;
    }
};

class Output : public wxPanel{
private:
    std::vector<std::pair<wxString, double&>> m_fields;
    OutputData m_data, m_cutData;
    //double m_area, m_xCentroid, m_yCentroid, m_xQ, m_yQ, m_iXX, m_iYY, m_iXY;

    std::vector<std::pair<wxString, double&>> m_fieldsCut;
    //double m_areaCut, m_xCentroidCut, m_yCentroidCut, m_xQCut, m_yQCut, m_iXXCut, m_iYYCut, m_iXYCut;

public:
    explicit Output(wxWindow* parent);

    void UpdateOutput(OutputData& mainPolygon, OutputData& cutPolygon);
    void UpdateCutOutput(OutputData& cutPolygon);
};