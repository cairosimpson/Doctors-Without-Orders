#include "DoctorsWithoutOrders.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

/* * * * Doctors Without Orders * * * */

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool solve(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule);
void initializeMap(Vector<Doctor> &doctors,Map<string, Set<string>>& schedule);
bool hasTime(Vector<Doctor> &doctors,Vector<Patient> &patients, Map<string, Set<string>>& schedule, int index);



//This function determines if all patients can be seen (returns result as boolean)
bool canAllPatientsBeSeen(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule) {
    initializeMap(doctors, schedule);

    return solve(doctors,patients, schedulew);
}

//This function recursively determines if all patients can be seen and is called by canAllPatientsBeSeen
bool solve(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule){
    if(patients.size() == 0) // BASE CASE
            return true;
    for(int i = 0; i < doctors.size(); i++){
        for(int j = 0; j < patients.size(); j++){
            Patient holder = patients.get(j);

            if(doctors.get(i).hoursFree - patients.get(j).hoursNeeded >= 0){ //if doctor can fit in patient
                doctors[i].hoursFree = doctors.get(i).hoursFree - patients.get(j).hoursNeeded; //updates doctors availability
                Set<string> personalSchedule = schedule.get(doctors.get(i).name);
                personalSchedule.add(patients.get(j).name);
                schedule.add(doctors.get(i).name, personalSchedule); //adds patient to doctors schedule in map
                patients.remove(j); //takes out patient
            }
            else continue; //ensures that it doesnt unmake choice that was never made

            if(solve(doctors, patients, schedule)) return true;

            patients.insert(j, holder); //readds patient
            doctors[i].hoursFree = doctors.get(i).hoursFree + patients.get(j).hoursNeeded; //resets doctors availability
            Set<string> personalSchedule = schedule.get(doctors.get(i).name);
            personalSchedule.remove(patients.get(j).name);//removes patient from schedule
            schedule.add(doctors.get(i).name, personalSchedule); //updates map

        }

    }
        return false;

}

//This function adds every doctor's name to map with an empty set associated to each name
void initializeMap(Vector<Doctor> &doctors,Map<string, Set<string>>& schedule){
    for(int i = 0; i < doctors.size(); i++){
        Set<string> s;
        schedule.add(doctors.get(i).name,s);
    }
}
