#pragma once
#include <array>
#include <unordered_map>

#include <wx/wx.h>
#include <wx/combobox.h>
#include <wx/listctrl.h>
#include "wx/valnum.h"

#include  "xprop/section.h"
#include "graphics2d.h"
#include "imagepanel.h"

// Rectangular Shaped Cross-Section
class XS {
public:
    enum class ID {WIDTH = wxID_HIGHEST + 1, DEPTH};

private:
    double m_width, m_depth;
 protected:
    std::vector<std::tuple<ID, wxString, double&>> m_fields;
public:
    // Constructor
    XS(double m_width = 4.0, double m_depth = 6.0);

    // Generate Point Data
    std::vector<Point> generatePointData();

    // Setters
    void setWidth(double width);
    void setDepth(double depth);

    // Getters
    [[nodiscard]]  std::vector<std::tuple<ID, wxString, double&>> getFields() const;
};


class UpdateEvent : public wxCommandEvent{
private:
    const std::vector<Point> m_pointData;
    const int m_thetaSliderValue;
    const double m_distanceSliderValue;
public:
    UpdateEvent(wxEventType eventType, int winID, const std::vector<Point>& pointData, int theta, double distance)
        : wxCommandEvent(eventType, winID), m_pointData(pointData), m_thetaSliderValue(theta), m_distanceSliderValue(distance){

    }
     //Implement Virtual Function
    virtual wxEvent *Clone() const {return new UpdateEvent(*this);}

    // Getter
    std::vector<Point> getPoints() const {
        return m_pointData;
    }
    int getTheta() const {
        return m_thetaSliderValue;
    }
    double getDistance() const {
        return m_distanceSliderValue;
    }
};

wxDECLARE_EVENT(UPDATE_DATA, UpdateEvent);

class Input : public wxPanel {
private:
    XS m_section;
    wxListView *m_coordinateList;
    wxSlider* cutAngleAdjuster, *cutDepthAdjuster;

public:
    explicit Input(wxWindow *parent);

    // Populate List
    void populateCoordinateList();

    // Generate Event
    void sendUpdateEvent();

    // Event Handler
    void OnComboBoxSelect(wxCommandEvent& event);
};