#include"Patient.h"
#include"Doctor.h"
#include<iostream>
using namespace std;
int main(){
    patient p1(101,"Aviral",20,'M',"Dead",9,false);
    p1.PatientDisplay();
    doctor d1(201,"Rahul",40,'M',true,"Cardiologist");
    d1.DoctorDisplay();
    return 0;
}