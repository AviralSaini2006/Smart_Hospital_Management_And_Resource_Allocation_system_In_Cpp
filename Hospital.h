#ifndef HOSPITAL_H
#define HOSPITAL_H

#include"Patient.h"
#include"Doctor.h"
#include"Bed.h"
#include"Appointment.h"
#include<vector>
#include <map>
#include <string>
#include <ctime>
#include <queue>
#include <utility>
using namespace std;
struct PartnerHospital {
    int hospital_id;
    string name;
    string location;
    string specialization;
    int totalICUBeds;
    int availableICUBeds;
};

struct TransferRecord {
    int patient_id;
    string hospital_name;
    string location;
    string reason;
    time_t transfer_time;
    int distanceKm;
};
class hospital{
    private:
       string HospitalName;
       vector<patient> patients;
       vector<doctor> doctors;
       vector<bed> beds;
       // (severity, -index in patients vector): highest severity first, earlier patient first on a tie
       priority_queue<pair<int,int> > waiting_queue;
       priority_queue<pair<int, int>> doctorWaitingQueue;
       appointmentbook book;   // all appointments (conflict checking lives inside it)
       vector<pair<int,int> > assignments;   // (patient_id, doctor_id) for patients currently under a doctor
       vector<PartnerHospital> partnerHospitals;
vector<TransferRecord> transferRecords;

map<int, vector<pair<int, int>>> hospitalGraph;
    public:
       hospital(string h) : HospitalName(h) {
    hospitalGraph[0];
}
       bool bedExists(int id);       // true if a bed with this ID is already registered
       void showFreeBeds();          // lists only the free beds and a count per ward
       bool patientExists(int id);   // true if a patient with this ID is already registered
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
       void showAdmitted();

       bool doctorExists(int id);
       void bookAppointment(int patient_id,int doctor_id,string date_time_text);
       void cancelAppointment(int appointment_id);
       void completeAppointment(int appointment_id);
       void showAppointments();
       void saveAppointments();
       void loadAppointments();

       void saveData();   // saves patients, doctors, beds and admissions to hospital_data.txt
       void loadData();   // loads them back and rebuilds the priority queue

       void transferBed(int patient_id,int new_bed_id);   // moves an admitted patient to another free bed
       void showWardSummary();        
       // Centralized registration
bool registerPatient(patient p);
bool registerDoctor(doctor d);
bool registerBed(bed b);

// Automated workflows
bool processNextPatient();
bool admitPatient(int patient_id, int severity,
                  string diagnosis);
bool dischargePatientById(int patient_id);     
void processWaitingQueue();     
bool assessPatient(int patient_id, int severity, string diagnosis);     
bool initialTriage(int patient_id, int severity);
bool completeAssessment(int patient_id,string diagnosis,int systolicBP,int diastolicBP,int heartRate,int spo2,int respiratoryRate,double temperature);
void processDoctorWaitingQueue();   // total / free / occupied beds for each ward   // patient - doctor - bed for everyone currently admitted
bool registerPartnerHospital(int id, string name, string location,
                             string specialization, int icuBeds);

bool addHospitalRoute(int fromID, int toID, int distanceKm);

bool updatePartnerICUAvailability(int id, int availableBeds);

void showPartnerHospitals();
void showHospitalRoutes();
void showTransferRecords();

bool transferCriticalPatient(int patientIndex);
void showDoctorWaitingQueue();
}; 

#endif


