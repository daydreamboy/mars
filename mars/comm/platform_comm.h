// Tencent is pleased to support the open source community by making Mars available.
// Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.

// Licensed under the MIT License (the "License"); you may not use this file except in
// compliance with the License. You may obtain a copy of the License at
// http://opensource.org/licenses/MIT

// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied. See the License for the specific language governing permissions and
// limitations under the License.

/*
 * platform_comm.h
 *
 *  Created on: 2012-11-2
 *      Author: yerungui
 */

#ifndef COMM_PLATFORM_COMM_H_
#define COMM_PLATFORM_COMM_H_

#include <functional>
#include <memory>
#include <string>

#ifdef ANDROID
#include "mars/comm/thread/mutex.h"
#endif

#ifndef __cplusplus
#error "C++ only"
#endif

namespace mars {
namespace comm {

enum NetType { kNoNet = -1, kWifi = 1, kMobile = 2, kOtherNet = 3 };

void OnPlatformNetworkChange();

NetType getNetInfo(bool realtime = false);

enum NetTypeForStatistics {
    NETTYPE_NON = -1,
    NETTYPE_NOT_WIFI = 0,
    NETTYPE_WIFI = 1,
    NETTYPE_WAP = 2,
    NETTYPE_2G = 3,
    NETTYPE_3G = 4,
    NETTYPE_4G = 5,
    NETTYPE_UNKNOWN = 6,  // ignore, DO NOT reuse
    NETTYPE_5G = 7,
};

NetTypeForStatistics getNetTypeForStatistics();
enum ENewNetType {
    NEW_NETTYPE_UNKNOW = 0,
    NEW_NETTYPE_WIFI = 1,
    NEW_NETTYPE_2G = 2,
    NEW_NETTYPE_3G = 3,
    NEW_NETTYPE_4G = 4,
    NEW_NETTYPE_5G = 5,
};
inline ENewNetType getAppNetType() {
    NetTypeForStatistics type = getNetTypeForStatistics();
    switch (type) {
        case NetTypeForStatistics::NETTYPE_WIFI:
            return NEW_NETTYPE_WIFI;
        case NetTypeForStatistics::NETTYPE_2G:
            return NEW_NETTYPE_2G;
        case NetTypeForStatistics::NETTYPE_3G:
            return NEW_NETTYPE_3G;
        case NetTypeForStatistics::NETTYPE_4G:
            return NEW_NETTYPE_4G;
        case NetTypeForStatistics::NETTYPE_5G:
            return NEW_NETTYPE_5G;
        default:
            return NEW_NETTYPE_UNKNOW;
    }
}

struct WifiInfo {
    std::string ssid;
    std::string bssid;
};

bool getCurWifiInfo(WifiInfo& _wifi_info, bool _force_refresh = false);

struct SIMInfo {
    std::string isp_code;
    std::string isp_name;
};
bool getCurSIMInfo(SIMInfo& _sim_info, bool realtime = false);

struct APNInfo {
    APNInfo() : nettype(kNoNet - 1), sub_nettype(0), extra_info("") {
    }
    int nettype;
    int sub_nettype;
    std::string extra_info;
};

bool getAPNInfo(APNInfo& info);
#if __cplusplus >= 201103L
#define __CXX11_CONSTEXPR__ constexpr
#else
#define __CXX11_CONSTEXPR__
#endif

__CXX11_CONSTEXPR__ static const char* const GPRS = "GPRS";
__CXX11_CONSTEXPR__ static const char* const Edge = "Edge";
__CXX11_CONSTEXPR__ static const char* const WCDMA = "WCDMA";
__CXX11_CONSTEXPR__ static const char* const HSDPA = "HSDPA";
__CXX11_CONSTEXPR__ static const char* const HSUPA = "HSUPA";
__CXX11_CONSTEXPR__ static const char* const CDMA1x = "CDMA1x";
__CXX11_CONSTEXPR__ static const char* const CDMAEVDORev0 = "CDMAEVDORev0";
__CXX11_CONSTEXPR__ static const char* const CDMAEVDORevA = "CDMAEVDORevA";
__CXX11_CONSTEXPR__ static const char* const CDMAEVDORevB = "CDMAEVDORevB";
__CXX11_CONSTEXPR__ static const char* const eHRPD = "eHRPD";
__CXX11_CONSTEXPR__ static const char* const LTE = "LTE";
__CXX11_CONSTEXPR__ static const char* const UMTS = "UMTS";
__CXX11_CONSTEXPR__ static const char* const CDMA = "CDMA";
__CXX11_CONSTEXPR__ static const char* const HSPA = "HSPA";
__CXX11_CONSTEXPR__ static const char* const IDEN = "IDEN";
__CXX11_CONSTEXPR__ static const char* const HSPAP = "HSPA+";
__CXX11_CONSTEXPR__ static const char* const G5 = "5G";
__CXX11_CONSTEXPR__ static const char* const NR = "NR";
__CXX11_CONSTEXPR__ static const char* const NRNSA = "NRNSA";
__CXX11_CONSTEXPR__ static const char* const WIFI = "WIFI";

struct RadioAccessNetworkInfo {
    /***
        一、制式演进路线

        1.GSM - WCDMA R99 - LTE FDD路线
        2G       GSM
        2.5G    GPRS
        2.75G  EDGE
        3G       WCDMA R99
        3.5G    HSPA（分HSUPA和HSDPA）
        3.75G  HSPA+（分HSUPA+和HSDPA+）
        3.9G    LTE FDD

        2.CDMA IS 95 - CDMA 2000 EV-DO Rev.A - LTE FDD路线
        2G       CDMA IS 95
        2.5G    CDMA 1X
        2.75G  CDMA 2000 EV-DO Rev.0
        3G       CDMA 2000 EV-DO Rev.A
        3.5G    跳过
        3.75G  跳过
        3.9G    LTE FDD

        3.CDMA IS 95 - CDMA 2000 EV-DO Rev.A - UMB路线
        2G       CDMA IS 95
        2.5G    CDMA 1X
        2.75G  CDMA 2000 EV-DO Rev.0
        3G       CDMA 2000 EV-DO Rev.A
        3.5G    CDMA 2000 EV-DO Rev.B
        3.75G  跳过
        3.9G    UMB

        4.GSM - TD-SCDMA - LTE TDD路线
        2G       GSM
        2.5G    跳过
        2.75G  跳过
        3G       TD-SCDMA
        3.5G    TD-HSPA（分TD-HSUPA和TD-HSDPA）
        3.75G  TD-HSPA+（分TD-HSUPA+和TD-HSDPA+）
        3.9G    LTE TDD


        二、路线点评
        1.GSM - WCDMA R99 - LTE FDD路线
        iPhone以及大部分主流手机的支持制式路线，使用率高达70%，现中国联通路线，技术成熟。为欧洲提议，全球流行。

        2.CDMA IS 95 - CDMA 2000 EV-DO Rev.A - LTE FDD／UMB路线
        由2G非主流CDMA演变而来，技术稳定，使用率高达20%，现中国电信路线。美国高通提议，LTE
    FDD转接部分为摩托罗拉公司提议。美日韩流行。

        3.GSM - TD-SCDMA - LTE TDD路线
        与第一个路线相反，它采用了TD时分技术，不可能和FD频分技术融合。技术新兴，使用率高达10%，兼容终端不多，现中国移动路线，中国大唐提议，中国流行，受Govements看好，支持。

        三、各制式点评介绍
        2G：
        GSM：最基本、主流的技术，无话可说。（中国除133、153、180、189号段外使用的技术）
        CDMA IS 95：最基本、辐射低的技术，无话可说。（中国133、153、180、189号段使用的技术）

        2.5G：
        GPRS：GSM演进走出的中央的一步。（中国除133、153、180、189号段外使用的技术）
        CDMA 1X：CDMA提供数据服务重要的一步。（中国133、153、180、189号段使用的技术）

        2.75G：
        EDGE：被人们无视的一种重要技术，90%机油没用gprs服务，而是这个服务。（中国除133、153、180、189号段外使用的技术）
        CDMA 2000 EV-DO Rev.0：被电信强烈无视的一种技术。（中国133、153、180、189号段使用的技术）

        3G：
        WCDMA：传说中最强的技术，上下行皆是其它3G技术的2倍多！（中国185、186号段使用的技术）
        CDMA 2000 EV-DO Rev.A：电信寄予它便宜的资本，用便宜的价格售出，无疑性价比超高！（中国189、180号段使用的技术）
        TD-SCDMA：国货，外货（基本除了国产和移动定制很少终端支持）的来支持它吧，移动服务值得肯定！（中国187、188号段使用的技术）

        3.5G：
        HSDPA：WCDMA的加速技术，主要增加下载速度，可与HSUPA自由调节。（中国未发展）
        HSUPA：WCDMA的加速技术，主要增加上传速度，可与HSDPA自由调节。（中国未发展）
        TD-HSDPA：TD-SCDMA的加速技术，主要增加下载速度，可与TD-HSUPA自由调节。（中国未发展，移动想跳过）
        TD-HSUPA：TD-SCDMA的加速技术，主要增加上传速度，可与TD-HSDPA自由调节。（中国未发展，移动想跳过）
        CDMA 2000 EV-DO Rev.B：CDMA终结技术，稳定+高速，曾经WCDMA的挑衅对象。（中国未发展）

        3.75G：
        HSDPA+：WCDMA的高级加速技术，主要增加下载速度，可与HSUPA（+）自由调节。（中国未发展）
        HSUPA+：WCDMA的高级加速技术，主要增加上传速度，可与HSDPA（+）自由调节。（中国未发展）
        TD-HSDPA+：TD-SCDMA的高级加速技术，主要增加下载速度，可与TD-HSUPA（+）自由调节。（中国未发展，移动想跳过）
        TD-HSUPA+：TD-SCDMA的高级加速技术，主要增加上传速度，可与TD-HSDPA（+）自由调节。（中国未发展，移动想跳过）

        3.9G：
        LTE
    FDD：Wi-Fi终结者，拥有比WLAN快10倍的能力，估速将达到4M／S以上，兼容性强！是CDMA2000和WCDMA歧路同归的重要制式！（中国未发展）
        LTE
    TDD：世博会展出对象，尖端技术，比FDD早发展一年，网速和FDD差不多，但不成熟，兼容性弱！（湖北移动、上海移动已启动试运营，国家大力推广）
    ***/

