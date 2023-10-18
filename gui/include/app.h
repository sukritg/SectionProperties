#pragma once
#include "wx/wx.h"

class App : public wxApp{
private:

public:
    virtual bool OnInit() override;
};

wxIMPLEMENT_APP(App);
