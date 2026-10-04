#include"Patient.h"
#include"Doctor.h"
#include"Bed.h"
#include<iostream>
using namespace std;
int main(){
    patient p1(101,"Aviral",20,'M',"Dead",9,false);
    p1.PatientDisplay();
    doctor d1(201,"Rahul",40,'M',true,"Cardiologist");
    d1.DoctorDisplay();
    bed b1(301,"ICU",true,-1);
    b1.BedDisplay();
    return 0;
}