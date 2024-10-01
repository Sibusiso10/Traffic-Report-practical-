#include <iostream> 
#include <algorithm> // For std::transform
#include <cctype>    // For std::tolower
#include <string> 
#include <fstream> 
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdexcept>

//Class TrafficInformation, will be the main class to deal with all data handling and outputting summaries for data read from the files. 
//The reason for using a class for this project is to make it reuseable, and by creating a variable of type "TrafficInformation", you can pass in just the location of the 
//files location and get access to the data summaries needed. 

double percentCalculator(const int total, const int value) {    //Function to calculate the percentage
    return (static_cast<double>(value) / total) * 100;
}

//Class time to handle the data TYPES OF TIME. 
class Time{
    public: 
        Time(int hour = 0, int minute = 0) : hours(hour), mins(minute) {}

        bool operator < (const Time& time) const {
            if (hours == time.hours) {
                return mins < time.mins;
            }
            return hours < time.hours;
        }
            int getHour(){
                return hours; 
            }
            int getMinutes(){
                return mins; 
            }
    private: 

        int hours; 
        int mins; 
};


struct TrafficCollectionType{ //Data structure to collect the traffic data into one variable
    std::string date; 
    Time time; 
    std::string type; 
    int speed; 
}; 


struct PeakData{  //Data structure to collect the Peak traffic hours, can store the hour interval the start and end of the invterval and the number if vehicles in that interval. 
    int firstHour; 
    int endHour; 
    int numOfVehiclesPerHour;
};


class PeakTrafficTimesReport{
    public: 
        PeakTrafficTimesReport(const std::string date, const int startHour):firstDate(date), dateCount(0), intervalPerHourCount(0) {

            // Initialize the outer vector with 1 element (day or date), and initialize the inner vector
            trafficPeakData.push_back(std::vector<PeakData>(1));

            // Now, you can assign values to the first element of the inner vector
            trafficPeakData[0][0].firstHour = startHour;
            trafficPeakData[0][0].endHour = startHour + 1; // Example value for end hour
            trafficPeakData[0][0].numOfVehiclesPerHour = 1; // Initialize with 0 vehicles
        }
        void setFirstDate(const std::string date){
            firstDate = date; 
        }

        void setInterval(const int hour){
            trafficPeakData[dateCount][intervalPerHourCount].firstHour = hour; 
            trafficPeakData[dateCount][intervalPerHourCount].endHour = hour + 1; 
        }
        void setPeakData(const std::string currentDate,const int currentHour){
            if(firstDate == currentDate){
                // If within the current hour interval, increase the vehicle count
                if(trafficPeakData[dateCount][intervalPerHourCount].endHour > currentHour){
                    trafficPeakData[dateCount][intervalPerHourCount].numOfVehiclesPerHour ++; 
                }
                // If outside the current hour interval, create a new interval
                else if(trafficPeakData[dateCount][intervalPerHourCount].endHour <= currentHour){ 
                    intervalPerHourCount ++; 
                    PeakData tempPeakData; 
                    tempPeakData.firstHour = currentHour; 
                    tempPeakData.endHour = currentHour + 1; 
                    tempPeakData.numOfVehiclesPerHour = 1; 
                    trafficPeakData[dateCount].push_back(tempPeakData);  // Add new interval
                }
            }
            else{
                // New date, reset interval count and create new entry
                intervalPerHourCount = 0; 
                dateCount++; 
                firstDate = currentDate; 
                std::vector<PeakData> templist;
                // Initialize the new day with its first interval
                PeakData tempPeakData; 
                tempPeakData.firstHour = currentHour; 
                tempPeakData.endHour = currentHour + 1; 
                tempPeakData.numOfVehiclesPerHour = 1;

                templist.push_back(tempPeakData); 
                trafficPeakData.push_back(templist);  // Add new day to trafficPeakData
            }
        }

        void displayPeakData() const{
            for (const auto& dayData : trafficPeakData) {   // Iterate over each day
                for (const auto& peak : dayData) {          // Iterate over each hour interval
                    std::cout << "First Hour: " << peak.firstHour 
                            << "\t, End Hour: " << peak.endHour 
                            << "\t, Vehicles: " << peak.numOfVehiclesPerHour << std::endl;
                }
            }
        }
    private: 
        std::string firstDate; 
        int dateCount; 
        int intervalPerHourCount; 
        std::vector<std::vector<PeakData>> trafficPeakData;
};


