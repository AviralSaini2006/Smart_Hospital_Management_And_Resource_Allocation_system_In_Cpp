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
    if(severity<1||severity>10){
        cout<<"Severity must be between 1 and 10."<<endl;
        return false;
    }
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
            assignments.push_back(make_pair(p.GetPatientID(),doctors[i].GetDocID()));
            cout<<"Patient "<<p.GetPatientName()<<" (severity "<<top.first<<") assigned to Dr. "
                <<doctors[i].GetDocName()<<" ("<<doctors[i].GetDocSpecialisation()<<")"<<endl;
            return;
        }
    }
    cout<<"No doctor is available. Patient stays in the queue."<<endl;
}


// gives a bed to every patient who has a doctor but no bed yet (most severe first)
void hospital::allocateBed(){
    bool any=false;
    // go from most severe to least severe
    vector<int> order;
    for(size_t a=0;a<assignments.size();a++) order.push_back(a);
    for(size_t x=0;x<order.size();x++){
        for(size_t y=x+1;y<order.size();y++){
            int sx=-1,sy=-1;
            for(size_t p=0;p<patients.size();p++){
                if(patients[p].GetPatientID()==assignments[order[x]].first) sx=patients[p].GetPatientSeverity();
                if(patients[p].GetPatientID()==assignments[order[y]].first) sy=patients[p].GetPatientSeverity();
            }
            if(sy>sx){ int t=order[x]; order[x]=order[y]; order[y]=t; }
        }
    }
    for(size_t k=0;k<order.size();k++){
        int pid=assignments[order[k]].first;
        bool has_bed=false;
        for(size_t b=0;b<beds.size();b++){
            if(!beds[b].GetBedAvailability() && beds[b].GetPatientBedID()==pid) has_bed=true;
        }
        if(has_bed) continue;
        int sev=-1;
        string name="";
        for(size_t p=0;p<patients.size();p++){
            if(patients[p].GetPatientID()==pid){ sev=patients[p].GetPatientSeverity(); name=patients[p].GetPatientName(); }
        }
        int chosen=-1;
        for(size_t b=0;b<beds.size();b++){
            if(!beds[b].GetBedAvailability()) continue;
            if(sev>=8 && beds[b].GetWardType()=="ICU"){ chosen=b; break; }   // severe patient: ICU first
            if(chosen==-1) chosen=b;                                          // otherwise any free bed
            if(sev<8) break;
        }
        if(chosen==-1){
            cout<<"No free bed for "<<name<<"."<<endl;
            continue;
        }
        beds[chosen].AssignPatient(pid);
        cout<<"Bed "<<beds[chosen].GetBedID()<<" ("<<beds[chosen].GetWardType()<<") given to "<<name<<endl;
        any=true;
    }
    if(!any) cout<<"No new bed was allocated."<<endl;
}

// frees the doctor and the bed of a patient
void hospital::dischargePatient(){
    int pid;
    cout<<"Enter Patient ID to discharge : ";
    cin>>pid;
    int found=-1;
    for(size_t a=0;a<assignments.size();a++){
        if(assignments[a].first==pid) found=a;
    }
    if(found==-1){
        cout<<"This patient is not currently admitted."<<endl;
        return;
    }
    int doc_id=assignments[found].second;
    for(size_t d=0;d<doctors.size();d++){
        if(doctors[d].GetDocID()==doc_id) doctors[d].SetDocAvailability(true);
    }
    for(size_t b=0;b<beds.size();b++){
        if(!beds[b].GetBedAvailability() && beds[b].GetPatientBedID()==pid) beds[b].Release();
    }
    assignments.erase(assignments.begin()+found);
    cout<<"Patient "<<pid<<" discharged. Doctor and bed are free again."<<endl;
}


void hospital::showAdmitted(){
    if(assignments.empty()){
        cout<<"No patients are admitted right now."<<endl;
        return;
    }
    for(size_t a=0;a<assignments.size();a++){
        int pid=assignments[a].first;
        int did=assignments[a].second;
        string pname="?",dname="?";
        int sev=-1;
        for(size_t p=0;p<patients.size();p++){
            if(patients[p].GetPatientID()==pid){ pname=patients[p].GetPatientName(); sev=patients[p].GetPatientSeverity(); }
        }
        for(size_t d=0;d<doctors.size();d++){
            if(doctors[d].GetDocID()==did) dname=doctors[d].GetDocName();
        }
        string bedtext="no bed yet";
        for(size_t b=0;b<beds.size();b++){
            if(!beds[b].GetBedAvailability() && beds[b].GetPatientBedID()==pid){
                bedtext="Bed "+to_string(beds[b].GetBedID())+" ("+beds[b].GetWardType()+")";
            }
        }
        cout<<pname<<" (ID "<<pid<<", severity "<<sev<<") | Dr. "<<dname<<" | "<<bedtext<<endl;
    }
}
