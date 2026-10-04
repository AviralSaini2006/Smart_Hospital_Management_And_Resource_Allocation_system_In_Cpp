#ifndef BED_H
#define BED_H

#include<iostream>
#include<string>

using namespace std;

class bed{
    private:
       int bed_id,patient_bed_id;
       string ward_type;
       bool availability;
    public:
    bed(int bed_id,string ward_type,bool availability,int patient_bed_id);
    int GetBedID();
    string GetWardType();
    bool GetBedAvailability();
    int GetPatientBedID();
    void BedDisplay();
};

#endif