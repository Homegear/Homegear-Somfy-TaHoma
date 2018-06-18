/* Copyright 2013-2017 Sathya Laufer
 *
 * Homegear is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Homegear is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Homegear.  If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */

#include "Api.h"
#include "GD.h"

namespace MyFamily
{

Api::Api()
{
    _out.init(GD::bl);
    _out.setPrefix(GD::out.getPrefix() + " TaHoma client: ");

    signal(SIGPIPE, SIG_IGN);

    _httpClient = std::unique_ptr<BaseLib::HttpClient>(new BaseLib::HttpClient(GD::bl, "www.tahomalink.com", 443, false, true));
    _jsonEncoder = std::unique_ptr<BaseLib::Rpc::JsonEncoder>(new BaseLib::Rpc::JsonEncoder(GD::bl));
    _jsonDecoder = std::unique_ptr<BaseLib::Rpc::JsonDecoder>(new BaseLib::Rpc::JsonDecoder(GD::bl));

    _loggedIn = false;
}

Api::~Api()
{
}

bool Api::login()
{
    try
    {
        std::lock_guard<std::mutex> loginGuard(_loginMutex);
        if(_loggedIn) return true;

        auto setting = GD::family->getFamilySetting("user");
        if(!setting || setting->stringValue.empty())
        {
            _out.printError("Error: Setting \"user\" not specified in \"somfytahoma.conf\".");
            return false;
        }
        std::string user = BaseLib::HelperFunctions::stringReplace(setting->stringValue, "\"", "\\\"");
        setting = GD::family->getFamilySetting("password");
        if(!setting || setting->stringValue.empty())
        {
            _out.printError("Error: Setting \"password\" not specified in \"somfytahoma.conf\".");
            return false;
        }
        std::string password = BaseLib::HelperFunctions::stringReplace(setting->stringValue, "\"", "\\\"");;

        std::string postData = "userId=" + BaseLib::Http::encodeURL(user) + "&userPassword=" + BaseLib::Http::encodeURL(password);
        std::string postRequest = "POST " + _path + "login HTTP/1.1\r\nUser-Agent: Homegear\r\nHost: www.tahomalink.com:" + std::to_string(443) + "\r\nConnection: Close\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: " + std::to_string(postData.size()) + "\r\n\r\n" + postData;
        BaseLib::Http result;
        _httpClient->sendRequest(postRequest, result);

        if(GD::bl->debugLevel >= 5) _out.printDebug("Debug: Response: " + std::string(result.getContent().data(), result.getContentSize()));

        if(result.getHeader().responseCode == 200)
        {
            auto resultJson = _jsonDecoder->decode(result.getContent());
            auto jsonIterator = resultJson->structValue->find("success");
            if(jsonIterator != resultJson->structValue->end() && jsonIterator->second->booleanValue)
            {
                if(GD::bl->debugLevel >= 5) _out.printDebug("Debug: Successfully logged in.");

                auto fieldIterator = result.getHeader().fields.find("set-cookie");
                if(fieldIterator == result.getHeader().fields.end()) return false;

                _cookie = fieldIterator->second;
                _loggedIn = true;

                return true;
            }

            return false;
        }
        else if(result.getHeader().responseCode >= 400 && result.getHeader().responseCode <= 499)
        {
            auto resultJson = _jsonDecoder->decode(result.getContent());
            auto jsonIterator = resultJson->structValue->find("error");
            if(jsonIterator != resultJson->structValue->end()) GD::out.printError("Error logging in: " + jsonIterator->second->stringValue);
            else GD::out.printError("Error logging in: Unknown error");
        }

        return false;
    }
    catch(const std::exception& ex)
    {
        _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(BaseLib::Exception& ex)
    {
        _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__, ex.what());
    }
    catch(...)
    {
        _out.printEx(__FILE__, __LINE__, __PRETTY_FUNCTION__);
    }
    return false;
}

}