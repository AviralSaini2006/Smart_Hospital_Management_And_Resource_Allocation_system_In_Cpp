#ifndef PATIENT_H
#define PATIENT_H

#include<string>
#include<iostream>

using namespace std;

class patient{
    private:
       int patient_id,patient_age;
       string patient_name,diagnosis;
       char patient_gender;
       int severity;
       bool emergency_status;
    public:
       patient(int id,string name,int age,char gender,string diagnosis,int sev,bool emergency_status);
    int GetPatientID();
    int GetPatientAge();
    string GetPatientName();
    char GetPatientGender();
    string GetPatientDiagnosis();
    int GetPatientSeverity();
    bool GetPatientEmergencyStatus();
    void PatientDisplay();
};

#endif