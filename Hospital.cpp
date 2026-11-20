#include"Hospital.h"
#include<fstream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <utility>
#include <sstream>
#include <map>
#include <limits>
#include <functional>
#include <ctime>
#include<cctype>

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
       void hospital::AddPatient(patient p) {
    patients.push_back(p);
}

void hospital::AddDoctor(doctor d) {
    doctors.push_back(d);
}

void hospital::AddBed(bed b) {
    beds.push_back(b);
}
      bool hospital::registerPatient(patient p)
{
    if (patientExists(p.GetPatientID())) {
        cout << "Error: Patient ID already exists.\n";
        return false;
    }

    AddPatient(p);

    int patientIndex = static_cast<int>(patients.size()) - 1;

    doctorWaitingQueue.push(make_pair(0, -patientIndex));

    cout << "\nPatient registered successfully.\n";
    cout << "Patient added to doctor waiting queue.\n";

    processDoctorWaitingQueue();

    return true;
}

bool hospital::registerDoctor(doctor d) {
    if (doctorExists(d.GetDocID())) {
        cout << "Error: Doctor ID already exists.\n";
        return false;
    }

    AddDoctor(d);
    processDoctorWaitingQueue();
    return true;
}

bool hospital::registerBed(bed b) {
    if (bedExists(b.GetBedID())) {
        cout << "Error: Bed ID already exists.\n";
        return false;
    }

    AddBed(b);
    processWaitingQueue();
    return true;
}

       void hospital::showPatients(){
        if(patients.empty())
        cout<<"No Patient exists right now!"<<endl;
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

void hospital::allocateBed()
{
    bool any = false;

    vector<int> order;

    for (size_t i = 0; i < assignments.size(); i++)
        order.push_back(i);

    // Sort patients by severity, highest first.
    for (size_t i = 0; i < order.size(); i++)
    {
        for (size_t j = i + 1; j < order.size(); j++)
        {
            int s1 = -1, s2 = -1;

            for (size_t p = 0; p < patients.size(); p++)
            {
                if (patients[p].GetPatientID() == assignments[order[i]].first)
                    s1 = patients[p].GetPatientSeverity();

                if (patients[p].GetPatientID() == assignments[order[j]].first)
                    s2 = patients[p].GetPatientSeverity();
            }

            if (s2 > s1)
                swap(order[i], order[j]);
        }
    }

    for (size_t k = 0; k < order.size(); k++)
    {
        int pid = assignments[order[k]].first;
        int patientIndex = -1;

        for (size_t p = 0; p < patients.size(); p++)
        {
            if (patients[p].GetPatientID() == pid)
            {
                patientIndex = p;
                break;
            }
        }

        if (patientIndex == -1)
            continue;

        patient &p = patients[patientIndex];
        int sev = p.GetPatientSeverity();

        if (sev < 1)
            continue;

        bool hasBed = false;

        for (size_t b = 0; b < beds.size(); b++)
        {
            if (!beds[b].GetBedAvailability() &&
                beds[b].GetPatientBedID() == pid)
            {
                hasBed = true;
                break;
            }
        }

        if (hasBed)
            continue;

        int chosen = -1;

        for (size_t b = 0; b < beds.size(); b++)
        {
            if (!beds[b].GetBedAvailability())
                continue;

            if (sev >= 8)
            {
                if (beds[b].GetWardType() == "ICU")
                {
                    chosen = b;
                    break;
                }
            }
            else
            {
                chosen = b;
                break;
            }
        }

        if (chosen == -1)
        {
            if (sev >= 8)
                cout << "No ICU bed available for " << p.GetPatientName() << ".\n";
            else
                cout << "No free bed available for " << p.GetPatientName() << ".\n";

            continue;
        }

        beds[chosen].AssignPatient(pid);

        cout << "Bed " << beds[chosen].GetBedID()
             << " (" << beds[chosen].GetWardType()
             << ") allocated to " << p.GetPatientName() << ".\n";

        any = true;
    }

    if (!any)
        cout << "No new bed was allocated.\n";
}


// frees the doctor and the bed of a patient
void hospital::dischargePatient() {
    int pid;

    cout << "Enter Patient ID to discharge: ";
    cin >> pid;

    dischargePatientById(pid);
}



void hospital::showAdmitted()
{
    bool found = false;

    for (size_t p = 0; p < patients.size(); p++)
    {
        int pid = patients[p].GetPatientID();

        // Find an occupied bed belonging to this patient
        int bedIndex = -1;

        for (size_t b = 0; b < beds.size(); b++)
        {
            if (!beds[b].GetBedAvailability() &&
                beds[b].GetPatientBedID() == pid)
            {
                bedIndex = b;
                break;
            }
        }

        // Skip patients who don't have a bed
        if (bedIndex == -1)
            continue;

        found = true;

        string doctorName = "Not Assigned";

        // Find assigned doctor
        for (size_t a = 0; a < assignments.size(); a++)
        {
            if (assignments[a].first == pid)
            {
                int did = assignments[a].second;

                for (size_t d = 0; d < doctors.size(); d++)
                {
                    if (doctors[d].GetDocID() == did)
                    {
                        doctorName = doctors[d].GetDocName();
                        break;
                    }
                }

                break;
            }
        }

        cout << patients[p].GetPatientName()
             << " (ID: " << pid << ")"
             << " | Severity: " << patients[p].GetPatientSeverity()
             << " | Dr. " << doctorName
             << " | Bed: " << beds[bedIndex].GetBedID()
             << " (" << beds[bedIndex].GetWardType() << ")"
             << endl;
    }

    if (!found)
        cout << "No patients are currently admitted." << endl;
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
//   P id name age gender diagnosis severity emergency required_specialisation
//   D id name age gender availability specialisation
//   B id ward availability patient_id
//   A patient_id doctor_id
const string DATA_FILE="hospital_data.txt";





void hospital::saveData()
{
    ofstream out(DATA_FILE.c_str());

    if (!out) {
        cout << "Could not save data.\n";
        return;
    }

    // Save patients
    for (auto &p : patients) {
        out << "P "
            << p.GetPatientID() << " "
            << quoted(p.GetPatientName()) << " "
            << p.GetPatientAge() << " "
            << p.GetPatientGender() << " "
            << quoted(p.GetPatientDiagnosis()) << " "
            << p.GetPatientSeverity() << " "
            << p.GetPatientEmergencyStatus() << " "
            << quoted(p.GetRequiredSpecialisation()) << " "
            << p.getSystolicBP() << " "
            << p.getDiastolicBP() << " "
            << p.getHeartRate() << " "
            << p.getSpO2() << " "
            << p.getRespiratoryRate() << " "
            << p.getTemperature() << " "
            << p.GetDischarged() << " "
            << p.GetTransferred() << " "
            << quoted(p.GetTransferHospital()) << "\n";
    }

    // Save doctors
    for (auto &d : doctors) {
        out << "D "
            << d.GetDocID() << " "
            << quoted(d.GetDocName()) << " "
            << d.GetDocAge() << " "
            << d.GetDocGender() << " "
            << d.GetDocAvailability() << " "
            << quoted(d.GetDocSpecialisation()) << "\n";
    }

    // Save beds
    for (auto &b : beds) {
        out << "B "
            << b.GetBedID() << " "
            << quoted(b.GetWardType()) << " "
            << b.GetBedAvailability() << " "
            << b.GetPatientBedID() << "\n";
    }

    // Save doctor assignments
    for (auto &a : assignments) {
        out << "A " << a.first << " " << a.second << "\n";
    }

    // Save partner hospitals
    for (auto &h : partnerHospitals) {
        out << "H "
            << h.hospital_id << " "
            << quoted(h.name) << " "
            << quoted(h.location) << " "
            << quoted(h.specialization) << " "
            << h.totalICUBeds << " "
            << h.availableICUBeds << "\n";
    }

    // Save routes
    for (auto &node : hospitalGraph) {
        for (auto &edge : node.second) {
            if (node.first < edge.first) {
                out << "R "
                    << node.first << " "
                    << edge.first << " "
                    << edge.second << "\n";
            }
        }
    }

    // Save transfer records
    for (auto &r : transferRecords) {
        out << "T "
            << r.patient_id << " "
            << quoted(r.hospital_name) << " "
            << quoted(r.location) << " "
            << quoted(r.reason) << " "
            << static_cast<long long>(r.transfer_time) << " "
            << r.distanceKm << "\n";
    }

    out.close();

    cout << "All hospital data saved successfully.\n";
}

void hospital::loadData()
{
    ifstream in(DATA_FILE.c_str());

    if (!in) {
        cout << "No saved data file found.\n";
        return;
    }

    patients.clear();
    doctors.clear();
    beds.clear();
    assignments.clear();
    partnerHospitals.clear();
    transferRecords.clear();
    hospitalGraph.clear();

    waiting_queue = priority_queue<pair<int, int>>();
    doctorWaitingQueue = priority_queue<pair<int, int>>();

    hospitalGraph[0];

    string line;

    while (getline(in, line)) {
        if (line.empty())
            continue;

        istringstream record(line);
        char type;

        if (!(record >> type))
            continue;

        if (type == 'P') {
            int id, age, severity;
            bool emergency, discharged = false, transferred = false;
            char gender;

            string name, diagnosis, specialisation;
            string transferHospital;

            int systolic = 0, diastolic = 0;
            int heartRate = 0, oxygen = 0;
            int respiratoryRate = 0;
            double temperature = 0.0;

            if (!(record >> id
                         >> quoted(name)
                         >> age
                         >> gender
                         >> quoted(diagnosis)
                         >> severity
                         >> emergency
                         >> quoted(specialisation)))
                continue;

            record >> systolic
                   >> diastolic
                   >> heartRate
                   >> oxygen
                   >> respiratoryRate
                   >> temperature;

            if (!(record >> discharged)) {
                record.clear();
                discharged = false;
            }

            if (!(record >> transferred
                         >> quoted(transferHospital))) {
                record.clear();
                transferred = false;
                transferHospital = "";
            }

            patient p(
                id, name, age, gender,
                diagnosis, severity, emergency,
                specialisation, discharged,
                transferred, transferHospital
            );

            p.setVitals(
                systolic, diastolic, heartRate,
                oxygen, respiratoryRate, temperature
            );

            patients.push_back(p);
        }

        else if (type == 'D') {
            int id, age;
            bool availability;
            char gender;
            string name, specialisation;

            if (record >> id
                       >> quoted(name)
                       >> age
                       >> gender
                       >> availability
                       >> quoted(specialisation)) {
                doctors.push_back(
                    doctor(id, name, age, gender,
                           availability, specialisation)
                );
            }
        }

        else if (type == 'B') {
            int id, patientID;
            bool availability;
            string ward;

            if (record >> id
                       >> quoted(ward)
                       >> availability
                       >> patientID) {
                beds.push_back(
                    bed(id, ward, availability, patientID)
                );
            }
        }

        else if (type == 'A') {
            int patientID, doctorID;

            if (record >> patientID >> doctorID) {
                assignments.push_back(
                    make_pair(patientID, doctorID)
                );
            }
        }

        else if (type == 'H') {
            PartnerHospital h;

            if (record >> h.hospital_id
                       >> quoted(h.name)
                       >> quoted(h.location)
                       >> quoted(h.specialization)
                       >> h.totalICUBeds
                       >> h.availableICUBeds) {
                partnerHospitals.push_back(h);
                hospitalGraph[h.hospital_id];
            }
        }

        else if (type == 'R') {
            int fromID, toID, distance;

            if (record >> fromID >> toID >> distance) {
                if (distance > 0) {
                    hospitalGraph[fromID].push_back(
                        make_pair(toID, distance)
                    );

                    hospitalGraph[toID].push_back(
                        make_pair(fromID, distance)
                    );
                }
            }
        }

        else if (type == 'T') {
            TransferRecord r;
            long long savedTime;

            if (record >> r.patient_id
                       >> quoted(r.hospital_name)
                       >> quoted(r.location)
                       >> quoted(r.reason)
                       >> savedTime
                       >> r.distanceKm) {
                r.transfer_time = static_cast<time_t>(savedTime);
                transferRecords.push_back(r);
            }
        }
    }

    in.close();

    // Reconstruct waiting queues
    for (size_t i = 0; i < patients.size(); i++) {
        patient &p = patients[i];

        if (p.GetDischarged() || p.GetTransferred())
            continue;

        int patientID = p.GetPatientID();

        bool hasDoctor = false;
        bool hasBed = false;

        for (auto &a : assignments) {
            if (a.first == patientID) {
                hasDoctor = true;
                break;
            }
        }

        for (auto &b : beds) {
            if (!b.GetBedAvailability() &&
                b.GetPatientBedID() == patientID) {
                hasBed = true;
                break;
            }
        }

        if (!hasDoctor) {
            doctorWaitingQueue.push(
                make_pair(0, -(int)i)
            );
        }
        else if (!hasBed &&
                 p.GetPatientDiagnosis() != "NULL" &&
                 p.GetPatientSeverity() >= 1) {
            waiting_queue.push(
                make_pair(p.GetPatientSeverity(), -(int)i)
            );
        }
    }

    cout << "\nHospital data loaded successfully.\n";
    cout << "Patients: " << patients.size() << '\n';
    cout << "Doctors: " << doctors.size() << '\n';
    cout << "Beds: " << beds.size() << '\n';
    cout << "Partner hospitals: " << partnerHospitals.size() << '\n';
    cout << "Transfer records: " << transferRecords.size() << '\n';
}




// ---------------- Bed management ----------------
// moves an admitted patient from his/her current bed to another free bed
void hospital::transferBed(int patient_id,int new_bed_id){
    int old_index=-1,new_index=-1;
    for(size_t b=0;b<beds.size();b++){
        if(!beds[b].GetBedAvailability() && beds[b].GetPatientBedID()==patient_id) old_index=b;
        if(beds[b].GetBedID()==new_bed_id) new_index=b;
    }
    if(old_index==-1){
        cout<<"This patient does not have a bed right now."<<endl;
        return;
    }
    if(new_index==-1){
        cout<<"Bed ID not found."<<endl;
        return;
    }
    if(new_index==old_index){
        cout<<"The patient is already in this bed."<<endl;
        return;
    }
    if(!beds[new_index].GetBedAvailability()){
        cout<<"That bed is not free."<<endl;
        return;
    }
    int sev=-1;
    string name="";
    for(size_t p=0;p<patients.size();p++){
        if(patients[p].GetPatientID()==patient_id){
            sev=patients[p].GetPatientSeverity();
            name=patients[p].GetPatientName();
        }
    }
    if(sev>=8 && beds[new_index].GetWardType()!="ICU"){
        cout<<"Severe patients (severity 8 or more) must stay in an ICU bed."<<endl;
        return;
    }
    beds[old_index].Release();
    beds[new_index].AssignPatient(patient_id);
    cout<<name<<" moved to Bed "<<beds[new_index].GetBedID()<<" ("<<beds[new_index].GetWardType()<<"). "
        <<"Bed "<<beds[old_index].GetBedID()<<" is free again."<<endl;
}

// prints how many beds each ward has, and how many of them are free
void hospital::showWardSummary(){
    if(beds.empty()){
        cout<<"No beds are registered."<<endl;
        return;
    }
    vector<string> wards;
    vector<int> total,free_beds;
    for(size_t b=0;b<beds.size();b++){
        size_t w=0;
        while(w<wards.size() && wards[w]!=beds[b].GetWardType()) w++;
        if(w==wards.size()){
            wards.push_back(beds[b].GetWardType());
            total.push_back(0);
            free_beds.push_back(0);
        }
        total[w]++;
        if(beds[b].GetBedAvailability()) free_beds[w]++;
    }
    for(size_t w=0;w<wards.size();w++){
        cout<<wards[w]<<" : "<<total[w]<<" beds, "<<free_beds[w]<<" free, "
            <<(total[w]-free_beds[w])<<" occupied"<<endl;
    }
}

bool hospital::processNextPatient()
{
    if (waiting_queue.empty())
        return false;

    pair<int, int> top = waiting_queue.top();
    int patientIndex = -top.second;

    if (patientIndex < 0 ||
        patientIndex >= static_cast<int>(patients.size())) {
        waiting_queue.pop();
        return true;
    }

    patient &p = patients[patientIndex];

    if (p.GetDischarged() || p.GetTransferred()) {
        waiting_queue.pop();
        return true;
    }

    if (p.GetPatientDiagnosis() == "NULL") {
        cout << "Patient assessment is incomplete.\n";
        return false;
    }

    int doctorId = -1;

    for (const auto &a : assignments) {
        if (a.first == p.GetPatientID()) {
            doctorId = a.second;
            break;
        }
    }

    if (doctorId == -1) {
        cout << "No doctor assigned to this patient.\n";
        return false;
    }

    int bedIndex = -1;
    bool requiresICU = (p.GetPatientSeverity() >= 8);

    for (size_t i = 0; i < beds.size(); i++) {
        if (!beds[i].GetBedAvailability())
            continue;

        string ward = beds[i].GetWardType();

        for (char &c : ward)
            c = toupper(static_cast<unsigned char>(c));

        if (requiresICU && ward == "ICU") {
            bedIndex = static_cast<int>(i);
            break;
        }

        if (!requiresICU && ward != "ICU") {
            bedIndex = static_cast<int>(i);
            break;
        }
    }

    if (bedIndex == -1) {
        if (requiresICU) {
            cout << "\nNo local ICU bed available for patient "
                 << p.GetPatientID() << ".\n";

            if (transferCriticalPatient(patientIndex)) {
                waiting_queue.pop();
                return true;
            }
        }
        else {
            cout << "No suitable free non-ICU bed. Patient remains in queue.\n";
        }

        return false;
    }

    beds[bedIndex].AssignPatient(p.GetPatientID());

    waiting_queue.pop();

    cout << "\nPatient admitted successfully.\n";
    cout << "Patient ID: " << p.GetPatientID() << '\n';
    cout << "Bed ID: " << beds[bedIndex].GetBedID() << '\n';
    cout << "Ward: " << beds[bedIndex].GetWardType() << '\n';

    return true;
}

bool hospital::admitPatient(int patient_id, int severity, string diagnosis) {
    if (!queuePatient(patient_id, severity, diagnosis)) {
        return false;
    }

    processWaitingQueue();
    return true;
}

bool hospital::dischargePatientById(int patient_id) {
    int patientIndex = -1;

    for (size_t i = 0; i < patients.size(); i++) {
        if (patients[i].GetPatientID() == patient_id) {
            patientIndex = (int)i;
            break;
        }
    }

    if (patientIndex == -1) {
        cout << "Patient ID not found.\n";
        return false;
    }

    patient &p = patients[patientIndex];

    if (p.GetDischarged()) {
        cout << "This patient has already been discharged.\n";
        return false;
    }
    if (p.GetTransferred()) {
    cout << "Patient has already been transferred to "
         << p.GetTransferHospital() << ".\n";
    return false;
}

    int doctor_id = -1;

    for (const auto &a : assignments) {
        if (a.first == patient_id) {
            doctor_id = a.second;
            break;
        }
    }

    for (auto &b : beds) {
        if (!b.GetBedAvailability() &&
            b.GetPatientBedID() == patient_id) {
            b.Release();
            break;
        }
    }

    if (doctor_id != -1) {
        for (auto &d : doctors) {
            if (d.GetDocID() == doctor_id) {
                d.SetDocAvailability(true);
                break;
            }
        }
    }

    assignments.erase(
        remove_if(assignments.begin(), assignments.end(),
            [patient_id](const pair<int, int> &a) {
                return a.first == patient_id;
            }),
        assignments.end()
    );

    p.setDischarged(true);

    cout << "Patient discharged successfully.\n";

    processDoctorWaitingQueue();
    processWaitingQueue();

    return true;
}


void hospital::processWaitingQueue() {
    while (!waiting_queue.empty()) {
        int patientIndex = -waiting_queue.top().second;

        if (patientIndex < 0 ||
            patientIndex >= (int)patients.size() ||
            patients[patientIndex].GetDischarged()) {
            waiting_queue.pop();
            continue;
        }

        if (!processNextPatient()) {
            break;
        }
    }
}

bool hospital::completeAssessment(
    int patient_id,
    string diagnosis,
    int systolicBP,
    int diastolicBP,
    int heartRate,
    int spo2,
    int respiratoryRate,
    double temperature)
{
    int patientIndex = -1;

    // Find patient
    for (size_t i = 0; i < patients.size(); i++) {
        if (patients[i].GetPatientID() == patient_id) {
            patientIndex = i;
            break;
        }
    }

    if (patientIndex == -1) {
        cout << "Patient ID not found.\n";
        return false;
    }

    patient &p = patients[patientIndex];

    // Check whether doctor is assigned
    bool doctorAssigned = false;

    for (const auto &a : assignments) {
        if (a.first == patient_id) {
            doctorAssigned = true;
            break;
        }
    }

    if (!doctorAssigned) {
        cout << "No doctor assigned to this patient.\n";
        return false;
    }

    if (p.GetPatientDiagnosis() != "NULL") {
        cout << "Patient has already been assessed.\n";
        return false;
    }

    if (diagnosis.empty() || diagnosis == "NULL") {
        cout << "Enter a valid diagnosis.\n";
        return false;
    }

    // Basic input validation
    if (systolicBP <= 0 || diastolicBP <= 0 ||
        heartRate <= 0 || spo2 < 0 || spo2 > 100 ||
        respiratoryRate <= 0 || temperature <= 0) {

        cout << "Invalid vital parameters.\n";
        return false;
    }

    // Store vital parameters
    p.setVitals(
        systolicBP,
        diastolicBP,
        heartRate,
        spo2,
        respiratoryRate,
        temperature
    );

    // Demonstration severity scoring
    int score = 0;

    // Oxygen saturation
    if (spo2 <= 90)
        score += 3;
    else if (spo2 <= 94)
        score += 2;

    // Heart rate
    if (heartRate < 40 || heartRate > 130)
        score += 3;
    else if (heartRate < 50 || heartRate > 110)
        score += 2;

    // Respiratory rate
    if (respiratoryRate < 8 || respiratoryRate > 30)
        score += 3;
    else if (respiratoryRate < 10 || respiratoryRate > 24)
        score += 2;

    // Blood pressure
    if (systolicBP < 90 || systolicBP > 180)
        score += 3;
    else if (systolicBP < 100 || systolicBP > 160)
        score += 2;

    // Temperature
    if (temperature < 35 || temperature >= 40)
        score += 2;
    else if (temperature >= 38.5)
        score += 1;

    // Convert score into severity (1-10)
    int severity = min(10, score + 1);

    p.setSeverity(severity);
    p.setEmergencyStatus(severity >= 8);
    p.setDiagnosis(diagnosis);

    cout << "\n===== ASSESSMENT RESULTS =====\n";

    cout << "Patient: " << p.GetPatientName() << '\n';
    cout << "Diagnosis: " << diagnosis << '\n';

    cout << "Calculated severity: "
         << severity << "/10\n";

    // Add patient to bed allocation queue
    waiting_queue.push(
        make_pair(severity, -patientIndex)
    );

    cout << "Patient added to bed allocation queue.\n";

    processWaitingQueue();

    return true;
}


void hospital::processDoctorWaitingQueue()
{
    vector<pair<int, int>> skipped;

    auto normalize = [](string s) {
        while (!s.empty() && isspace((unsigned char)s.front()))
            s.erase(s.begin());

        while (!s.empty() && isspace((unsigned char)s.back()))
            s.pop_back();

        for (char &c : s)
            c = tolower((unsigned char)c);

        return s;
    };

    while (!doctorWaitingQueue.empty())
    {
        auto top = doctorWaitingQueue.top();
        doctorWaitingQueue.pop();

        int patientIndex = -top.second;

        if (patientIndex < 0 ||
            patientIndex >= static_cast<int>(patients.size()))
            continue;

        patient &p = patients[patientIndex];

        if (p.GetDischarged() || p.GetTransferred())
            continue;

        bool alreadyAssigned = false;

        for (const auto &a : assignments)
        {
            if (a.first == p.GetPatientID())
            {
                alreadyAssigned = true;
                break;
            }
        }

        if (alreadyAssigned)
            continue;

        string required = normalize(p.GetRequiredSpecialisation());

        int doctorIndex = -1;

        for (size_t i = 0; i < doctors.size(); i++)
        {
            if (!doctors[i].GetDocAvailability())
                continue;

            string availableSpecialisation =
                normalize(doctors[i].GetDocSpecialisation());

            bool match =
                required.empty() ||
                required == "null" ||
                required == availableSpecialisation;

            if (match)
            {
                doctorIndex = static_cast<int>(i);
                break;
            }
        }

        if (doctorIndex == -1)
        {
            cout << "\nNo matching available doctor found for "
                 << p.GetPatientName() << ".\n";

            cout << "Required specialisation: "
                 << p.GetRequiredSpecialisation() << '\n';

            skipped.push_back(top);
            continue;
        }

        doctor &d = doctors[doctorIndex];

        d.SetDocAvailability(false);

        assignments.push_back(
            make_pair(p.GetPatientID(), d.GetDocID())
        );

        cout << "\nDoctor assigned successfully!\n";
        cout << "Patient: " << p.GetPatientName() << '\n';
        cout << "Doctor: Dr. " << d.GetDocName() << '\n';
        cout << "Specialisation: "
             << d.GetDocSpecialisation() << '\n';
    }

    for (const auto &entry : skipped)
        doctorWaitingQueue.push(entry);
}

bool hospital::registerPartnerHospital(int id, string name, string location,
                                        string specialization, int icuBeds)
{
    if (id <= 0 || icuBeds < 0) {
        cout << "Invalid hospital ID or ICU bed count.\n";
        return false;
    }

    for (const auto &h : partnerHospitals) {
        if (h.hospital_id == id) {
            cout << "Hospital ID already exists.\n";
            return false;
        }
    }

    PartnerHospital h;

    h.hospital_id = id;
    h.name = name;
    h.location = location;
    h.specialization = specialization;
    h.totalICUBeds = icuBeds;
    h.availableICUBeds = icuBeds;

    partnerHospitals.push_back(h);
    hospitalGraph[id];

    cout << "Partner hospital registered successfully.\n";
    return true;
}
bool hospital::addHospitalRoute(int fromID, int toID, int distanceKm)
{
    if (fromID == toID || distanceKm <= 0) {
        cout << "Invalid route or distance.\n";
        return false;
    }

    auto hospitalExists = [&](int id) {
        if (id == 0)
            return true;

        for (const auto &h : partnerHospitals) {
            if (h.hospital_id == id)
                return true;
        }

        return false;
    };

    if (!hospitalExists(fromID) || !hospitalExists(toID)) {
        cout << "One or both hospital IDs do not exist.\n";
        return false;
    }

    auto updateEdge = [&](int from, int to) {
        for (auto &edge : hospitalGraph[from]) {
            if (edge.first == to) {
                edge.second = distanceKm;
                return;
            }
        }

        hospitalGraph[from].push_back({to, distanceKm});
    };

    updateEdge(fromID, toID);
    updateEdge(toID, fromID);

    cout << "Route added successfully.\n";
    return true;
}
bool hospital::updatePartnerICUAvailability(int id, int availableBeds)
{
    for (auto &h : partnerHospitals) {
        if (h.hospital_id == id) {
            if (availableBeds < 0 ||
                availableBeds > h.totalICUBeds) {
                cout << "Invalid ICU bed count.\n";
                return false;
            }

            h.availableICUBeds = availableBeds;

            cout << "ICU availability updated.\n";

            processWaitingQueue();
            return true;
        }
    }

    cout << "Partner hospital not found.\n";
    return false;
}
void hospital::showPartnerHospitals()
{
    if (partnerHospitals.empty()) {
        cout << "No partner hospitals registered.\n";
        return;
    }

    cout << "\n========== PARTNER HOSPITALS ==========\n";

    for (const auto &h : partnerHospitals) {
        cout << "\nHospital ID: " << h.hospital_id;
        cout << "\nName: " << h.name;
        cout << "\nLocation: " << h.location;
        cout << "\nSpecialization: " << h.specialization;
        cout << "\nTotal ICU Beds: " << h.totalICUBeds;
        cout << "\nAvailable ICU Beds: " << h.availableICUBeds;
        cout << "\n---------------------------------------\n";
    }
}
void hospital::showHospitalRoutes()
{
    cout << "\n========== HOSPITAL NETWORK ==========\n";

    bool found = false;

    for (const auto &node : hospitalGraph) {
        int fromID = node.first;

        for (const auto &edge : node.second) {
            int toID = edge.first;

            if (fromID >= toID)
                continue;

            string fromName = HospitalName;
            string toName;

            if (fromID != 0) {
                for (const auto &h : partnerHospitals) {
                    if (h.hospital_id == fromID) {
                        fromName = h.name;
                        break;
                    }
                }
            }

            if (toID == 0) {
                toName = HospitalName;
            }
            else {
                for (const auto &h : partnerHospitals) {
                    if (h.hospital_id == toID) {
                        toName = h.name;
                        break;
                    }
                }
            }

            cout << fromName << " <---- "
                 << edge.second << " km ----> "
                 << toName << '\n';

            found = true;
        }
    }

    if (!found)
        cout << "No routes registered.\n";
}
void hospital::showTransferRecords()
{
    if (transferRecords.empty()) {
        cout << "No patients have been transferred.\n";
        return;
    }

    cout << "\n========== TRANSFER RECORDS ==========\n";

    for (const auto &r : transferRecords) {
        cout << "\nPatient ID: " << r.patient_id;
        cout << "\nDestination: " << r.hospital_name;
        cout << "\nLocation: " << r.location;
        cout << "\nReason: " << r.reason;
        cout << "\nDistance: " << r.distanceKm << " km";

        tm *timeInfo = localtime(&r.transfer_time);

        if (timeInfo != nullptr) {
            cout << "\nTransfer Time: "
                 << put_time(timeInfo, "%d-%m-%Y %H:%M:%S");
        }

        cout << "\n--------------------------------------\n";
    }
}
bool hospital::transferCriticalPatient(int patientIndex)
{
    if (patientIndex < 0 ||
        patientIndex >= static_cast<int>(patients.size())) {
        return false;
    }

    patient &p = patients[patientIndex];

    if (p.GetDischarged() || p.GetTransferred())
        return false;

    if (p.GetPatientSeverity() < 8)
        return false;

    const int INF = numeric_limits<int>::max();

    map<int, int> dist;
    map<int, int> previous;

    for (const auto &node : hospitalGraph) {
        dist[node.first] = INF;
        previous[node.first] = -1;
    }

    if (hospitalGraph.find(0) == hospitalGraph.end()) {
        cout << "Hospital network is not configured.\n";
        return false;
    }

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[0] = 0;
    pq.push({0, 0});

    // Dijkstra's shortest path algorithm
    while (!pq.empty()) {
        int currentDistance = pq.top().first;
        int currentHospital = pq.top().second;
        pq.pop();

        if (currentDistance > dist[currentHospital])
            continue;

        for (const auto &edge : hospitalGraph[currentHospital]) {
            int nextHospital = edge.first;
            int weight = edge.second;

            if (currentDistance + weight < dist[nextHospital]) {
                dist[nextHospital] = currentDistance + weight;
                previous[nextHospital] = currentHospital;

                pq.push({dist[nextHospital], nextHospital});
            }
        }
    }

    int destinationID = -1;
    int minimumDistance = INF;

    string required = p.GetRequiredSpecialisation();

    // Select the nearest suitable hospital
    for (const auto &h : partnerHospitals) {
        if (h.availableICUBeds <= 0)
            continue;

        bool specializationMatch =
            required.empty() ||
            required == "NULL" ||
            h.specialization == required ||
            h.specialization == "Multi-speciality" ||
            h.specialization == "Multispeciality";

        if (!specializationMatch)
            continue;

        if (dist.find(h.hospital_id) == dist.end())
            continue;

        if (dist[h.hospital_id] == INF)
            continue;

        if (dist[h.hospital_id] < minimumDistance) {
            minimumDistance = dist[h.hospital_id];
            destinationID = h.hospital_id;
        }
    }

    if (destinationID == -1) {
        cout << "No reachable partner hospital has a suitable available ICU bed.\n";
        return false;
    }

    PartnerHospital *destination = nullptr;

    for (auto &h : partnerHospitals) {
        if (h.hospital_id == destinationID) {
            destination = &h;
            break;
        }
    }

    if (destination == nullptr)
        return false;

    // Reconstruct shortest path
    vector<int> path;
    int current = destinationID;

    while (current != -1) {
        path.push_back(current);
        current = previous[current];
    }

    reverse(path.begin(), path.end());

    // Release the assigned doctor
    for (auto it = assignments.begin(); it != assignments.end();) {
        if (it->first == p.GetPatientID()) {
            for (auto &d : doctors) {
                if (d.GetDocID() == it->second) {
                    d.SetDocAvailability(true);
                    break;
                }
            }

            it = assignments.erase(it);
        }
        else {
            ++it;
        }
    }

    // Reserve one ICU bed at the destination
    destination->availableICUBeds--;

    p.setTransferred(true);
    p.setTransferHospital(destination->name);

    TransferRecord record;

    record.patient_id = p.GetPatientID();
    record.hospital_name = destination->name;
    record.location = destination->location;
    record.reason = "No suitable ICU bed available locally";
    record.transfer_time = time(nullptr);
    record.distanceKm = minimumDistance;

    transferRecords.push_back(record);

    cout << "\n========== PATIENT TRANSFER ==========\n";

    cout << "Patient ID: " << p.GetPatientID();
    cout << "\nPatient Name: " << p.GetPatientName();
    cout << "\nDestination: " << destination->name;
    cout << "\nLocation: " << destination->location;
    cout << "\nDistance: " << minimumDistance << " km";

    cout << "\n\nShortest Route:\n";

    for (size_t i = 0; i < path.size(); i++) {
        int id = path[i];

        if (id == 0) {
            cout << HospitalName;
        }
        else {
            for (const auto &h : partnerHospitals) {
                if (h.hospital_id == id) {
                    cout << h.name;
                    break;
                }
            }
        }

        if (i != path.size() - 1)
            cout << " -> ";
    }

    cout << "\nRemaining ICU Beds: "
         << destination->availableICUBeds;

    cout << "\nTransfer Status: Successful\n";
    cout << "======================================\n";

    processDoctorWaitingQueue();

    return true;
}
void hospital::showDoctorWaitingQueue()
{
    if (doctorWaitingQueue.empty()) {
        cout << "\nDoctor waiting queue is empty.\n";
        return;
    }

    priority_queue<pair<int, int>> temp = doctorWaitingQueue;

    cout << "\n========== DOCTOR WAITING QUEUE ==========\n";

    while (!temp.empty()) {
        int patientIndex = -temp.top().second;
        temp.pop();

        if (patientIndex < 0 ||
            patientIndex >= static_cast<int>(patients.size()))
            continue;

        patient &p = patients[patientIndex];

        if (p.GetDischarged() || p.GetTransferred())
            continue;

        cout << "Patient ID: " << p.GetPatientID() << '\n';
        cout << "Name: " << p.GetPatientName() << '\n';
        cout << "Required Specialisation: "
             << p.GetRequiredSpecialisation() << '\n';
        cout << "-----------------------------\n";
    }
}