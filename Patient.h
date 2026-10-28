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
       string required_specialisation;   // doctor specialisation this patient needs ("NULL" = any)
    public:
       patient(int id,string name,int age,char gender,string diagnosis,int sev,bool emergency_status,string required_specialisation);
    int GetPatientID();
    int GetPatientAge();
    string GetPatientName();
    char GetPatientGender();
    string GetPatientDiagnosis();
    int GetPatientSeverity();
    bool GetPatientEmergencyStatus();
    string GetRequiredSpecialisation();
    void PatientDisplay();
    void setSeverity(int s);
    void setDiagnosis(string d);
    void setEmergencyStatus(bool e);
};

#endif