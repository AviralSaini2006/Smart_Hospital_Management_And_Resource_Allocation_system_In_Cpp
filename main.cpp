#include"Hospital.h"
#include<iostream>
using namespace std;
int main(){
    hospital h("MediAlloc");
    int ch;
    cout<<"Smart Hospital Management and Resource Allocation System"<<endl;
    while(true){
        cout<<"1) Register a Patient"<<endl;
        cout<<"2) Register a Doctor"<<endl;
        cout<<"3) Register a Bed"<<endl;
        cout<<"4) Display all patients"<<endl;
        cout<<"5) Display all doctors"<<endl;
        cout<<"6) Display all beds"<<endl;
        cout<<"7) Set severity and add patient to priority queue"<<endl;
        cout<<"8) Show priority queue"<<endl;
        cout<<"9) Assign doctor to most severe patient"<<endl;
        cout<<"10) Allocate beds to admitted patients"<<endl;
        cout<<"11) Discharge a patient"<<endl;
        cout<<"12) Show admitted patients (doctor and bed)"<<endl;
        cout<<"13) Book an appointment"<<endl;
        cout<<"14) Cancel an appointment"<<endl;
        cout<<"15) Complete an appointment"<<endl;
        cout<<"16) Show all appointments"<<endl;
        cout<<"17) Save appointments to file"<<endl;
        cout<<"18) Load appointments from file"<<endl;
        cout<<"19) Show free beds"<<endl;
        cout<<"20) Exit Program"<<endl;
        cout<<"Answer : ";
        if(!(cin>>ch)){
            cin.clear();
            cin.ignore(10000,'\n');
            cout<<"Please enter a number from the menu."<<endl;
            continue;
        }
        switch(ch){
            case 1:{
               int i,a;
               string n;
               char g;
               cout<<"Enter Patient ID : ";cin>>i;
               if(h.patientExists(i)){
                   cout<<"A patient with this ID already exists."<<endl;
                   break;
               }
               cout<<"Enter Patient Name : ";cin>>n;
               cout<<"Enter Patient Age : ";cin>>a;
               cout<<"Enter Patient Gender (M/F): ";cin>>g;
               patient p(i,n,a,g,"NULL",-1,false);
               h.AddPatient(p);
               break;}
            case 2:{
               int i,a;
               string n,s;
               char g;
               bool av;
               cout<<"Enter Doctor ID : ";cin>>i;
               if(h.doctorExists(i)){
                   cout<<"A doctor with this ID already exists."<<endl;
                   break;
               }
               cout<<"Enter Doctor Name : ";cin>>n;
               cout<<"Enter Doctor Age : ";cin>>a;
               cout<<"Enter Doctor Gender (M/F): ";cin>>g;
               cout<<"Enter Doctor Availability : ";cin>>av;
               cout<<"Enter Doctor Specialisation : ";cin>>s;
               doctor d(i,n,a,g,av,s);
               h.AddDoctor(d);
               break;
            }
            case 3:{
                int i;
                string w;
                bool av;
                cout<<"Enter Bed ID : ";cin>>i;
                if(h.bedExists(i)){
                    cout<<"A bed with this ID already exists."<<endl;
                    break;
                }
               cout<<"Enter Ward Type : ";cin>>w;
               cout<<"Enter Bed Availability : ";cin>>av;
               bed b(i,w,av,-1);
               h.AddBed(b);
               break;
            }
            case 4:{
                h.showPatients();
                break;
            }
            case 5:{
                h.showDoctors();
                break;
            }
            case 6:{
                h.showBeds();
                break;
            }
            case 7:{
                int i,sev;
                string d;
                cout<<"Enter Patient ID : ";cin>>i;
                cout<<"Enter Severity (1-10) : ";cin>>sev;
                cout<<"Enter Diagnosis : ";cin>>d;
                h.queuePatient(i,sev,d);
                break;
            }
            case 8:{
                h.showQueue();
                break;
            }
            case 9:{
                h.assignDoctor();
                break;
            }
            case 10:{
                h.allocateBed();
                break;
            }
            case 11:{
                h.dischargePatient();
                break;
            }
            case 12:{
                h.showAdmitted();
                break;
            }
            case 13:{
                int pid,did;
                string date,time;
                cout<<"Enter Patient ID : ";cin>>pid;
                cout<<"Enter Doctor ID : ";cin>>did;
                cout<<"Enter Date (DD-MM-YYYY) : ";cin>>date;
                cout<<"Enter Time (HH:MM, 24-hour) : ";cin>>time;
                h.bookAppointment(pid,did,date+" "+time);
                break;
            }
            case 14:{
                int id;
                cout<<"Enter Appointment ID : ";cin>>id;
                h.cancelAppointment(id);
                break;
            }
            case 15:{
                int id;
                cout<<"Enter Appointment ID : ";cin>>id;
                h.completeAppointment(id);
                break;
            }
            case 16:{
                h.showAppointments();
                break;
            }
            case 17:{
                h.saveAppointments();
                break;
            }
            case 18:{
                h.loadAppointments();
                break;
            }
            case 19:{
                h.showFreeBeds();
                break;
            }
            case 20:
                return 0;
            default:
                cout<<"Invalid choice, try again."<<endl;

        }
    }
    return 0;
}