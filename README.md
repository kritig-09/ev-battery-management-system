![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen?style=for-the-badge)

# EV Battery Management System (BMS) Simulation

## 🔋 Problem Statement
Electric Vehicle (EV) batteries require continuous monitoring of State of Charge (SOC) and critical safety parameters to ensure safe and reliable operation. This project simulates a core Battery Management System (BMS) module by analyzing real-world EV driving data to estimate battery charge level and detect potential safety faults.

---

## Dataset
This project uses the **Battery and Heating Data in Real Driving Cycles** dataset (TU Munich, available on Kaggle), which contains real-world EV driving data including battery voltage, current, and temperature recorded during actual driving trips.

---

## What this project does
- Reads real EV trip data (10,000+ time-series records)
- Estimates State of Charge (SOC) using the Coulomb Counting method
- Detects safety faults:
  - Over-voltage
  - Under-voltage
  - Over-temperature
- Classifies battery condition (Healthy / Moderate / Low) using estimated SOC values
- Calculates average discharge rate over the trip
- Tracks voltage and temperature ranges throughout the trip
- Logs SOC, voltage, and temperature values for visualization

> **Note:**  
> This project uses estimated **State of Charge (SOC)** as a simplified indicator of battery condition for educational purposes. It should **not** be interpreted as a true **State of Health (SOH)** estimation. Accurate SOH estimation requires battery aging models, capacity fade analysis, and internal resistance measurements over multiple charge-discharge cycles.

---

# 📊 Battery Health Dashboard

### State of Charge (SOC) over Time
![SOC Graph](soc_graph.png)

### Battery Voltage over Time
![Voltage Graph](voltage_graph.png)

### Battery Temperature over Time
![Temperature Graph](temp_graph.png)

### Observations
- SOC decreases steadily from **100% to approximately 94.4%**, representing normal battery discharge during the trip.
- Voltage varies between **380V and 396V**, reflecting changing driving conditions such as acceleration and regenerative braking.
- Battery temperature rises from **21°C to 22°C** before stabilizing within the safe operating range.

---

## Sample Output

![Terminal Output](output_screenshot.png)

```
Total rows processed: 10090
Final SOC: 94.4163%
Total faults detected: 0
Voltage range: 380.42V to 394.47V
Temperature range: 21°C to 22°C
Battery Health Status: Healthy
Discharge Rate: 0.553446% per 100 seconds
```

> **Note:** Battery Health Status shown above is a simplified indicator derived from estimated SOC values for demonstration purposes.

---

## Fault Detection Validation

To verify the fault detection logic, the temperature threshold was temporarily reduced from **45°C to 21.5°C**, forcing the system to detect abnormal operating conditions.

**Result**
- 9,486 fault samples detected successfully
- Confirms that threshold-based fault detection logic functions correctly
- The threshold was restored to **45°C** for the final simulation

---

## Assumptions
- Battery capacity is assumed to be **60 Ah** (typical EV battery pack)
- Safe operating voltage range: **300V–420V**
- Maximum safe battery temperature: **45°C**
- Negative current values indicate battery discharge, consistent with the dataset convention

---

## Tech Stack

| Component | Details |
|-----------|---------|
| Language | C++ |
| Data Source | TU Munich EV Driving Dataset (Kaggle) |
| Core Method | Coulomb Counting for SOC Estimation |
| Output | Terminal-based Battery Management System Simulation |

---

## ⚙️ How to Run

1. Clone this repository

```
git clone https://github.com/kritig-09/ev-battery-management-system.git
```

2. Place `TripA01-selected-columns.csv` in the same directory as `main.cpp`

3. Compile

```
g++ main.cpp -o main
```

4. Run

```
./main
```

---

## Future Improvements
- Open Circuit Voltage (OCV) based SOC correction to reduce Coulomb Counting drift
- True State of Health (SOH) estimation using battery aging models, capacity fade analysis, and internal resistance measurements across multiple trips
- Cell balancing algorithms
- Interactive dashboard for real-time battery monitoring
- CAN Bus integration for communication with EV subsystems

---


