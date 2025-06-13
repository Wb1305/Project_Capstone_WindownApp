import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../../Components"

Rectangle {
    id: settingConfig

    property var preferWidth
    property var preferHeight
    property var fieldWidth: 350

    property var configManager

    // property string serverIP: configManager.serverIp
    // property int serverPort: configManager.serverPort
    // property int overloadingValue: configManager.overloadingValue
    // property int warningValue: configManager.warningValue
    // property double weightRam: configManager.weightRam * 100
    // property double weightSwap: configManager.weightSwap * 100
    // property double weightCpu: configManager.weightCpu * 100
    // property double weightTemp: configManager.weightTemp * 100
    // property double weightFreq: configManager.weightFreq * 100
    // property int tempMin: configManager.tempMin
    // property int tempMax: configManager.tempMax
    // property int criticalCpuThreshold: configManager.criticalCpuThreshold
    // property int criticalMemThreshold: configManager.criticalMemThreshold
    // property int criticalTempThreshold: configManager.criticalTempThreshold
    // property int criticalDurationSecondsThreshold: configManager.criticalDurationSecondsThreshold
    // property int overloadCountThreshold: configManager.overloadCountThreshold
    // property int overloadConsecutiveThreshold: configManager.overloadConsecutiveThreshold
    // property int overloadDebounceSeconds: configManager.overloadDebounceSeconds
    // property int warningCountThreshold: configManager.warningCountThreshold
    // property int warningConsecutiveThreshold: configManager.warningConsecutiveThreshold
    // property int warningDebounceSeconds: configManager.warningDebounceSeconds
    // property int potentialOverloadCount: configManager.potentialOverloadCount

    anchors.fill: parent
    anchors.topMargin: 10
    anchors.leftMargin: 20

    ScrollView {
        anchors.fill: parent

        ColumnLayout {
            Layout.preferredWidth: settingConfig.preferWidth
            Layout.preferredHeight: settingConfig.preferHeight
            spacing: 20
            // --- Server Settings ---
            GroupBox {
                title: "Server Settings"
                font.pixelSize: 15
                Layout.preferredWidth: settingConfig.preferWidth - 50

                RowLayout {

                    spacing: 30
                    Label { text: "Server IP:"; font.pixelSize: 14; font.bold: true }
                    Text {
                        id: ipField
                        text: settingConfig.configManager.serverIp
                        font.pixelSize: 14
                        color: "gray"
                        font.bold: true
                    }

                    ConfigField{
                        id: portField
                        labelText: "Server Port:"
                        value: settingConfig.configManager.serverPort
                        maxValue: 9999
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setServerPort(parseInt(portField.value))
                        }
                    }
                }
            }

            // --- Weight Settings ---
            GroupBox {
                title: "Resource Weights"
                font.pixelSize: 15
                Layout.preferredWidth: settingConfig.preferWidth - 50

                GridLayout {
                    columnSpacing: 30
                    rowSpacing: 20
                    rows: 3
                    columns: 3

                    flow: GridLayout.LeftToRight

                    ConfigField{
                        id: weightRamField
                        labelText: "RAM Weight (%):"
                        // value: 35
                        value: settingConfig.configManager.weightRam * 100
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWeightRam(parseFloat(weightRamField.value) / 100)
                        }
                    }

                    ConfigField{
                        id: weightSwapField
                        labelText: "Swap Weight (%):"
                        // value: 25
                        value: settingConfig.configManager.weightSwap * 100
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWeightSwap(parseFloat(weightSwapField.value) / 100)
                        }
                    }

                    ConfigField{
                        id: weightCpuField
                        labelText: "CPU Weight (%):"
                        // value: 25
                        value: settingConfig.configManager.weightCpu * 100
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWeightCpu(parseFloat(weightCpuField.value) / 100)
                        }
                    }

                    ConfigField{
                        id: weightTempField
                        labelText: "Temperature Weight (%):"
                        // value: 10
                        value: settingConfig.configManager.weightTemp * 100
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWeightTemp(parseFloat(weightTempField.value) / 100)
                        }
                    }

                    ConfigField{
                        id: weightFreqField
                        labelText: "Frequency Weight (%):"
                        // value: 5
                        value: settingConfig.configManager.weightFreq * 100
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWeightFreq(parseFloat(weightFreqField.value) / 100)
                        }
                    }
                }
            }

            // --- Critical Thresholds ---
            GroupBox {
                title: "Critical Thresholds"
                font.pixelSize: 15
                Layout.preferredWidth: settingConfig.preferWidth - 50

                GridLayout {
                    columnSpacing: 30
                    rowSpacing: 20
                    rows: 3
                    columns: 3

                    flow: GridLayout.LeftToRight

                    ConfigField{
                        id: minTempField
                        labelText: "Temperature Min (°C):"
                        // value: 40
                        value: settingConfig.configManager.tempMin
                        maxValue: 150
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setTempMin(parseInt(minTempField.value))
                        }
                    }

                    ConfigField{
                        id: maxTempField
                        labelText: "Temperature Max (°C):"
                        // value: 90
                        value: settingConfig.configManager.tempMax
                        maxValue: 150
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setTempMax(parseInt(maxTempField.value))
                        }
                    }

                    ConfigField{
                        id: criticalCpuField
                        labelText: "Critical CPU (%):"
                        // value: 85
                        value: settingConfig.configManager.criticalCpuThreshold
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setCriticalCpuThreshold(parseInt(criticalCpuField.value))
                        }
                    }

                    ConfigField{
                        id: criticalMemField
                        labelText: "Critical Mem (%):"
                        // value: 180
                        value: settingConfig.configManager.criticalMemThreshold
                        maxValue: 32768
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setCriticalMemThreshold(parseInt(criticalMemField.value))
                        }
                    }

                    ConfigField{
                        id: criticalTempField
                        labelText: "Critical Temp (°C):"
                        // value: 90
                        value: settingConfig.configManager.criticalTempThreshold
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setCriticalTempThreshold(parseInt(criticalTempField.value))
                        }
                    }

                    ConfigField{
                        id: criticalDurationField
                        labelText: "Critical Duration (s):"
                        // value: 60
                        value: settingConfig.configManager.criticalDurationSecondsThreshold
                        maxValue: 3600
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setCriticalDurationSecondsThreshold(parseInt(criticalDurationField.value))
                        }
                    }
                }
            }

            // --- Score setting ---
            GroupBox{
                title: "Score Settings"
                font.pixelSize: 15
                Layout.preferredWidth: settingConfig.preferWidth - 50

                GridLayout {
                    columnSpacing: 30
                    rowSpacing: 20
                    rows: 3
                    columns: 3
                    flow: GridLayout.LeftToRight

                    ConfigField{
                        id: overloadScoreField
                        labelText: "Overload Score Threshold:"
                        // value: 40
                        value: settingConfig.configManager.overloadingValue
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setOverloadingValue(parseInt(overloadScoreField.value))
                        }
                    }

                    ConfigField{
                        id: warningScoreField
                        labelText: "Warning Score Threshold:"
                        // value: 30
                        value: settingConfig.configManager.warningValue
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWarningValue(parseInt(warningScoreField.value))
                        }
                    }
                }
            }

            // --- Overload & Warning Settings ---
            GroupBox {
                title: "Overload & Warning Settings"
                font.pixelSize: 15
                Layout.preferredWidth: settingConfig.preferWidth - 50

                GridLayout {
                    columnSpacing: 30
                    rowSpacing: 20
                    rows: 3
                    columns: 3
                    flow: GridLayout.LeftToRight

                    ConfigField{
                        id: overloadCountField
                        labelText: "Overload Count Threshold:"
                        // value: 40
                        value: settingConfig.configManager.overloadCountThreshold
                        maxValue: 1000
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setOverloadCountThreshold(parseInt(overloadCountField.value))
                        }
                    }

                    ConfigField{
                        id: overloadConsecutiveField
                        labelText: "Overload Consecutive Threshold:"
                        // value: 30
                        value: settingConfig.configManager.overloadConsecutiveThreshold
                        maxValue: 1000
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setOverloadConsecutiveThreshold(parseInt(overloadConsecutiveField.value))
                        }
                    }

                    ConfigField{
                        id: overloadDebounceField
                        labelText: "Overload Debounce (s):"
                        // value: 10
                        value: settingConfig.configManager.overloadDebounceSeconds
                        maxValue: 3600
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setOverloadDebounceSeconds(parseInt(overloadDebounceField.value))
                        }
                    }

                    ConfigField{
                        id: warningCountField
                        labelText: "Warning Count Threshold:"
                        // value: 20
                        value: settingConfig.configManager.warningCountThreshold
                        maxValue: 1000
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWarningCountThreshold(parseInt(warningCountField.value))
                        }
                    }

                    ConfigField{
                        id: warningConsecutiveField
                        labelText: "Warning Consecutive Threshold:"
                        // value: 15
                        value: settingConfig.configManager.warningConsecutiveThreshold
                        maxValue: 1000
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWarningConsecutiveThreshold(parseInt(warningConsecutiveField.value))
                        }
                    }

                    ConfigField{
                        id: warningDebounceField
                        labelText: "Warning Debounce (s):"
                        // value: 10
                        value: settingConfig.configManager.warningDebounceSeconds
                        maxValue: 3600
                        preferredWidth: settingConfig.fieldWidth
                        onValueChanged:{
                            settingConfig.configManager.setWarningDebounceSeconds(parseInt(warningDebounceField.value))
                        }
                    }

                    // ConfigField{
                    //     id: potentialOverloadField
                    //     labelText: "Potential Overload Count:"
                    //     // value: 45
                    //     value: settingConfig.configManager.potentialOverloadCount
                    //     maxValue: 1000
                    //     preferredWidth: settingConfig.fieldWidth
                    //     onValueChanged:{
                    //         settingConfig.configManager.setPotentialOverloadCount(parseInt(potentialOverloadField.value))
                    //     }
                    // }
                }
            }

            // --- Save Button ---

            CustomButton{
                buttonName: "Save"
                Layout.alignment: Qt.AlignHCenter
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        // Save data logic here
                        console.log("[UI] Saved configuration")
                        settingConfig.configManager.save();
                    }
                }
            }
        }
    }
}

