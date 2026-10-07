#ifndef HOSPITAL_H
#define HOSPITAL_H

#include"Patient.h"
#include"Doctor.h"
#include"Bed.h"
#include<vector>
#include<queue>
#include<utility>
using namespace std;

class hospital{
    private:
       string HospitalName;
       vector<patient> patients;
       vector<doctor> doctors;
       vector<bed> beds;
       // (severity, -index in patients vector): highest severity first, earlier patient first on a tie
       priority_queue<pair<int,int> > waiting_queue;
       vector<pair<int,int> > assignments;   // (patient_id, doctor_id) for patients currently under a doctor
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
       void assignDoctor();       // gives the most severe waiting patient an available doctor
       bool queuePatient(int patient_id,int severity,string diagnosis);   // set severity and push into priority queue
       void showQueue();
}; 

#endif