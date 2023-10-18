#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

class ImagePanel : public wxPanel
{
private:
    wxBitmap m_bitmap;
public:
    ImagePanel(wxWindow* parent, wxBitmap bitmap);

    void OnDraw(wxPaintEvent& evt);

};