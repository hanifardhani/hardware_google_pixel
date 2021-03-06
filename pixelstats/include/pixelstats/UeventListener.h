/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef HARDWARE_GOOGLE_PIXEL_PIXELSTATS_UEVENTLISTENER_H
#define HARDWARE_GOOGLE_PIXEL_PIXELSTATS_UEVENTLISTENER_H

#include <android-base/chrono_utils.h>
#include <android/frameworks/stats/1.0/IStats.h>

using android::frameworks::stats::V1_0::IStats;
using android::frameworks::stats::V1_0::UsbPortOverheatEvent;

namespace android {
namespace hardware {
namespace google {
namespace pixel {

/**
 * A class to listen for uevents and report reliability events to
 * the PixelStats HAL.
 * Runs in a background thread if created with ListenForeverInNewThread().
 * Alternatively, process one message at a time with ProcessUevent().
 */
class UeventListener {
  public:
    UeventListener(
            const std::string audio_uevent,
            const std::string overheat_path =
                    "/sys/devices/platform/soc/soc:google,overheat_mitigation",
            const std::string charge_metrics_path = "/sys/class/power_supply/battery/charge_stats");

    bool ProcessUevent();  // Process a single Uevent.
    void ListenForever();  // Process Uevents forever
  private:
    bool ReadFileToInt(const std::string &path, int *val);
    bool ReadFileToInt(const char *path, int *val);
    void ReportMicStatusUevents(const char *devpath, const char *mic_status);
    void ReportMicBrokenOrDegraded(const int mic, const bool isBroken);
    void ReportUsbPortOverheatEvent(const char *driver);
    void ReportChargeStats(sp<IStats> &stats_client, const char *line);
    void ReportVoltageTierStats(sp<IStats> &stats_client, const char *line);
    void ReportChargeMetricsEvent(const char *driver);

    const std::string kAudioUevent;
    const std::string kUsbPortOverheatPath;
    const std::string kChargeMetricsPath;
    // Proto messages are 1-indexed and VendorAtom field numbers start at 2, so
    // store everything in the values array at the index of the field number
    // -2.
    const int kVendorAtomOffset = 2;

    int uevent_fd_;
};

}  // namespace pixel
}  // namespace google
}  // namespace hardware
}  // namespace android

#endif  // HARDWARE_GOOGLE_PIXEL_PIXELSTATS_UEVENTLISTENER_H
