#include"Hospital.h"
#include<fstream>
using namespace std;

       bool hospital::bedExists(int id){
          for(size_t i=0;i<beds.size();i++){
              if(beds[i].GetBedID()==id) return true;
          }
          return false;
       }
       void hospital::showFreeBeds(){
          int free_count=0;
          for(size_t i=0;i<beds.size();i++){
              if(beds[i].GetBedAvailability()){
                  cout<<"Bed "<<beds[i].GetBedID()<<" ("<<beds[i].GetWardType()<<") is free"<<endl;
                  free_count++;
              }
          }
          cout<<"Free beds : "<<free_count<<" out of "<<beds.size()<<endl;
       }
       bool hospital::patientExists(int id){
          for(size_t i=0;i<patients.size();i++){
              if(patients[i].GetPatientID()==id) return true;
          }
          return false;
       }
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


// ---------------- Appointment scheduling ----------------
const string APPOINTMENT_FILE="appointments.txt";

bool hospital::doctorExists(int id){
    for(size_t i=0;i<doctors.size();i++){
        if(doctors[i].GetDocID()==id) return true;
    }
    return false;
}

void hospital::bookAppointment(int patient_id,int doctor_id,string date_time_text){
    if(!patientExists(patient_id)){
        cout<<"Patient ID not found."<<endl;
        return;
    }
    if(!doctorExists(doctor_id)){
        cout<<"Doctor ID not found."<<endl;
        return;
    }
    int result=book.BookAppointment(patient_id,doctor_id,date_time_text);
    if(result>0)       cout<<"Appointment booked. Appointment ID : "<<result<<endl;
    else if(result==-1) cout<<"Invalid date/time. Use DD-MM-YYYY HH:MM (example 25-12-2026 10:30)."<<endl;
    else if(result==-2) cout<<"This time is in the past. Choose a future time."<<endl;
    else if(result==-3) cout<<"The doctor is already booked at this time."<<endl;
    else if(result==-4) cout<<"The patient already has an appointment at this time."<<endl;
}

void hospital::cancelAppointment(int appointment_id){
    if(book.CancelAppointment(appointment_id)) cout<<"Appointment cancelled."<<endl;
    else cout<<"Appointment not found or it is not active."<<endl;
}

void hospital::completeAppointment(int appointment_id){
    if(book.CompleteAppointment(appointment_id)) cout<<"Appointment marked as completed."<<endl;
    else cout<<"Appointment not found or it is not active."<<endl;
}

void hospital::showAppointments(){
    book.DisplayAll();
}

void hospital::saveAppointments(){
    if(book.SaveAppointments(APPOINTMENT_FILE)) cout<<"Appointments saved to "<<APPOINTMENT_FILE<<endl;
    else cout<<"Could not save appointments."<<endl;
}

void hospital::loadAppointments(){
    if(book.LoadAppointments(APPOINTMENT_FILE)) cout<<"Appointments loaded from "<<APPOINTMENT_FILE<<endl;
    else cout<<"No saved appointment file found."<<endl;
}


// ---------------- File handling (save / load) ----------------
// One line per record:
//   P id name age gender diagnosis severity emergency
//   D id name age gender availability specialisation
//   B id ward availability patient_id
//   A patient_id doctor_id
const string DATA_FILE="hospital_data.txt";

void hospital::saveData(){
    ofstream out(DATA_FILE.c_str());
    if(!out){
        cout<<"Could not save data."<<endl;
        return;
    }
    for(size_t i=0;i<patients.size();i++){
        out<<"P "<<patients[i].GetPatientID()<<" "<<patients[i].GetPatientName()<<" "
           <<patients[i].GetPatientAge()<<" "<<patients[i].GetPatientGender()<<" "
           <<patients[i].GetPatientDiagnosis()<<" "<<patients[i].GetPatientSeverity()<<" "
           <<patients[i].GetPatientEmergencyStatus()<<"\n";
    }
    for(size_t i=0;i<doctors.size();i++){
        out<<"D "<<doctors[i].GetDocID()<<" "<<doctors[i].GetDocName()<<" "
           <<doctors[i].GetDocAge()<<" "<<doctors[i].GetDocGender()<<" "
           <<doctors[i].GetDocAvailability()<<" "<<doctors[i].GetDocSpecialisation()<<"\n";
    }
    for(size_t i=0;i<beds.size();i++){
        out<<"B "<<beds[i].GetBedID()<<" "<<beds[i].GetWardType()<<" "
           <<beds[i].GetBedAvailability()<<" "<<beds[i].GetPatientBedID()<<"\n";
    }
    for(size_t i=0;i<assignments.size();i++){
        out<<"A "<<assignments[i].first<<" "<<assignments[i].second<<"\n";
    }
    cout<<"Data saved to "<<DATA_FILE<<endl;
}

void hospital::loadData(){
    ifstream in(DATA_FILE.c_str());
    if(!in){
        cout<<"No saved data file found."<<endl;
        return;
    }
    patients.clear();
    doctors.clear();
    beds.clear();
    assignments.clear();
    waiting_queue=priority_queue<pair<int,int> >();

    char type;
    while(in>>type){
        if(type=='P'){
            int id,age,sev;
            bool em;
            string name,diag;
            char g;
            in>>id>>name>>age>>g>>diag>>sev>>em;
            patients.push_back(patient(id,name,age,g,diag,sev,em));
        }
        else if(type=='D'){
            int id,age;
            bool av;
            string name,spec;
            char g;
            in>>id>>name>>age>>g>>av>>spec;
            doctors.push_back(doctor(id,name,age,g,av,spec));
        }
        else if(type=='B'){
            int id,pid;
            bool av;
            string ward;
            in>>id>>ward>>av>>pid;
            beds.push_back(bed(id,ward,av,pid));
        }
        else if(type=='A'){
            int pid,did;
            in>>pid>>did;
            assignments.push_back(make_pair(pid,did));
        }
    }

    // rebuild the priority queue: patients who have a severity but no doctor yet
    for(size_t i=0;i<patients.size();i++){
        if(patients[i].GetPatientSeverity()==-1) continue;
        bool has_doctor=false;
        for(size_t a=0;a<assignments.size();a++){
            if(assignments[a].first==patients[i].GetPatientID()) has_doctor=true;
        }
        if(!has_doctor) waiting_queue.push(make_pair(patients[i].GetPatientSeverity(),-(int)i));
    }
    cout<<"Data loaded: "<<patients.size()<<" patients, "<<doctors.size()<<" doctors, "
        <<beds.size()<<" beds."<<endl;
}
