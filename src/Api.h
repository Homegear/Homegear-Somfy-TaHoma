/* Copyright 2013-2019 Homegear GmbH
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

#ifndef TAHOMA_API_H
#define TAHOMA_API_H

#include <homegear-base/Sockets/HttpClient.h>
#include <homegear-base/Output/Output.h>
#include <homegear-base/Encoding/JsonEncoder.h>
#include <homegear-base/Encoding/JsonDecoder.h>

namespace MyFamily
{

class Api
{
public:
    Api();
    virtual ~Api();

    void start();
    void stop();
private:
    BaseLib::Output _out;
    std::unique_ptr<BaseLib::HttpClient> _httpClient;
    std::unique_ptr<BaseLib::Rpc::JsonEncoder> _jsonEncoder;
    std::unique_ptr<BaseLib::Rpc::JsonDecoder> _jsonDecoder;

    std::atomic_bool _stopWorkerThread;
    std::thread _workerThread;

    const std::string _path = "/enduser-mobile-web/externalAPI/json/";
    std::mutex _loginMutex;
    std::atomic_bool _loggedIn;
    std::string _cookie;

    bool login();
    void worker();
};

}

#endif
