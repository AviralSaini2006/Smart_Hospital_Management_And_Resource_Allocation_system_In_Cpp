#ifndef HOSPITAL_H
#define HOSPITAL_H

#include"Patient.h"
#include"Doctor.h"
#include"Bed.h"
#include<vector>
using namespace std;

class hospital{
    private:
       string HospitalName;
       vector<patient> patients;
       vector<doctor> doctors;
       vector<bed> beds;
    public:
       hospital(string h):HospitalName(h){}
       void AddPatient(patient p);
       void AddDoctor(doctor d);
       void AddBed(bed b);

       void showPatients();
       void showDoctors();
       void showBeds();

       void allocateBed();
       void dischargePatient();
       void assignDoctor();
}; 

#endif