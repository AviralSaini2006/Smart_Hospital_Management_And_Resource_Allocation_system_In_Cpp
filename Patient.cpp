#include "Patient.h"
#include<iostream>
using namespace std;
       patient::patient(int patient_id,string patient_name,int patient_age,char patient_gender,string diagnosis,int severity,bool emergency_status){
        this->patient_id=patient_id;
        this->patient_age=patient_age;
        this->patient_name=patient_name;
        this->patient_gender=patient_gender;
        this->diagnosis=diagnosis;
        this->severity=severity;
        this->emergency_status=emergency_status;
       }
    int patient::GetPatientID(){
        return patient_id;
    }
    int patient::GetPatientAge(){
        return patient_age;
    }
    string patient::GetPatientName(){
        return patient_name;
    }
    char patient::GetPatientGender(){
        return patient_gender;
    }
    string patient::GetPatientDiagnosis(){
        return diagnosis;
    }
    int patient::GetPatientSeverity(){
        return severity;
    }
    bool patient::GetPatientEmergencyStatus(){
        return emergency_status;
    }  
    void patient::PatientDisplay(){
        cout<<"Patient ID : "<<GetPatientID()<<endl;
        cout<<"Patient Name : "<<GetPatientName()<<endl;
        cout<<"Patient Age : "<<GetPatientAge()<<endl;
        cout<<"Patient Gender : "<<GetPatientGender()<<endl;
        cout<<"Patient Diagnosis : "<<GetPatientDiagnosis()<<endl;
        cout<<"Patient Severity : "<<GetPatientSeverity()<<endl;
        cout<<"Patient Emergency Status : "<<(GetPatientEmergencyStatus()?"YES":"NO")<<endl;
    }