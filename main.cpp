#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int main() {
    ofstream socLog("soc_log.csv");
    socLog << "Time,SOC,Voltage,Temperature\n";

    ifstream file("TripA01-selected-columns.csv");
    if (!file.is_open()) {
        cout << "Error: Unable to open input file." << endl;
        return 1;
    }

    string line;
    getline(file, line); // Skip header row

    // Battery parameters (documented in README)
    double batteryCapacity_Ah = 60.0;
    double soc = 100.0;
    double chargeUsed_Ah = 0.0;

    // Safety thresholds
    double MIN_VOLTAGE = 300.0;
    double MAX_VOLTAGE = 420.0;
    double MAX_TEMP = 45;

    double prevTime = -1;
    int rowCount = 0;
    int faultCount = 0;
    double minVoltage = 9999, maxVoltage = -9999;
    double minTemp = 9999, maxTemp = -9999;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;

        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }

        if (row.size() < 10) continue; // Skip incomplete rows

        double time = stod(row[0]);
        double voltage = stod(row[7]);
        double current = stod(row[8]);
        double temperature = stod(row[9]);

        // Coulomb Counting: update SOC based on current and elapsed time
        if (prevTime >= 0) {
            double dt = time - prevTime;
            double dt_hours = dt / 3600.0;
            chargeUsed_Ah += (-current) * dt_hours; // Negative current indicates discharge

            soc = 100.0 - (chargeUsed_Ah / batteryCapacity_Ah * 100.0);
            if (soc > 100) soc = 100;
            if (soc < 0) soc = 0;
        }
        prevTime = time;

        // Fault detection based on safety thresholds
        bool fault = false;
        string faultMsg = "";

        if (voltage < MIN_VOLTAGE) {
            fault = true;
            faultMsg += "[UNDER-VOLTAGE] ";
        }
        if (voltage > MAX_VOLTAGE) {
            fault = true;
            faultMsg += "[OVER-VOLTAGE] ";
        }
        if (temperature > MAX_TEMP) {
            fault = true;
            faultMsg += "[OVER-TEMPERATURE] ";
        }

        if (fault) {
            faultCount++;
        }

        // Track voltage and temperature range
        if (voltage < minVoltage) minVoltage = voltage;
        if (voltage > maxVoltage) maxVoltage = voltage;
        if (temperature < minTemp) minTemp = temperature;
        if (temperature > maxTemp) maxTemp = temperature;

        socLog << time << "," << soc << "," << voltage << "," << temperature << "\n";

        // Print progress every 1000 rows
        if (rowCount % 1000 == 0) {
            cout << "[Progress] Time: " << time << "s | SOC: " << soc << "%" << endl;
        }

        rowCount++;
    }

    file.close();
    socLog.close();

    // Classify overall battery health status
    string healthStatus;
    if (soc > 80) healthStatus = "Healthy";
    else if (soc > 50) healthStatus = "Moderate";
    else healthStatus = "Low";

    // Calculate average discharge rate
    double totalSOCDrop = 100.0 - soc;
    double dischargeRate = totalSOCDrop / (prevTime / 100.0); // % per 100 seconds

    cout << "\n---- Summary ----" << endl;
    cout << "Total rows processed: " << rowCount << endl;
    cout << "Final SOC: " << soc << "%" << endl;
    cout << "Total faults detected: " << faultCount << endl;
    cout << "Voltage range: " << minVoltage << "V to " << maxVoltage << "V" << endl;
    cout << "Temperature range: " << minTemp << "C to " << maxTemp << "C" << endl;
    cout << "Battery Health Status: " << healthStatus << endl;
    cout << "Discharge Rate: " << dischargeRate << "% per 100 seconds" << endl;

    return 0;
}
