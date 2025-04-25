#ifndef CONFIGKEYS_H
#define CONFIGKEYS_H

namespace MyConfigKeys {
// --- Server ---
static constexpr const char* ServerIp = "server/ip";
static constexpr const char* ServerPort = "server/port";

// --- Level values ---
static constexpr const char* OverloadingValue = "levels/overloading";
static constexpr const char* WarningValue     = "levels/warning";

// --- Weights ---
static constexpr const char* WeightRam   = "weights/ram";
static constexpr const char* WeightSwap  = "weights/swap";
static constexpr const char* WeightCpu   = "weights/cpu";
static constexpr const char* WeightTemp  = "weights/temp";
static constexpr const char* WeightFreq  = "weights/freq";

// --- Temperature bounds ---
static constexpr const char* TempMin = "temperature/min";
static constexpr const char* TempMax = "temperature/max";

// --- Critical thresholds ---
static constexpr const char* CriticalCpuThreshold  = "critical/cpu";
static constexpr const char* CriticalMemThreshold  = "critical/mem";
static constexpr const char* CriticalTempThreshold = "critical/temp";
static constexpr const char* CriticalDurationSecondsThreshold = "critical/duration_seconds";

// --- Overload config ---
static constexpr const char* OverloadCountThreshold        = "overload/count_threshold";
static constexpr const char* OverloadConsecutiveThreshold  = "overload/consecutive_threshold";
static constexpr const char* OverloadDebounceSeconds            = "overload/debounce_seconds";

// --- Warning config ---
static constexpr const char* WarningCountThreshold       = "warning/count_threshold";
static constexpr const char* WarningConsecutiveThreshold = "warning/consecutive_threshold";
static constexpr const char* WarningDebounceSeconds           = "warning/debounce_seconds";

// --- Potential overload ---
static constexpr const char* PotentialOverloadCount = "overload/potential_count";
}

#endif // CONFIGKEYS_H
