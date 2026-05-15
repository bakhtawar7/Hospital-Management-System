#include <iostream>
#include <fstream>
using namespace std;

// Function prototypes
void displayMenu();
void admitPatient(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount);
void dischargePatient(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount);
void updatePatientRecord(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int patientCount);
void generateSummaryReport(char patientIds[][100], char patientNames[][100], char patientBloodGroups[][100], int patientCount);
void scheduleAppointment(char appointmentPatientIds[][100], char appointmentDoctorNames[][100], char appointmentTimeSlots[][100], int& appointmentCount);
void writeToFile(const char* filename, char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int patientCount);
void readFromFile(const char* filename, char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount);
void copyString(char* destination, const char* source);
bool compareStrings(const char* str1, const char* str2);

int main() {
    const int MAX_LENGTH = 100;
    const int MAX_PATIENTS = 100;
    const int MAX_APPOINTMENTS = 100;

    char patientIds[MAX_PATIENTS][MAX_LENGTH];
    char patientNames[MAX_PATIENTS][MAX_LENGTH];
    char patientAddresses[MAX_PATIENTS][MAX_LENGTH];
    char patientBloodGroups[MAX_PATIENTS][MAX_LENGTH];
    int patientCount = 0;

    char appointmentPatientIds[MAX_APPOINTMENTS][MAX_LENGTH];
    char appointmentDoctorNames[MAX_APPOINTMENTS][MAX_LENGTH];
    char appointmentTimeSlots[MAX_APPOINTMENTS][MAX_LENGTH];
    int appointmentCount = 0;

    // Read data from file into patient arrays
    readFromFile("Hospital.txt", patientIds, patientNames, patientAddresses, patientBloodGroups, patientCount);

    int choice;
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            admitPatient(patientIds, patientNames, patientAddresses, patientBloodGroups, patientCount);
            break;
        case 2:
            dischargePatient(patientIds, patientNames, patientAddresses, patientBloodGroups, patientCount);
            break;
        case 3:
            updatePatientRecord(patientIds, patientNames, patientAddresses, patientBloodGroups, patientCount);
            break;
        case 4:
            generateSummaryReport(patientIds, patientNames, patientBloodGroups, patientCount);
            break;
        case 5:
            scheduleAppointment(appointmentPatientIds, appointmentDoctorNames, appointmentTimeSlots, appointmentCount);
            break;
        case 6:
            cout << "Exiting the program.\n";
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

// Function Definitions

void displayMenu() {
    cout << "\nHospital Management System Menu:\n";
    cout << "1. Admit Patient\n";
    cout << "2. Discharge Patient\n";
    cout << "3. Update Patient Record\n";
    cout << "4. Generate Summary Report\n";
    cout << "5. Schedule Appointment\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

void admitPatient(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount) {
    if (patientCount >= 100) {
        cout << "Patient list is full. Cannot admit more patients.\n";
        return;
    }

    cout << "Enter Patient ID: ";
    cin >> patientIds[patientCount];
    cout << "Enter Patient Name: ";
    cin.ignore();
    cin.getline(patientNames[patientCount], 100);
    cout << "Enter Address: ";
    cin.getline(patientAddresses[patientCount], 100);
    cout << "Enter Blood Group: ";
    cin >> patientBloodGroups[patientCount];

    ofstream outFile("Hospital.txt", ios::app);
    if (outFile.is_open()) {
        outFile << patientIds[patientCount] << "," << patientNames[patientCount] << "," << patientAddresses[patientCount] << "," << patientBloodGroups[patientCount] << "\n";
        outFile.close();
        cout << "Patient admitted successfully.\n";
    }
    else {
        cout << "Error opening file.\n";
    }

    ++patientCount;
}

void dischargePatient(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount) {
    char patientId[100];
    cout << "Enter Patient ID to discharge: ";
    cin >> patientId;

    bool found = false;
    int indexToRemove = -1;

    // Search for the patient in the loaded data
    for (int i = 0; i < patientCount; ++i) {
        if (compareStrings(patientIds[i], patientId)) {
            indexToRemove = i;  // Mark the index of the patient to remove
            found = true;
            break;
        }
    }

    if (found) {
        // Shift remaining patients down to remove the patient from memory
        for (int i = indexToRemove; i < patientCount - 1; ++i) {
            copyString(patientIds[i], patientIds[i + 1]);
            copyString(patientNames[i], patientNames[i + 1]);
            copyString(patientAddresses[i], patientAddresses[i + 1]);
            copyString(patientBloodGroups[i], patientBloodGroups[i + 1]);
        }
        --patientCount;  // Decrease patient count
        cout << "Patient discharged successfully.\n";

        // Write the updated patient data to the file
        writeToFile("Hospital.txt", patientIds, patientNames, patientAddresses, patientBloodGroups, patientCount);
    }
    else {
        cout << "Patient not found.\n";
    }
}

void updatePatientRecord(char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int patientCount) {
    char patientId[100];
    cout << "Enter Patient ID to update: ";
    cin >> patientId;

    bool found = false;
    for (int i = 0; i < patientCount; ++i) {
        if (compareStrings(patientIds[i], patientId)) {
            cout << "Enter new Address: ";
            cin.ignore();
            cin.getline(patientAddresses[i], 100);
            cout << "Enter new Blood Group: ";
            cin >> patientBloodGroups[i];
            found = true;
            cout << "Patient record updated successfully.\n";
            break;
        }
    }

    if (!found) {
        cout << "Patient not found.\n";
    }
}

void generateSummaryReport(char patientIds[][100], char patientNames[][100], char patientBloodGroups[][100], int patientCount) {
    cout << "\nSummary Report:\n";

    if (patientCount == 0) {
        cout << "No patients available to generate a summary report.\n";
        return;
    }

    for (int i = 0; i < patientCount; ++i) {
        if (patientIds[i][0] != '\0' && patientNames[i][0] != '\0' && patientBloodGroups[i][0] != '\0') {
            cout << "ID: " << patientIds[i]
                << ", Name: " << patientNames[i]
                << ", Blood Group: " << patientBloodGroups[i] << "\n";
        }
    }
}

void scheduleAppointment(char appointmentPatientIds[][100], char appointmentDoctorNames[][100], char appointmentTimeSlots[][100], int& appointmentCount) {
    if (appointmentCount >= 100) {
        cout << "Appointment list is full. Cannot schedule more appointments.\n";
        return;
    }

    cout << "Enter Patient ID: ";
    cin >> appointmentPatientIds[appointmentCount];
    cout << "Enter Doctor's Name: ";
    cin.ignore();
    cin.getline(appointmentDoctorNames[appointmentCount], 100);
    cout << "Enter Time Slot: ";
    cin.getline(appointmentTimeSlots[appointmentCount], 100);

    ++appointmentCount;
    cout << "Appointment scheduled successfully.\n";
}

void writeToFile(const char* filename, char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int patientCount) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (int i = 0; i < patientCount; ++i) {
            outFile << patientIds[i] << "," << patientNames[i] << "," << patientAddresses[i] << "," << patientBloodGroups[i] << "\n";
        }
        outFile.close();
    }
    else {
        cout << "Error opening file to write.\n";
    }
}

void readFromFile(const char* filename, char patientIds[][100], char patientNames[][100], char patientAddresses[][100], char patientBloodGroups[][100], int& patientCount) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        while (inFile) {
            inFile.getline(patientIds[patientCount], 100, ',');
            inFile.getline(patientNames[patientCount], 100, ',');
            inFile.getline(patientAddresses[patientCount], 100, ',');
            inFile.getline(patientBloodGroups[patientCount], 100);
            if (inFile) {
                patientCount++;
            }
        }
        inFile.close();
    }
    else {
        cout << "Error opening file to read.\n";
    }
}

void copyString(char* destination, const char* source) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

bool compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return str1[i] == str2[i];
}
