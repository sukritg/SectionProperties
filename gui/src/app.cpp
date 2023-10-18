#include "app.h"
#include "mainwnd.h"

bool App::OnInit()
{
#ifdef WIN32
    wxInitAllImageHandlers();
#endif
    auto *mainWindow = new MainWindow();
    mainWindow->Show(true);
    return true;
}

