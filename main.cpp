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
        cout<<"7) Exit Program"<<endl;
        cout<<"Answer : ";cin>>ch;
        switch(ch){
            case 1:{
               int i,a;
               string n;
               char g;
               cout<<"Enter Patient ID : ";cin>>i;
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
            case 7:
                return 0;

        }
    }
    return 0;
}