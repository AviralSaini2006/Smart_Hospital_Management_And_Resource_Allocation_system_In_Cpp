#ifndef DOCTOR_H
#define DOCTOR_H

#include<iostream>
#include<string>

using namespace std;

class doctor{
    private:
       int doctor_id,doctor_age;
       string doctor_name,specialisation;
       bool availability;
       char doctor_gender;
    public:
       doctor(int doctor_id,string doctor_name,int doctor_age,char doctor_gender,bool availability,string specialisation);
       int GetDocID();
       string GetDocName();
       int GetDocAge();
       char GetDocGender();
       bool GetDocAvailability();
       string GetDocSpecialisation();
       void DoctorDisplay();
       void SetDocAvailability(bool status);
};

#endif