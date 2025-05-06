#ifndef OVERLOADCONFIG_H
#define OVERLOADCONFIG_H

struct OverloadConfig{
    int overloadingValue = 0;
    int warningValue = 0;
    double weightRam = 0;
    double weightSwap = 0;
    double weightCpu = 0;
    double weightTemp = 0;
    double weightFreq = 0;
    int tempMin = 0;
    int tempMax = 0;
    int criticalCpuThreshold = 0;
    int criticalMemThreshold = 0;
    int criticalTempThreshold = 0;
    int criticalDurationSecondsThreshold = 0;
    int overloadCountThreshold = 0;
    int overloadConsecutiveThreshold = 0;
    int overloadDebounceSeconds = 0;
    int warningCountThreshold = 0;
    int warningConsecutiveThreshold = 0;
    int warningDebounceSeconds = 0;
    int potentialOverloadCount = 0;
};

#endif // OVERLOADCONFIG_H