class SpeedAnalysisReport{
    public: 
        SpeedAnalysisReport(){
            totalSpeed = 0; 
            numOfSlow = 0; 
            numOfMedium = 0;
            numOfFast = 0;
            imposibleSpeed = 0;  
        };

        void setTotalSpeed(const int speed){  //sets the total speed of all the cars
            totalSpeed += speed; 
        }

        double getAverage(const int totalNumberOfVehicles){    //Returns the average speed of vehicles passing over the specified period 
            return (static_cast<double>(totalSpeed) / totalNumberOfVehicles); 
        }

        void speedOrganizer(const int speed){   //Records the speeds of all the Vehicles and organizise them into proper classifications. 
             if(speed >= 0 && speed <= 30){
                numOfSlow++; 
             }
             else if(speed > 30 && speed < 60){
                numOfMedium++; 
             }
             else if( speed >= 60){
                numOfFast++; 
             }
             else {
                imposibleSpeed++; 
             }
        }

        void displaySpeedAnalysisReport(const int totalNumberOfVehicles){
            std::cout<<"\n--------------------------------------------------------\n"; 
            std::cout<< std::fixed << std::setprecision(2);
            std::cout<<"Speed Analysis Report: \n-----------------------\n\n"
                    <<"Average speed: "<< getAverage(totalNumberOfVehicles) <<"km/h"
                    <<"\n\nSpeed distribution: \n\n"
                    <<"-Slow(0-30 km/h): "<<numOfSlow <<" vehicles\n" 
                    <<"-Medium(30-60 km/h): "<<numOfMedium <<" vehicles\n"
                    <<"-Fats(60+ km/h): "<<numOfFast <<" vehicles\n";
            if(imposibleSpeed > 0){
                std::cout<<"Number of Impossibles speeds in the data file: "<< imposibleSpeed<<std::endl; 
            }
            std::cout<<"--------------------------------------------------------\n";
        };

    private: 
        int totalSpeed; 
        int numOfSlow; 
        int numOfMedium;
        int numOfFast; 
        int imposibleSpeed;  
};





class TrafficSummaryReport {
    public: 
        TrafficSummaryReport(std::string fDate): firstDate(fDate), numOfVehiclesPerDay(1, 1){ 
            numOfCars = 0; 
            numOfBuses = 0; 
            numOfTruck = 0;
            numOfMotorBikes = 0;
            day = 1; 
            total = 0;
        }; //Traffic Summary Report constructor
        void recordNumOfVehiclesPerDay(const std::string& date){//records and updates the member variables based on the data input
            if(firstDate == date){
                numOfVehiclesPerDay[day - 1] += 1; 
            }
            else {
                day++; 
                numOfVehiclesPerDay.push_back(1);
                firstDate = date;
            }

        }; 
        //Vehicle counter
        void vehicleCount(const std::string type) {
            if("CAR" == type){
                numOfCars++; 
            }
            else if("BUS" == type){
                numOfBuses++; 
            }
            else if("MOTORCYCLE" == type){
                numOfMotorBikes++; 
            }
            else if("TRUCK" == type){
                numOfTruck++; 
            }
        
        }
        //outputs the passed in date
        //allows the checking if the passed in value is correct for the developer
        void getFirstDate(){
            std::cout<<firstDate <<std::endl; 
        }
        int getTotalNumOfVehicles(){
            return total; 
        }
        //displays Traffic Summary Report 
        void displayTrafficSummaryReport(){
            int currentDay = 1; 
            std::cout<<"\n--------------------------------------------------------\n";
            std::cout<<"Traffic Summary Report: \n------------------------\n"<<std::endl; 
            

            //Display the sum of the vehicles per day
            for(int numOFVehicles : numOfVehiclesPerDay ){
                         total += numOFVehicles;
                }
            std::cout<<"Total vehicles: " << total << "\n"; 

            //Display the number of the vehicles per day
            for(int numOFVehicles : numOfVehiclesPerDay ){
               std::cout<<"Day "<< currentDay<< ": " <<numOFVehicles <<"\n\n";
               currentDay++;  
            }
            std::cout<<"------------------------\nVehicle distribution: \n"<<std::endl;
            std::cout << std::fixed << std::setprecision(2);
            std::cout<<"-Cars: "<< numOfCars << " ("<< percentCalculator(total, numOfCars) << "%)\n" 
                     <<"-Buses: "<< numOfBuses << " ("<< percentCalculator(total, numOfBuses) << "%)\n"
                     <<"-Motorbikes: "<< numOfMotorBikes << " ("<< percentCalculator(total, numOfMotorBikes) << "%)\n"
                     <<"-Truck: "<< numOfTruck << " ("<< percentCalculator(total, numOfTruck) << "%)\n" ;
            std::cout<<"--------------------------------------------------------\n";
        }; 

