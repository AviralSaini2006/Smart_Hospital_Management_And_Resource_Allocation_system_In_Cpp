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


// find the patient in the vector, set severity/diagnosis, then push into the priority queue
bool hospital::queuePatient(int patient_id,int severity,string diagnosis){
    for(size_t i=0;i<patients.size();i++){
        if(patients[i].GetPatientID()==patient_id){
            if(patients[i].GetPatientSeverity()!=-1){
                cout<<"This patient is already in the queue."<<endl;
                return false;
            }
            patients[i].setSeverity(severity);
            patients[i].setDiagnosis(diagnosis);
            waiting_queue.push(make_pair(severity,-(int)i));
            cout<<"Patient added to the priority queue."<<endl;
            return true;
        }
    }
    cout<<"Patient ID not found."<<endl;
    return false;
}

void hospital::showQueue(){
    if(waiting_queue.empty()){
        cout<<"Queue is empty."<<endl;
        return;
    }
    priority_queue<pair<int,int> > copy=waiting_queue;   // copy, so the real queue stays unchanged
    while(!copy.empty()){
        pair<int,int> t=copy.top();
        copy.pop();
        cout<<patients[-t.second].GetPatientName()<<" (ID "<<patients[-t.second].GetPatientID()
            <<", severity "<<t.first<<")"<<endl;
    }
}

void hospital::assignDoctor(){
    if(waiting_queue.empty()){
        cout<<"No patients are waiting."<<endl;
        return;
    }
    for(size_t i=0;i<doctors.size();i++){
        if(doctors[i].GetDocAvailability()){
            pair<int,int> top=waiting_queue.top();
            waiting_queue.pop();
            patient &p=patients[-top.second];
            doctors[i].SetDocAvailability(false);
            cout<<"Patient "<<p.GetPatientName()<<" (severity "<<top.first<<") assigned to Dr. "
                <<doctors[i].GetDocName()<<" ("<<doctors[i].GetDocSpecialisation()<<")"<<endl;
            return;
        }
    }
    cout<<"No doctor is available. Patient stays in the queue."<<endl;
}
