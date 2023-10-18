#pragma once
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"

#include  "xprop/section.h"

#include <algorithm>
#include <vector>

class Graphics2D : public wxPanel{
private:
    std::vector<wxPoint2DDouble> m_mainPolygon;
    std::vector<wxPoint2DDouble> m_cutPolygon;
    double m_xScale, m_yScale;
    double m_xMax, m_xMin, m_yMin, m_yMax;
public:
    // Constructor
    explicit Graphics2D(wxWindow* parent);

    // Set Main Polygon Display Data
    void setMainPolygon(std::vector<Point> polygon);
    void setCutPolygon(std::vector<Point>& polygon);

    // Draw coordinate axis
    static void drawCoordinateAxis(wxGraphicsContext& gc, wxDouble offsetX, wxDouble offsetY);

    // Draw arrow with start point and angle about horizontal axis
    // theta in radians
   static void drawArrow(wxGraphicsContext& gc, wxPoint2DDouble startPoint, double theta,
                  double arrowTotalLength = 50.0,double arrowHeadLength = 10.0, double arrowHeadWidth = 10.0,
                  const wxColour& arrowColor = wxColour(162,210,255));

   static void drawText(wxGraphicsContext& gc, const wxString& text, wxPoint2DDouble textPosition, wxDouble textAngle);

   static void drawPoint(wxGraphicsContext& gc, wxPoint2DDouble position, wxDouble diameter, wxColour color);

    // Event Handlers
    void OnDraw(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);

};
