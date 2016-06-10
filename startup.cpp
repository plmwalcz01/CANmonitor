#include "startup.h"
#include "View/setuptab.h"
#include "View/mainview.h"
#include "utils.h"
#include "Model/settings.h"
#include "Model/provider.h"
#include "Model/cansocket.h"
#include "Model/device.h"
#include "ViewMgr/setubtabviewmanager.h"
#include "View/controltab.h"
#include "ViewMgr/controltabviewmanager.h"

namespace CANMonitor
{
    Startup::Startup() :
        QObject(nullptr),
        mSetupTab(*new SetupTab(nullptr)),
        mControlTab(*new ControlTab(nullptr)),
        mMainView(*new MainView(nullptr, mSetupTab, mControlTab)),
        mDevicePtr(new Device(this)),
        mSetupViewMgr(new SetuptabViewManager(this, mSetupTab, *mDevicePtr, provider::GetSettingsAsSingleton())),
        mControlTabMgr(new ControlTabViewManager(this, mControlTab, *mDevicePtr, provider::GetSettingsAsSingleton()))
    {
        mDevicePtr->Disconnect();
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
