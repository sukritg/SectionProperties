#include "graphics2d.h"


Graphics2D::Graphics2D(wxWindow* parent)
: wxPanel(parent)
{
    Bind(wxEVT_PAINT, &Graphics2D::OnDraw, this);
    Bind(wxEVT_SIZE, &Graphics2D::OnSize, this);
}

void Graphics2D::OnDraw(wxPaintEvent &event) {

    // Dark mode
    bool isDark = wxSystemSettings::GetAppearance().IsDark();

    // Set Background Color
    this-> SetBackgroundStyle(wxBG_STYLE_PAINT);
    isDark ?
    this->SetBackgroundColour("#424242") :
    this->SetBackgroundColour(wxColour(255,255,255)) ;

    // Create paint DC
     wxAutoBufferedPaintDC dc(this);
     dc.Clear();

    // Create graphics context
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    // Get window size
    wxSize widgetSize;
    widgetSize = GetClientSize();
    wxDouble width, height;
    width = widgetSize.GetWidth();
    height = widgetSize.GetHeight();

    // Set screen offset
    wxDouble screenHorizontalOffset, screenVerticalOffset;
    screenHorizontalOffset = width * 0.1;
    screenVerticalOffset = height * 0.1;

    // Transformation Matrix
    wxAffineMatrix2D T;
    width = width -  2 * screenHorizontalOffset;
    height = height - 2 * screenVerticalOffset;
    wxDouble xScale = width;
    wxDouble yScale = height;

    T.Translate(screenHorizontalOffset, screenVerticalOffset);
    T.Scale(xScale, yScale);
    T.Mirror(wxVERTICAL);
    T.Translate(0, -1);

    // Draw Main Polygon
    std::vector<wxPoint2DDouble> transformedPolygon(m_mainPolygon.size());
    std::vector<wxPoint2DDouble> transformedCutPolygon(m_cutPolygon.size());
    if (gc && !m_mainPolygon.empty())
    {
        // Main Polygon
        gc->SetPen(wxPen(wxColour("#D4ADFC"), 3));
        std::transform(m_mainPolygon.begin(), m_mainPolygon.end(), transformedPolygon.begin(),
                       [T](wxPoint2DDouble& point){return T.TransformPoint(point);});
        gc->DrawLines(transformedPolygon.size(), transformedPolygon.data());

        // Cut Polygon
        gc->SetPen(wxPen(wxColour("#9EBC9E"), 3));
                std::transform(m_cutPolygon.begin(), m_cutPolygon.end(), transformedCutPolygon.begin(),
                       [T](wxPoint2DDouble& point){return T.TransformPoint(point);});
        gc->DrawLines(transformedCutPolygon.size(), transformedCutPolygon.data());

        // Main Polygon Annotation
        for (int i = 0; i < transformedPolygon.size() - 1; i++)
        {
            drawText(*gc, wxString::Format(wxT("%i"), i+1),
                     wxPoint2DDouble(transformedPolygon[i].m_x + 10,
                                     transformedPolygon[i].m_y - 10),
                                     0.0);
            drawPoint(*gc, wxPoint2DDouble(transformedPolygon[i].m_x,
                                           transformedPolygon[i].m_y),
                      5.0, wxColour("#D4ADFC"));
        }

        // Cut Polygon Annotation
        for (int i = 0; i < transformedCutPolygon.size() - 1; i++)
        {
            drawText(*gc, wxString::Format(wxT("%i"), i+1),
                     wxPoint2DDouble(transformedCutPolygon[i].m_x - 10,
                                     transformedCutPolygon[i].m_y - 10),
                     0.0);
            drawPoint(*gc, wxPoint2DDouble(transformedCutPolygon[i].m_x,
                                           transformedCutPolygon[i].m_y),
                      5.0, wxColour("#D4ADFC"));
        }

        // Draw axis
        drawCoordinateAxis(*gc, screenHorizontalOffset * 0.5, widgetSize.GetHeight() - screenVerticalOffset * 0.5);
        delete gc;
    }
}

void Graphics2D::setMainPolygon(std::vector<Point> polygon) {
    m_mainPolygon.clear();
    // Convert coordinates to natural coordinates [0, 1]
    m_xMax = std::max_element(polygon.cbegin(), polygon.cend(),
                           [](const Point& point1, const Point& point2){return point1.x() < point2.x();})->x();
    m_yMax = std::max_element(polygon.cbegin(), polygon.cend(),
                                 [](const Point& point1, const Point& point2){return point1.y() < point2.y();})->y();
    m_xMin = std::min_element(polygon.cbegin(), polygon.cend(),
                                 [](const Point& point1, const Point& point2){return point1.x() < point2.x();})->x();
    m_yMin = std::min_element(polygon.cbegin(), polygon.cend(),
                                 [](const Point& point1, const Point& point2){return point1.y() < point2.y();})->y();

    m_xScale = 1.0/(m_xMax - m_xMin);
    m_yScale = 1.0/(m_yMax - m_yMin);
    double scale = std::min(m_xScale, m_yScale);
    wxAffineMatrix2D T;
    T.Translate(0.5 - (m_xMax-m_xMin) * scale * 0.5, 0.5 - (m_yMax-m_yMin) * scale * 0.5);
    T.Scale(scale, scale);
    T.Translate(-m_xMin, -m_yMin);
    for (Point& point : polygon)
        m_mainPolygon.emplace_back(
                T.TransformPoint(wxPoint2DDouble(point.x(), point.y()))
                );
    Refresh();
}

