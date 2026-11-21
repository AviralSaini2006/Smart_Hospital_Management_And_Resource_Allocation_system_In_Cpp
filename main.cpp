
#include "Hospital.h"
#include "TerminalUI.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;

void patientMenu(hospital &h) {
    while (true) {
        UI::header("PATIENT SERVICES");

        UI::menuItem('1', "Register Patient");
        UI::menuItem('2', "Doctor Assessment");
        UI::menuItem('3', "Discharge Patient");
        UI::menuItem('4', "Transfer Patient Bed");
        UI::menuItem('5', "Show All Patients");
        UI::menuItem('6', "Show Admitted Patients");
        UI::menuItem('7', "Show Waiting Queue");
        UI::menuItem('8', "Show Doctor Waiting Queue");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(8);

        if (choice == 0) return;

        UI::header("PATIENT SERVICES", "PATIENT MANAGEMENT");

        switch (choice) {
            case 1: {
                int id, age;
                string name, req;
                char gender;

                UI::section("PATIENT REGISTRATION");

                cout << "Patient ID: ";
                cin >> id;

                cout << "Name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Age: ";
                cin >> age;

                cout << "Gender (M/F/O): ";
                cin >> gender;

                gender = toupper(gender);

                cout << "Required Doctor Specialisation (or NULL): ";
                getline(cin >> ws, req);

                patient p(id, name, age, gender, "NULL", -1, false, req);
                h.registerPatient(p);
                break;
            }

            case 2: {
                int id, systolicBP, diastolicBP;
                int heartRate, spo2, respiratoryRate;
                double temperature;
                string diagnosis;

                UI::section("DOCTOR ASSESSMENT");

                cout << "Patient ID: ";
                cin >> id;

                cout << "Systolic BP: ";
                cin >> systolicBP;

                cout << "Diastolic BP: ";
                cin >> diastolicBP;

                cout << "Heart Rate (BPM): ";
                cin >> heartRate;

                cout << "SpO2 (%): ";
                cin >> spo2;

                cout << "Respiratory Rate: ";
                cin >> respiratoryRate;

                cout << "Temperature (Celsius): ";
                cin >> temperature;

                cout << "Diagnosis: ";
                cin >> ws;
                getline(cin, diagnosis);

                h.completeAssessment(
                    id, diagnosis, systolicBP, diastolicBP,
                    heartRate, spo2, respiratoryRate, temperature
                );
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

            case 5: h.showPatients(); break;
            case 6: h.showAdmitted(); break;
            case 7: h.showQueue(); break;
            case 8: h.showDoctorWaitingQueue(); break;
        }

        UI::pause();
    }
}

void hospitalSetupMenu(hospital &h) {
    while (true) {
        UI::header("HOSPITAL SETUP");

        UI::menuItem('1', "Register Doctor");
        UI::menuItem('2', "Register Bed");
        UI::menuItem('3', "Show Doctors");
        UI::menuItem('4', "Show All Beds");
        UI::menuItem('5', "Show Free Beds");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(5);

        if (choice == 0) return;

        UI::header("HOSPITAL SETUP", "DOCTOR AND BED MANAGEMENT");

        switch (choice) {
            case 1: {
                int id, age;
                string name, specialization;
                char gender;

                UI::section("DOCTOR REGISTRATION");

                cout << "Doctor ID: ";
                cin >> id;

                cout << "Name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Age: ";
                cin >> age;

                cout << "Gender (M/F/O): ";
                cin >> gender;

                gender = toupper(static_cast<unsigned char>(gender));

                cout << "Specialization: ";
                getline(cin >> ws, specialization);

                doctor d(id, name, age, gender, true, specialization);
                h.registerDoctor(d);
                break;
            }

            case 2: {
                int id;
                string ward;

                UI::section("BED REGISTRATION");

                cout << "Bed ID: ";
                cin >> id;

                cout << "Ward type (ICU/General): ";
                cin >> ward;

for (char &c : ward)
    c = static_cast<char>(
        tolower(static_cast<unsigned char>(c))
    );

if (ward == "icu")
    ward = "ICU";
else if (ward == "general")
    ward = "General";

                bed b(id, ward, true, -1);
                h.registerBed(b);
                break;
            }

            case 3: h.showDoctors(); break;
            case 4: h.showBeds(); break;
            case 5: h.showFreeBeds(); break;
        }

        UI::pause();
    }
}

void appointmentMenu(hospital &h) {
    while (true) {
        UI::header("APPOINTMENT MANAGEMENT");

        UI::menuItem('1', "Book Appointment");
        UI::menuItem('2', "Cancel Appointment");
        UI::menuItem('3', "Complete Appointment");
        UI::menuItem('4', "Show Appointments");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(4);

        if (choice == 0) return;

        UI::header("APPOINTMENTS", "APPOINTMENT MANAGEMENT");

        switch (choice) {
            case 1: {
                int patientId, doctorId;
                string dateTime;

                cout << "Patient ID: ";
                cin >> patientId;

                cout << "Doctor ID: ";
                cin >> doctorId;

                cout << "Date and time (Use DD-MM-YYYY HH:MM) : ";
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
        }

        UI::pause();
    }
}

void reportsMenu(hospital &h) {
    while (true) {
        UI::header("HOSPITAL REPORTS");

        UI::menuItem('1', "Patient List");
        UI::menuItem('2', "Doctor List");
        UI::menuItem('3', "Bed Status");
        UI::menuItem('4', "Waiting Queue");
        UI::menuItem('5', "Admitted Patients");
        UI::menuItem('6', "Ward Summary");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(6);

        if (choice == 0) return;

        UI::header("HOSPITAL REPORTS", "SYSTEM OVERVIEW");

        switch (choice) {
            case 1: h.showPatients(); break;
            case 2: h.showDoctors(); break;
            case 3: h.showBeds(); break;
            case 4: h.showQueue(); break;
            case 5: h.showAdmitted(); break;
            case 6: h.showWardSummary(); break;
        }

        UI::pause();
    }
}

void dataMenu(hospital &h) {
    while (true) {
        UI::header("DATA MANAGEMENT");

        UI::menuItem('1', "Save All Hospital Data");
        UI::menuItem('2', "Load All Hospital Data");
        UI::menuItem('3', "Save Appointments");
        UI::menuItem('4', "Load Appointments");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(4);

        if (choice == 0) return;

        UI::header("DATA MANAGEMENT", "FILE OPERATIONS");

        switch (choice) {
            case 1: h.saveData(); break;
            case 2: h.loadData(); break;
            case 3: h.saveAppointments(); break;
            case 4: h.loadAppointments(); break;
        }

        UI::pause();
    }
}

void partnerHospitalMenu(hospital &h) {
    while (true) {
        UI::header("PARTNER HOSPITAL MANAGEMENT");

        UI::menuItem('1', "Register Partner Hospital");
        UI::menuItem('2', "Add Hospital Route");
        UI::menuItem('3', "Show Partner Hospitals");
        UI::menuItem('4', "Show Hospital Network");
        UI::menuItem('5', "Update ICU Availability");
        UI::menuItem('6', "Show Transfer Records");
        UI::menuItem('0', "Back to Dashboard");

        UI::footer();
        int choice = UI::getChoice(6);

        if (choice == 0) return;

        UI::header("PARTNER HOSPITAL MANAGEMENT", "HOSPITAL NETWORK");

        switch (choice) {
            case 1: {
                int id, beds;
                string name, location, specialization;

                cout << "Hospital ID: ";
                cin >> id;

                cout << "Hospital Name: ";
                cin >> ws;
                getline(cin, name);

                cout << "Location: ";
                getline(cin, location);

                cout << "Specialization: ";
                getline(cin, specialization);

                cout << "Total ICU Beds: ";
                cin >> beds;

                h.registerPartnerHospital(
                    id, name, location, specialization, beds
                );
                break;
            }

            case 2: {
                int fromID, toID, distance;

                cout << "Enter 0 for MediAlloc.\n";

                cout << "Starting Hospital ID: ";
                cin >> fromID;

                cout << "Destination Hospital ID: ";
                cin >> toID;

                cout << "Distance in KM: ";
                cin >> distance;

                h.addHospitalRoute(fromID, toID, distance);
                break;
            }

            case 3:
                h.showPartnerHospitals();
                break;

            case 4:
                h.showHospitalRoutes();
                break;

            case 5: {
                int id, available;

                cout << "Partner Hospital ID: ";
                cin >> id;

                cout << "Currently Available ICU Beds: ";
                cin >> available;

                h.updatePartnerICUAvailability(id, available);
                break;
            }

            case 6:
                h.showTransferRecords();
                break;
        }

        UI::pause();
    }
}

int main() {
    hospital h("MediAlloc");
    int choice;

    UI::init();

    UI::header("STARTING MEDIALLOC");

    cout << "\n   Loading hospital data...\n";

    h.loadData();
    h.loadAppointments();

    cout << "\n   Data loading completed.\n";

    UI::pause();

    while (true) {
        UI::header("MAIN DASHBOARD");

        UI::menuItem('1', "Patient Services");
        UI::menuItem('2', "Hospital Setup");
        UI::menuItem('3', "Appointments");
        UI::menuItem('4', "Hospital Reports");
        UI::menuItem('5', "Save / Load Data");
        UI::menuItem('6', "Partner Hospital Management");
        UI::menuItem('0', "Exit Application");

        UI::footer();
        int choice = UI::getChoice(6);

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

            case 6:
                partnerHospitalMenu(h);
                break;

         case 0:
    UI::header("SAVING DATA");

    cout << "\n   Saving hospital data...\n";
    h.saveData();

    cout << "\n   Saving appointments...\n";
    h.saveAppointments();

    cout << "\n   All data saved successfully.\n";
    cout << "\n   Thank you for using MediAlloc!\n";

    return 0;
        }
    }
}
