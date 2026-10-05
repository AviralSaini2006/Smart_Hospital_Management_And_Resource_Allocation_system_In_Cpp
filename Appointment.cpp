#include"Appointment.h"
#include<iostream>
#include<fstream>
#include<cstdio>
#include<chrono>

using namespace std;

    // ---------- appointment ----------
    appointment::appointment(int appointment_id,int patient_id,int doctor_id,time_t start_time){
        this->appointment_id=appointment_id;
        this->patient_id=patient_id;
        this->doctor_id=doctor_id;
        this->start_time=start_time;
        status="Scheduled";
    }
    int appointment::GetAppointmentID(){ return appointment_id; }
    int appointment::GetPatientID(){ return patient_id; }
    int appointment::GetDoctorID(){ return doctor_id; }
    time_t appointment::GetStartTime(){ return start_time; }
    string appointment::GetStatus(){ return status; }
    string appointment::GetTimeString(){
        char buf[32];
        strftime(buf,sizeof(buf),"%d-%m-%Y %H:%M",localtime(&start_time));
        return string(buf);
    }
    void appointment::Cancel(){ status="Cancelled"; }
    void appointment::Complete(){ status="Completed"; }
    bool appointment::IsActive(){ return status=="Scheduled"; }
    bool appointment::Overlaps(time_t other_start){
        double diff=difftime(other_start,start_time);
        if(diff<0) diff=-diff;
        return diff<SLOT_MINUTES*60;
    }
    void appointment::AppointmentDisplay(){
        cout<<"Appointment ID : "<<appointment_id<<endl;
        cout<<"Patient ID : "<<patient_id<<endl;
        cout<<"Doctor ID : "<<doctor_id<<endl;
        cout<<"Time : "<<GetTimeString()<<endl;
        cout<<"Status : "<<status<<endl;
    }

    // ---------- appointmentbook ----------
    appointmentbook::appointmentbook(){
        next_id=1;
    }

    time_t appointmentbook::ParseDateTime(string text){
        int d,m,y,h,mi;
        char extra;
        if(sscanf(text.c_str(),"%d-%d-%d %d:%d %c",&d,&m,&y,&h,&mi,&extra)!=5) return -1;
        if(y<2000||m<1||m>12||d<1||d>31||h<0||h>23||mi<0||mi>59) return -1;
        tm t={};
        t.tm_mday=d; t.tm_mon=m-1; t.tm_year=y-1900;
        t.tm_hour=h; t.tm_min=mi; t.tm_sec=0;
        t.tm_isdst=-1;
        time_t result=mktime(&t);
        if(t.tm_mday!=d||t.tm_mon!=m-1) return -1;   // e.g. 31-02-2026 got rolled over
        return result;
    }

    bool appointmentbook::DoctorIsFree(int doctor_id,time_t start_time){
        for(size_t i=0;i<appointments.size();i++){
            if(appointments[i].IsActive() && appointments[i].GetDoctorID()==doctor_id
               && appointments[i].Overlaps(start_time)) return false;
        }
        return true;
    }
    bool appointmentbook::PatientIsFree(int patient_id,time_t start_time){
        for(size_t i=0;i<appointments.size();i++){
            if(appointments[i].IsActive() && appointments[i].GetPatientID()==patient_id
               && appointments[i].Overlaps(start_time)) return false;
        }
        return true;
    }

    int appointmentbook::BookAppointment(int patient_id,int doctor_id,string date_time_text){
        time_t t=ParseDateTime(date_time_text);
        if(t==-1) return -1;
        time_t now=chrono::system_clock::to_time_t(chrono::system_clock::now());
        if(t<now) return -2;
        if(!DoctorIsFree(doctor_id,t)) return -3;
        if(!PatientIsFree(patient_id,t)) return -4;
        appointments.push_back(appointment(next_id,patient_id,doctor_id,t));
        return next_id++;
    }

    appointment* appointmentbook::FindAppointment(int appointment_id){
        for(size_t i=0;i<appointments.size();i++){
            if(appointments[i].GetAppointmentID()==appointment_id) return &appointments[i];
        }
        return nullptr;
    }
    bool appointmentbook::CancelAppointment(int appointment_id){
        appointment* a=FindAppointment(appointment_id);
        if(a==nullptr||!a->IsActive()) return false;
        a->Cancel();
        return true;
    }
    bool appointmentbook::CompleteAppointment(int appointment_id){
        appointment* a=FindAppointment(appointment_id);
        if(a==nullptr||!a->IsActive()) return false;
        a->Complete();
        return true;
    }

    void appointmentbook::DisplayAll(){
        cout<<"---- All Appointments ----"<<endl;
        for(size_t i=0;i<appointments.size();i++){
            appointments[i].AppointmentDisplay();
            cout<<"--------------------------"<<endl;
        }
    }
    void appointmentbook::DisplayForDoctor(int doctor_id){
        cout<<"---- Appointments for Doctor "<<doctor_id<<" ----"<<endl;
        for(size_t i=0;i<appointments.size();i++){
            if(appointments[i].GetDoctorID()==doctor_id){
                appointments[i].AppointmentDisplay();
                cout<<"--------------------------"<<endl;
            }
        }
    }
    void appointmentbook::DisplayForPatient(int patient_id){
        cout<<"---- Appointments for Patient "<<patient_id<<" ----"<<endl;
        for(size_t i=0;i<appointments.size();i++){
            if(appointments[i].GetPatientID()==patient_id){
                appointments[i].AppointmentDisplay();
                cout<<"--------------------------"<<endl;
            }
        }
    }

    // File format (one per line): id patient_id doctor_id start_time status
    bool appointmentbook::SaveAppointments(string filename){
        ofstream out(filename.c_str());
        if(!out) return false;
        for(size_t i=0;i<appointments.size();i++){
            out<<appointments[i].GetAppointmentID()<<" "<<appointments[i].GetPatientID()<<" "
               <<appointments[i].GetDoctorID()<<" "<<(long long)appointments[i].GetStartTime()<<" "
               <<appointments[i].GetStatus()<<"\n";
        }
        return true;
    }
    bool appointmentbook::LoadAppointments(string filename){
        ifstream in(filename.c_str());
        if(!in) return false;
        appointments.clear();
        next_id=1;
        int id,pid,did;
        long long t;
        string status;
        while(in>>id>>pid>>did>>t>>status){
            appointment a(id,pid,did,(time_t)t);
            if(status=="Cancelled") a.Cancel();
            else if(status=="Completed") a.Complete();
            appointments.push_back(a);
            if(id>=next_id) next_id=id+1;
        }
        return true;
    }