void Graphics2D::setCutPolygon(std::vector<Point> &polygon) {
    m_cutPolygon.clear();
    double scale = std::min(m_xScale, m_yScale);
    wxAffineMatrix2D T;
    T.Translate(0.5 - (m_xMax-m_xMin) * scale * 0.5, 0.5 - (m_yMax-m_yMin) * scale * 0.5);
    T.Scale(scale, scale);
    T.Translate(-m_xMin, -m_yMin);
    for (Point& point : polygon)
        m_cutPolygon.emplace_back(
                T.TransformPoint(wxPoint2DDouble(point.x(), point.y()))
        );
    Refresh();
}

void Graphics2D::drawCoordinateAxis(wxGraphicsContext& gc,  wxDouble offsetX, wxDouble offsetY) {

    // Definition
    double screenXOffset = offsetX;
    double screenYOffset = offsetY;
    double arrowTotalLength = 50;
    double arrowHeadLength = 10;
    double arrowHeadWidth = 5;
    wxColor arrowColor("#ACD8AA");

    // x-axis
    drawArrow(gc, wxPoint2DDouble(screenXOffset, screenYOffset), 0.0, arrowTotalLength,
              arrowHeadLength, arrowHeadWidth,arrowColor);

    // y-axis
    drawArrow(gc, wxPoint2DDouble(screenXOffset, screenYOffset), M_PI/2, arrowTotalLength,
              arrowHeadLength, arrowHeadWidth, arrowColor);

    // Annotation
    drawText(gc, wxT("x"), wxPoint2DDouble(screenXOffset + arrowTotalLength, screenYOffset + 10),
             0.0);
    drawText(gc, wxT("y"), wxPoint2DDouble(screenXOffset - 10, screenYOffset - arrowTotalLength),
             0.0);

}

void Graphics2D::drawArrow(wxGraphicsContext& gc, wxPoint2DDouble startPoint, double theta,
                           double arrowTotalLength,double arrowHeadLength, double arrowHeadWidth,
                           const wxColour& arrowColor) {

    // Angle transformation
    theta = 2*M_PI - theta;

    // Color settings
    wxPen pen(arrowColor,2, wxPENSTYLE_SOLID);
    gc.SetPen(pen);
    wxBrush brush(arrowColor, wxBRUSHSTYLE_SOLID);
    gc.SetBrush(brush);

    // Arrow line
    wxGraphicsPath line = gc.CreatePath();
    double arrowLineLength = arrowTotalLength - arrowHeadLength;
    line.MoveToPoint(startPoint);
    line.AddLineToPoint(startPoint.m_x +  arrowLineLength*cos(theta), startPoint.m_y + arrowLineLength*sin(theta));
    gc.StrokePath(line);

    // Arrow head
    wxGraphicsPath head = gc.CreatePath();
    head.MoveToPoint(startPoint.m_x +  arrowTotalLength*cos(theta), startPoint.m_y + arrowTotalLength*sin(theta));
    head.AddLineToPoint(startPoint.m_x + arrowLineLength*cos(theta) -  arrowLineLength*sin(theta)/(std::sqrt(std::pow(-arrowLineLength*sin(theta),2)+std::pow(+arrowLineLength*cos(theta),2))) * 0.5 * arrowHeadWidth,
                        startPoint.m_y + arrowLineLength*sin(theta) +  arrowLineLength*cos(theta)/(std::sqrt(std::pow(-arrowLineLength*sin(theta),2)+std::pow(+arrowLineLength*cos(theta),2))) * 0.5 * arrowHeadWidth);
    head.AddLineToPoint(startPoint.m_x + arrowLineLength*cos(theta) +  arrowLineLength*sin(theta)/(std::sqrt(std::pow(+arrowLineLength*sin(theta),2)+std::pow(-arrowLineLength*cos(theta),2))) * 0.5 * arrowHeadWidth,
                        startPoint.m_y + arrowLineLength*sin(theta) -  arrowLineLength*cos(theta)/(std::sqrt(std::pow(+arrowLineLength*sin(theta),2)+std::pow(-arrowLineLength*cos(theta),2))) * 0.5 * arrowHeadWidth);
    gc.FillPath(head);

}

void Graphics2D::drawText(wxGraphicsContext& gc, const wxString& text, wxPoint2DDouble textPosition,
                          wxDouble textAngle) {
    gc.SetFont(wxSystemSettings::GetFont(wxSYS_SYSTEM_FONT), *wxWHITE);
    wxDouble textWidth, textHeight;
    gc.GetTextExtent(text, &textWidth, &textHeight);
    gc.DrawText(text, textPosition.m_x - textWidth * 0.5, textPosition.m_y - textHeight * 0.5, textAngle);
}
void Graphics2D::OnSize(wxSizeEvent &event) {
    Refresh();
}
void Graphics2D::drawPoint(wxGraphicsContext &gc, wxPoint2DDouble position, wxDouble diameter, wxColour color) {
    wxGraphicsPath path = gc.CreatePath();
    path.AddCircle(position.m_x, position.m_y, diameter);
    gc.SetBrush(color);
    gc.FillPath(path);
}
