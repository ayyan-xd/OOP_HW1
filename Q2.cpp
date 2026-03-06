#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
using namespace std;

struct SensorReading {
    int sensorID;
    double value;
    string Location;
    string StringType;
    SensorReading(int id, double val, string loc, string type) {
        sensorID = id;
        value = val;
        Location = loc;
        StringType = type;
    }
    SensorReading() {
        sensorID = -1;
        value = 0.0;
        Location = " ";
        StringType = " ";
    }
};

struct Alert {
    int priority;
    string message;
    string AlertType;
    Alert(int p, string msg, string type) {
        priority = p;
        message = msg;
        AlertType = type;
    }
    Alert() {
        priority = 0;
        message = " ";
        AlertType = " ";
    }
    bool operator<(const Alert& other) const {
        return priority < other.priority;
    }
};

class SensorProcessor {
    unordered_map<int, SensorReading> sensorData;
    unordered_map<string, vector<SensorReading>> locationMap;
    priority_queue<Alert> alertQueue;
public:
    void addSensorReading(SensorReading reading) {
        sensorData[reading.sensorID] = reading;
        locationMap[reading.Location].push_back(reading);
    }
    void addAlert(Alert alert) {
        alertQueue.push(alert);
    }
    void processNextAlert() {
        if (alertQueue.empty()) {
            cout << "No alerts to process." << endl;
            return;
        }
        Alert topAlert = alertQueue.top();
        alertQueue.pop();
        cout << "Processing Alert: " << topAlert.message << " with priority " << topAlert.priority << endl;
    }
    SensorReading getSensorReading(int sensorID) {
        if (sensorData.find(sensorID) != sensorData.end()) {
            return sensorData[sensorID];
        }
        return SensorReading();
    }
    vector<SensorReading> getReadingsByLocation(string location) {
        if (locationMap.find(location) != locationMap.end()) {
            return locationMap[location];
        }
        return vector<SensorReading>();
    }
};

int main() {
    SensorProcessor processor;
    int choice;
    cout << "=== Sensor Processing System ===" << endl;
    while(true) {
        cout << "\n--- Menu ---" << endl;
        cout << "1. Add Sensor Reading" << endl;
        cout << "2. Add Alert" << endl;
        cout << "3. Process Next Alert" << endl;
        cout << "4. Get Sensor Reading by ID" << endl;
        cout << "5. Get Readings by Location" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        if(choice == 1) {
            int sensorID;
            double value;
            string location, type;
            cout << "Enter Sensor ID: ";
            cin >> sensorID;
            cout << "Enter Value: ";
            cin >> value;
            cout << "Enter Location: ";
            cin >> location;
            cout << "Enter Sensor Type: ";
            cin >> type;
            SensorReading reading(sensorID, value, location, type);
            processor.addSensorReading(reading);
            cout << "Sensor reading added successfully!" << endl;
        }
        else if(choice == 2) {
            int priority;
            string message, alertType;
            cout << "Enter Priority: ";
            cin >> priority;
            cin.ignore();
            cout << "Enter Alert Message: ";
            getline(cin, message);
            cout << "Enter Alert Type: ";
            getline(cin, alertType);
            Alert alert(priority, message, alertType);
            processor.addAlert(alert);
            cout << "Alert added successfully!" << endl;
        }
        else if(choice == 3) {
            processor.processNextAlert();
        }
        else if(choice == 4) {
            int sensorID;
            cout << "Enter Sensor ID: ";
            cin >> sensorID;
            SensorReading reading = processor.getSensorReading(sensorID);
            if(reading.sensorID != -1) {
                cout << "Sensor ID: " << reading.sensorID << endl;
                cout << "Value: " << reading.value << endl;
                cout << "Location: " << reading.Location << endl;
                cout << "Type: " << reading.StringType << endl;
            } else {
                cout << "Sensor not found." << endl;
            }
        }
        else if(choice == 5) {
            string location;
            cout << "Enter Location: ";
            cin >> location;
            vector<SensorReading> readings = processor.getReadingsByLocation(location);
            if(readings.empty()) {
                cout << "No readings found for this location." << endl;
            } else {
                cout << "Readings for location '" << location << "':" << endl;
                for(const auto& reading : readings) {
                    cout << "  Sensor ID: " << reading.sensorID 
                         << ", Value: " << reading.value 
                         << ", Type: " << reading.StringType << endl;
                }
            }
        }
        else if(choice == 6) {
            cout << "Exiting program. Goodbye!" << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}
