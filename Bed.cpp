#include"Bed.h"
#include<iostream>

using namespace std;

    bed::bed(int bed_id,string ward_type,bool availability,int patient_bed_id){
        this->bed_id=bed_id;
        this->ward_type=ward_type;
        this->availability=availability;
        this->patient_bed_id=patient_bed_id;
    }
    int bed::GetBedID(){
        return bed_id;
    }
    string bed::GetWardType(){
        return ward_type;
    }
    bool bed::GetBedAvailability(){
        return availability;
    }
    int bed::GetPatientBedID(){
        return patient_bed_id;
    }
    void bed::BedDisplay(){
        cout<<"Bed ID : "<<GetBedID()<<endl;
        cout<<"Ward Type : "<<GetWardType()<<endl;
        cout<<"Bed Availability : "<<(GetBedAvailability()?"Available":"Occupied")<<endl;
        cout<<"Patient using Bed ID : "<<GetPatientBedID()<<endl;
    }