    std::string radio_access_network;

    bool Is2G() const {
        return radio_access_network == GPRS || radio_access_network == CDMA1x || radio_access_network == Edge
               || radio_access_network == CDMAEVDORev0 || radio_access_network == UMTS || radio_access_network == CDMA;
    }
    bool Is3G() const {
        return radio_access_network == WCDMA || radio_access_network == CDMAEVDORevA || radio_access_network == HSDPA
               || radio_access_network == HSUPA || radio_access_network == CDMAEVDORevB || radio_access_network == eHRPD
               || radio_access_network == HSPAP || radio_access_network == HSPA;
    }
    bool Is4G() const {
        return radio_access_network == LTE;
    }
    bool Is5G() const {
        return radio_access_network == G5;
    }
    bool IsUnknown() const {
        return !Is2G() && !Is3G() && !Is4G() && !Is5G();
    }
    bool IsNR() const {
        return radio_access_network == NR || radio_access_network == NRNSA;
    }
    bool IsWiFi() const {
        return radio_access_network == WIFI;
    }
};

bool getCurRadioAccessNetworkInfo(RadioAccessNetworkInfo& _raninfo);

uint32_t getSignal(bool isWifi);

bool isNetworkConnected();

bool getifaddrs_ipv4_hotspot(std::string& _ifname, std::string& _ifip);

std::function<bool(std::string&)> SetWiFiIdCallBack(std::function<bool(std::string&)> _cb);
void ResetWiFiIdCallBack();
bool IsWiFiIdCallBackExists();

extern const char* kMarsDefaultNetworkIDNetLabel;
extern const char* kMarsNetworkIDNetLabelPrefix;
extern const char* kMarsWifiNetLabelPrefix;
extern const char* kMarsMobileNetLabelPrefix;
extern const char* kMarsDefaultNetLabel;

int getCurrNetLabel(std::string& netInfo);
int getNetworkIDLabel(std::string& netInfo);
int getRealtimeNetLabel(std::string& netInfo);

#ifdef __APPLE__
void FlushReachability();

float publiccomponent_GetSystemVersion();
#endif

class NetworkInfoCallback {
 public:
    NetworkInfoCallback() = default;
    virtual ~NetworkInfoCallback() = default;
    virtual bool getProxyInfo(int& port, std::string& strProxy, const std::string& _host) {
        return false;
    }
    virtual bool getAPNInfo(APNInfo& info) {
        return false;
    }
    virtual NetType getNetInfo(bool realtime) {
        return kNoNet;
    }
    virtual NetTypeForStatistics getNetTypeForStatistics() {
        return NETTYPE_NON;
    }
    virtual bool getCurRadioAccessNetworkInfo(struct RadioAccessNetworkInfo& _info) {
        return false;
    }
    virtual bool getCurWifiInfo(WifiInfo& _wifi_info, bool _force_refresh) {
        return false;
    }
    virtual bool getCurSIMInfo(SIMInfo& _sim_info, bool realtime) {
        return false;
    }
    virtual uint32_t getSignal(bool isWifi) {
        return -1;
    }
    virtual bool isNetworkConnected() {
        return false;
    }
    virtual bool getIfAddrsIpv4HotSpot(std::string& _ifname, std::string& _ifip) {
        return false;
    }
};
void SetNetworkInfoCallback(const std::shared_ptr<NetworkInfoCallback>& _cb);

#ifdef ANDROID
#ifdef NATIVE_CALLBACK
class AlarmCallback {
 public:
    AlarmCallback() = default;
    ~AlarmCallback() = default;
    virtual bool stopAlarm(int64_t id) {
        return false;
    }
    virtual bool startAlarm(int type, int64_t id, int after) {
        return false;
    }
};
void SetAlarmCallback(const std::shared_ptr<AlarmCallback>& _cb);

class WakeUpLockCallback {
 public:
    WakeUpLockCallback() = default;
    ~WakeUpLockCallback() = default;
    virtual void* wakeupLock_new() {
        return nullptr;
    }
    virtual void wakeupLock_delete(void* _object) {
    }
    virtual void wakeupLock_Lock(void* _object) {
    }
    virtual void wakeupLock_Lock_Timeout(void* _object, int64_t _timeout) {
    }
    virtual void wakeupLock_Unlock(void* _object) {
    }
    virtual bool wakeupLock_IsLocking(void* _object) {
        return false;
    }
};
void SetWakeUpLockCallback(const std::shared_ptr<WakeUpLockCallback>& _cb);
#endif  // #ifdef NATIVE_CALLBACK
bool startAlarm(int type, int64_t id, int after);
bool stopAlarm(int64_t id);

void* wakeupLock_new();
void wakeupLock_delete(void* _object);
void wakeupLock_Lock(void* _object);
void wakeupLock_Lock_Timeout(void* _object, int64_t _timeout);
void wakeupLock_Unlock(void* _object);
bool wakeupLock_IsLocking(void* _object);

std::string GetCurrentProcessName();
#endif  // #ifdef ANDROID

}  // namespace comm
}  // namespace mars

inline bool IsApple() {
#if defined(__APPLE__)
    return true;
#else
    return false;
#endif
}
inline bool IsOSX() {
#if !defined(__APPLE__)
    return false;
#else
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    return false;
#else
    return true;
#endif
#endif
}
inline bool IsIOS() {
#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
    return true;
#else
    return false;
#endif
#else
    return false;
#endif
}
inline bool IsWindows() {
#if defined(_WIN32)
    return true;
#else
    return false;
#endif
}
inline bool IsAndroid() {
#if defined(__ANDROID__)
    return true;
#else
    return false;
#endif
}
inline bool IsOHOS() {
#if defined(OHOS)
    return true;
#else
    return false;
#endif
}
inline bool IsLinux() {
#if defined(__ANDROID__) || defined(OHOS) || defined(__APPLE__)
    return false;
#elif defined(LINUX) || defined(__linux) || defined(__linux__) || defined(__LINUX) || defined(__LINUX__)
    return true;
#else
    return false;
#endif
}

#endif /* COMM_PLATFORM_COMM_H_ */
