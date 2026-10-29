#include"Hospital.h"
#include<iostream>
#include <cstdlib>
using namespace std;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
void patientMenu(hospital &h) {
    int choice;

    do {
        cout << "\n===== PATIENT SERVICES =====\n";
        cout << "1. Register Patient\n";
        cout << "2. Process Next Patient\n";
        cout << "3. Discharge Patient\n";
        cout << "4. Transfer Patient Bed\n";
        cout << "5. Show All Patients\n";
        cout << "6. Show Admitted Patients\n";
        cout << "7. Show Waiting Queue\n";
        cout << "8. Complete Doctor Assessment\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id, age;
                string name;
                char gender;

                cout << "Patient ID: ";
                cin >> id;

                cout << "Name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Age: ";
                cin >> age;

                cout << "Gender: ";
                cin >> gender;

                cout << "Required Doctor Specialisation (or NULL for any): ";
                string req;
                cin >> req;
                patient p(id, name, age, gender, "NULL", -1, false, req);
                h.registerPatient(p);
                break;
            }

            case 2: {
    int id, severity;

    cout << "Patient ID for triage: ";
    cin >> id;

    cout << "Severity (1-10): ";
    cin >> severity;

    h.initialTriage(id, severity);
    break;
}

            case 3: {
                int id;

                cout << "Patient ID to discharge: ";
                cin >> id;

                h.dischargePatientById(id);
                h.processDoctorWaitingQueue();
                break;
            }

            case 4: {
                int patientId, bedId;

                cout << "Patient ID: ";
                cin >> patientId;

                cout << "New bed ID: ";
                cin >> bedId;

                h.transferBed(patientId, bedId);
                break;
            }

            case 5:
                h.showPatients();
                break;

            case 6:
                h.showAdmitted();
                break;

            case 7:
                h.showQueue();
                break;
            case 8: {
    int id;
    string diagnosis;

    cout << "Patient ID for assessment: ";
    cin >> id;

    cout << "Diagnosis: ";
    cin >> ws;
    getline(cin, diagnosis);

    h.completeAssessment(id, diagnosis);
    break;
}

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}
void hospitalSetupMenu(hospital &h) {
    int choice;

    do {
        cout << "\n===== HOSPITAL SETUP =====\n";
        cout << "1. Register Doctor\n";
        cout << "2. Register Bed\n";
        cout << "3. Show Doctors\n";
        cout << "4. Show All Beds\n";
        cout << "5. Show Free Beds\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int id, age;
                string name, specialization;
                char gender;

                cout << "Doctor ID: ";
                cin >> id;

                cout << "Name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Age: ";
                cin >> age;

                cout << "Gender: ";
                cin >> gender;

                cout << "Specialization: ";
                cin >> ws;
                getline(cin, specialization);

                doctor d(id, name, age, gender,true, specialization);

                h.registerDoctor(d);
                h.processDoctorWaitingQueue();
                break;
            }

            case 2: {
                int id;
                string ward;

                cout << "Bed ID: ";
                cin >> id;

                cout << "Ward type (ICU/General): ";
                cin >> ward;

                bed b(id, ward, true, -1);
                h.registerBed(b);
                break;
            }

            case 3:
                h.showDoctors();
                break;

            case 4:
                h.showBeds();
                break;

            case 5:
                h.showFreeBeds();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}
void appointmentMenu(hospital &h) {
    int choice;

    do {
        cout << "\n===== APPOINTMENTS =====\n";
        cout << "1. Book Appointment\n";
        cout << "2. Cancel Appointment\n";
        cout << "3. Complete Appointment\n";
        cout << "4. Show Appointments\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1: {
                int patientId, doctorId;
                string dateTime;

                cout << "Patient ID: ";
                cin >> patientId;

                cout << "Doctor ID: ";
                cin >> doctorId;

                cout << "Date and time: ";
                cin >> ws;
                getline(cin, dateTime);

                h.bookAppointment(patientId, doctorId, dateTime);
                break;
            }

            case 2: {
                int id;

                cout << "Appointment ID to cancel: ";
                cin >> id;

                h.cancelAppointment(id);
                break;
            }

            case 3: {
                int id;

                cout << "Appointment ID to complete: ";
                cin >> id;

                h.completeAppointment(id);
                break;
            }

            case 4:
                h.showAppointments();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}
void reportsMenu(hospital &h) {
    int choice;

    do {
        cout << "\n===== REPORTS =====\n";
        cout << "1. Patient List\n";
        cout << "2. Doctor List\n";
        cout << "3. Bed Status\n";
        cout << "4. Waiting Queue\n";
        cout << "5. Admitted Patients\n";
        cout << "6. Ward Summary\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                h.showPatients();
                break;

            case 2:
                h.showDoctors();
                break;

            case 3:
                h.showBeds();
                break;

            case 4:
                h.showQueue();
                break;

            case 5:
                h.showAdmitted();
                break;

            case 6:
                h.showWardSummary();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}
void dataMenu(hospital &h) {
    int choice;

    do {
        cout << "\n===== DATA MANAGEMENT =====\n";
        cout << "1. Save All Hospital Data\n";
        cout << "2. Load All Hospital Data\n";
        cout << "3. Save Appointments\n";
        cout << "4. Load Appointments\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                h.saveData();
                break;

            case 2:
                h.loadData();
                break;

            case 3:
                h.saveAppointments();
                break;

            case 4:
                h.loadAppointments();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}
using namespace std;
int main() {
    hospital h("MediAlloc");
    int choice;

    do {
        cout << "\n========== MediAlloc ==========\n";
        cout << "1. Patient Services\n";
        cout << "2. Hospital Setup\n";
        cout << "3. Appointments\n";
        cout << "4. Reports\n";
        cout << "5. Save / Load Data\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                patientMenu(h);
                break;

            case 2:
                hospitalSetupMenu(h);
                break;

            case 3:
                appointmentMenu(h);
                break;

            case 4:
                reportsMenu(h);
                break;

            case 5:
                dataMenu(h);
                break;

case 0:
                cout << "Exiting MediAlloc. Goodbye!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}