/**
 * \file DeviceList.cpp
 * \brief Device list
 * \author vsalo
 */

#include "AppMgr/DeviceList.h"

namespace NsAppManager
{
    log4cplus::Logger DeviceList::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("DeviceList"));

    /**
     * \brief Default class constructor
     */
    DeviceList::DeviceList()
    {
    }

    /**
     * \brief Default class destructor
     */
    DeviceList::~DeviceList()
    {
    }

    /**
     * \brief set device list
     * \param deviceList device list
     */
    void DeviceList::setDeviceList(const NsConnectionHandler::tDeviceList &deviceList)
    {
        mDeviceList = deviceList;
    }

    /**
     * \brief get device list
     * \return device list
     */
    const NsConnectionHandler::tDeviceList &DeviceList::getDeviceList() const
    {
        return mDeviceList;
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t DeviceList::size() const
    {
        return mDeviceList.size();
    }

    /**
     * \brief Copy constructor
     */
    DeviceList::DeviceList(const DeviceList &)
    {
    }
}
