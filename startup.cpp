#include "startup.h"
#include "View/setuptab.h"
#include "View/mainview.h"
#include "utils.h"
#include "Model/settings.h"
#include "Model/provider.h"
#include "Model/cansocket.h"
#include "Model/device.h"
#include "ViewMgr/setubtabviewmanager.h"

namespace CANMonitor
{
    Startup::Startup() :
        QObject(nullptr),
        mSetupTab(*new SetupTab(nullptr)),
        mMainView(*new MainView(nullptr, mSetupTab)),
        mDevicePtr(new Device(this, *new CANSocket(this) )),
        mSetupViewMgr(new SetuptabViewManager(this, mSetupTab, *mDevicePtr, provider::GetSettingsAsSingleton()))
    {
    }

    Startup::~Startup()
    {
        Utils::DestructorMsg(this);
        delete &mMainView;
    }

    void Startup::show() const
    {
        mMainView.show();
    }
}
