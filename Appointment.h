#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include<iostream>
#include<string>
#include<vector>
#include<ctime>

using namespace std;

const int SLOT_MINUTES=30;   // every appointment lasts one 30-minute slot

class appointment{
    private:
        int appointment_id;
        int patient_id;
        int doctor_id;
        time_t start_time;
        string status;       // "Scheduled", "Cancelled", "Completed"

    public:
        appointment(int appointment_id,int patient_id,int doctor_id,time_t start_time);

        int GetAppointmentID();
        int GetPatientID();
        int GetDoctorID();
        time_t GetStartTime();
        string GetStatus();
        string GetTimeString();

        void Cancel();
        void Complete();
        bool IsActive();                          // true only if Scheduled
        bool Overlaps(time_t other_start);        // within the same 30-minute slot window
        void AppointmentDisplay();
};

// Manages all appointments and prevents conflicts
class appointmentbook{
    private:
        vector<appointment> appointments;
        int next_id;

    public:
        appointmentbook();

        // "DD-MM-YYYY HH:MM" -> time_t, returns -1 if the text is invalid
        static time_t ParseDateTime(string text);

        bool DoctorIsFree(int doctor_id,time_t start_time);
        bool PatientIsFree(int patient_id,time_t start_time);

        // returns appointment_id on success
        // -1 = invalid date/time, -2 = time is in the past,
        // -3 = doctor already booked, -4 = patient already has an appointment then
        int BookAppointment(int patient_id,int doctor_id,string date_time_text);

        bool CancelAppointment(int appointment_id);
        bool CompleteAppointment(int appointment_id);
        appointment* FindAppointment(int appointment_id);

        void DisplayAll();
        void DisplayForDoctor(int doctor_id);
        void DisplayForPatient(int patient_id);

        bool SaveAppointments(string filename);
        bool LoadAppointments(string filename);
};

#endif
