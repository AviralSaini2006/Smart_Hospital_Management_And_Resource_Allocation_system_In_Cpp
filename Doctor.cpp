#include<iostream>
#include "Doctor.h"

using namespace std;

       doctor::doctor(int doctor_id,string doctor_name,int doctor_age,char doctor_gender,bool availability,string specialisation){
        this->doctor_id=doctor_id;
        this->doctor_name=doctor_name;
        this->doctor_age=doctor_age;
        this->doctor_gender=doctor_gender;
        this->availability=availability;
        this->specialisation=specialisation;
       }
       int doctor::GetDocID(){
        return doctor_id;
       }
       string doctor::GetDocName(){
        return doctor_name;
       }
       int doctor::GetDocAge(){
        return doctor_age;
       }
       char doctor::GetDocGender(){
        return doctor_gender;
       }
       bool doctor::GetDocAvailability(){
        return availability;
       }
       string doctor::GetDocSpecialisation(){
        return specialisation;
       }
       void doctor::DoctorDisplay(){
        cout<<"Doctor ID : "<<GetDocID()<<endl;
        cout<<"Doctor Name : "<<GetDocName()<<endl;
        cout<<"Doctor Age : "<<GetDocAge()<<endl;
        cout<<"Doctor Gender : "<<GetDocGender()<<endl;
        cout<<"Doctor Availability : "<<(GetDocAvailability()?"Available":"Occupied")<<endl;
        cout<<"Doctor Specialisation : "<<GetDocSpecialisation()<<endl;
       }