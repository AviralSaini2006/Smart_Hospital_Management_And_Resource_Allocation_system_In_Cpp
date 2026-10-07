#include"Hospital.h"
using namespace std;

       void hospital::AddPatient(patient p){
          patients.push_back(p);
       }
       void hospital::AddDoctor(doctor d){
          doctors.push_back(d);
       }
       void hospital::AddBed(bed b){
          beds.push_back(b);
       }

       void hospital::showPatients(){
           for(patient p : patients){
            p.PatientDisplay();
           }
       }
       void hospital::showDoctors(){
        for(doctor d:doctors){
            d.DoctorDisplay();
        }
       }
       void hospital::showBeds(){
        for(bed b:beds){
            b.BedDisplay();
        }
    }
