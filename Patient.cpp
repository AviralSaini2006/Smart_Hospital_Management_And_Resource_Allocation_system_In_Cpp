
#include "Patient.h"
#include <iostream>
using namespace std;

patient::patient(int patient_id, string patient_name, int patient_age,
                 char patient_gender, string diagnosis, int severity,
                 bool emergency_status, string required_specialisation,
                 bool discharged, bool transferred, string transferHospital) {

    this->patient_id = patient_id;
    this->patient_age = patient_age;
    this->patient_name = patient_name;
    this->patient_gender = patient_gender;
    this->diagnosis = diagnosis;
    this->severity = severity;
    this->emergency_status = emergency_status;
    this->required_specialisation = required_specialisation;

    systolicBP = 0;
    diastolicBP = 0;
    heartRate = 0;
    spo2 = 0;
    respiratoryRate = 0;
    temperature = 0.0;

    this->discharged = discharged;
    this->transferred = transferred;
    this->transferHospital = transferHospital;
}

int patient::GetPatientID() { return patient_id; }
int patient::GetPatientAge() { return patient_age; }
string patient::GetPatientName() { return patient_name; }
char patient::GetPatientGender() { return patient_gender; }
string patient::GetPatientDiagnosis() { return diagnosis; }
int patient::GetPatientSeverity() { return severity; }
bool patient::GetPatientEmergencyStatus() { return emergency_status; }
string patient::GetRequiredSpecialisation() { return required_specialisation; }

void patient::PatientDisplay() {
    cout << "Patient ID : " << GetPatientID() << endl;
    cout << "Patient Name : " << GetPatientName() << endl;
    cout << "Patient Age : " << GetPatientAge() << endl;
    cout << "Patient Gender : " << GetPatientGender() << endl;
    cout << "Patient Diagnosis : " << GetPatientDiagnosis() << endl;
    cout << "Patient Severity : " << GetPatientSeverity() << endl;

    cout << "Patient Emergency Status : "
         << (GetPatientEmergencyStatus() ? "YES" : "NO") << endl;

    if (GetTransferred()) {
        cout << "Patient Status : Transferred\n";
        cout << "Transferred To : " << GetTransferHospital() << endl;
    }
    else {
        cout << "Patient Status : "
             << (GetDischarged() ? "Discharged" : "Active") << endl;
    }

    cout << "Required Specialisation : "
         << GetRequiredSpecialisation() << endl;
}

void patient::setSeverity(int s) {
    severity = s;
}

void patient::setDiagnosis(string d) {
    diagnosis = d;
}

void patient::setEmergencyStatus(bool e) {
    emergency_status = e;
}

void patient::setVitals(int sys, int dia, int hr,
                        int oxygen, int rr, double temp) {
    systolicBP = sys;
    diastolicBP = dia;
    heartRate = hr;
    spo2 = oxygen;
    respiratoryRate = rr;
    temperature = temp;
}

int patient::getSystolicBP() { return systolicBP; }
int patient::getDiastolicBP() { return diastolicBP; }
int patient::getHeartRate() { return heartRate; }
int patient::getSpO2() { return spo2; }
int patient::getRespiratoryRate() { return respiratoryRate; }
double patient::getTemperature() { return temperature; }

bool patient::GetDischarged() {
    return discharged;
}

void patient::setDischarged(bool status) {
    discharged = status;
}

bool patient::GetTransferred() {
    return transferred;
}

void patient::setTransferred(bool status) {
    transferred = status;
}

string patient::GetTransferHospital() {
    return transferHospital;
}

void patient::setTransferHospital(string name) {
    transferHospital = name;
}