    private: 
    std::string firstDate; 
    std::vector<int> numOfVehiclesPerDay; 
    int day; 
    int numOfCars; 
    int numOfBuses; 
    int numOfMotorBikes; 
    int numOfTruck;
    int total;
};


class TrafficInformation
{
    public: 
        TrafficInformation(std::string fileLoc): fileLocation(fileLoc){}  //Contractor for the class TrafficInformation to initailize it's members. 
        std::string getFileLoca () const{
            return fileLocation; 
        }
        void displayDataSummary(){
            std::ifstream outFile(fileLocation); 
            
            std::string firstLine; 

            std::string date, timeVehicleSpeed, vehicle, time, speed; 

            std::string hours, minutes;

            

            if(outFile.is_open()){
                std::cout << "Everying is working" << "\n"<< std::endl;
            }

            std::getline(outFile, firstLine); 
            outFile >> date >> timeVehicleSpeed; 
            trafficSamReport = TrafficSummaryReport(date); // Declearing TrafficSummaryReport variable
            
            
            std::stringstream sep(timeVehicleSpeed);
                std::getline(sep, time, ';');
                std::getline(sep, vehicle, ';');
                std::getline(sep, speed, ';');

            speedAnaReport.setTotalSpeed(std::stoi(speed)); 
            speedAnaReport.speedOrganizer(std::stoi(speed)); 

            trafficSamReport.vehicleCount(vehicle); // Passing the first vehicle for recording

            std::stringstream theTime(time);
            std::getline(theTime, hours, ':');
            std::getline(theTime, minutes, ':');

            peakTrafficTimesReport = PeakTrafficTimesReport(date, std::stoi(hours)); 

            while(outFile >> date >> timeVehicleSpeed){ 

                TrafficCollectionType data;
                data.date = date; 
                std::stringstream separator(timeVehicleSpeed);
                std::getline(separator, time, ';');
                std::getline(separator, data.type, ';');
                std::getline(separator, speed, ';');

                std::stringstream timeStream(time);
                std::getline(timeStream, hours, ':');
                std::getline(timeStream, minutes, ':');


                data.time = Time(std::stoi(hours), std::stoi(minutes));
                data.speed = std::stoi(speed); 

                trafficSamReport.recordNumOfVehiclesPerDay(data.date); // Record the traffic data
                trafficSamReport.vehicleCount(data.type);
                
                speedAnaReport.setTotalSpeed(data.speed);
                speedAnaReport.speedOrganizer(data.speed); 

                peakTrafficTimesReport.setPeakData(data.date, std::stoi(hours));
            }

            outFile.close();  // Close the file

            trafficSamReport.displayTrafficSummaryReport();  // Display summary
            std::cout<<"\n\n\n"; 
            speedAnaReport.displaySpeedAnalysisReport(trafficSamReport.getTotalNumOfVehicles()); 
            std::cout<<"\n\n\n"; 
            peakTrafficTimesReport.displayPeakData(); 

        }; //This member function is responsible for reading the traffic data. 
    private:
        std::string fileLocation;  //member variable for storing the files LocationName as one string. 
        TrafficSummaryReport trafficSamReport{" "};
        SpeedAnalysisReport speedAnaReport; 
        PeakTrafficTimesReport peakTrafficTimesReport;
};





int main(){
    TrafficInformation dataset("trafficdata (1).txt"); //declearing variable of type TrafficInformation, and passing in file name{location}. 
    std::cout<< dataset.getFileLoca()<<std::endl; 
    dataset.displayDataSummary(); 

    return 0; 
}
