/* Copyright 2013-2019 Homegear GmbH */

#include "DescriptionCreator.h"
#include "GD.h"

namespace MyFamily
{

DescriptionCreator::DescriptionCreator()
{

}

DescriptionCreator::PeerInfo DescriptionCreator::createDescription(std::string& interfaceId, std::string& serialNumber, uint32_t oldTypeNumber, std::unordered_set<uint32_t>& knownTypeNumbers)
{
    try
    {
        createDirectories();
    }
    catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }

    return PeerInfo();
}

void DescriptionCreator::createDirectories()
{
    try
    {
        uid_t localUserId = GD::bl->hf.userId(GD::bl->settings.dataPathUser());
        gid_t localGroupId = GD::bl->hf.groupId(GD::bl->settings.dataPathGroup());
        if(((int32_t)localUserId) == -1 || ((int32_t)localGroupId) == -1)
        {
            localUserId = GD::bl->userId;
            localGroupId = GD::bl->groupId;
        }

        std::string path1 = GD::bl->settings.familyDataPath();
        std::string path2 = path1 + std::to_string(GD::family->getFamily()) + "/";
        _xmlPath = path2 + "desc/";
        if(!BaseLib::Io::directoryExists(path1)) BaseLib::Io::createDirectory(path1, GD::bl->settings.dataPathPermissions());
        if(localUserId != 0 || localGroupId != 0)
        {
            if(chown(path1.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + path1);
            if(chmod(path1.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + path1);
        }
        if(!BaseLib::Io::directoryExists(path2)) BaseLib::Io::createDirectory(path2, GD::bl->settings.dataPathPermissions());
        if(localUserId != 0 || localGroupId != 0)
        {
            if(chown(path2.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + path2);
            if(chmod(path2.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + path2);
        }
        if(!BaseLib::Io::directoryExists(_xmlPath)) BaseLib::Io::createDirectory(_xmlPath, GD::bl->settings.dataPathPermissions());
        if(localUserId != 0 || localGroupId != 0)
        {
            if(chown(_xmlPath.c_str(), localUserId, localGroupId) == -1) GD::out.printWarning("Could not set owner on " + _xmlPath);
            if(chmod(_xmlPath.c_str(), GD::bl->settings.dataPathPermissions()) == -1) GD::out.printWarning("Could not set permissions on " + _xmlPath);
        }
    }
    catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

void DescriptionCreator::addParameterSet(std::shared_ptr<Tahoma>& interface, std::shared_ptr<HomegearDevice>& device, std::string& serialNumber, int32_t channel, std::string& type)
{
    try
    {

    }
    catch(const std::exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        GD::out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
}

}
