#include "imagepanel.h"


ImagePanel::ImagePanel(wxWindow *parent, wxBitmap bitmap) : wxPanel(parent, wxID_ANY){
    m_bitmap = bitmap;
    this->SetSize(300,200);
    Bind(wxEVT_PAINT, &ImagePanel::OnDraw, this);
}
void ImagePanel::OnDraw(wxPaintEvent& evt) {
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

    // Get window size
    wxSize widgetSize;
    widgetSize = GetClientSize();
    wxDouble width, height;
    width = widgetSize.GetWidth();
    height = widgetSize.GetHeight();

    // Create Graphics context
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

    // Set screen offset
    wxDouble screenHorizontalOffset, screenVerticalOffset;
    screenHorizontalOffset = screenVerticalOffset = FromDIP(25.0);

    // Transformation Matrix
    wxAffineMatrix2D T;
    T.Translate(screenHorizontalOffset, screenVerticalOffset);
    wxDouble scale = std::min((width-2*screenHorizontalOffset)/2.0,(height-2*screenVerticalOffset)/2.0);
    T.Scale(scale, -scale);
    T.Translate(1, -1);

    if(gc)
    {
        // Draw Bitmap
        gc->DrawBitmap(m_bitmap, screenHorizontalOffset, screenVerticalOffset, width-2*screenHorizontalOffset, height-2*screenVerticalOffset);

        // Delete Graphics context
        delete gc;

    }
}
