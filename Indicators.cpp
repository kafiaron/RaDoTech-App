#include "Indicators.h"

////Constructor
Indicators::Indicators(std::map<std::string, float> meridianPoint): meridianPoint(meridianPoint){
    processedData["energyLevel"] = 0.0f;
    processedData["immuneSystem"] = 0.0f;
    processedData["metabolism"] = 0.0f;
    processedData["psychoEmotional"] = 0.0f;
    processedData["musculoskeletal"] = 0.0f;
    processedData["leftMeridian"] = 0.0f;
    processedData["rightMeridian"] = 0.0f;
    processedData["upperMeridian"] = 0.0f;
    processedData["lowerMeridian"] = 0.0f;
}

////Destructor
Indicators::~Indicators(){}

///////////////////////
////Main indicators////
///////////////////////

//Energy: Average of the conductance of all the meridian points 
void Indicators::calculateEnergyLevel(double max, double min){
    float energyLevel = 0.0f; 
    for (std::map<std::string, float>::const_iterator it = meridianPoint.begin(); it != meridianPoint.end(); ++it) {
        energyLevel += it->second;
    }
    processedData["energyLevel"] = energyLevel/24;

    if(energyLevel/24 < min)    {status["energyLevel"]=0;}
    else if(energyLevel/24> max){status["energyLevel"]=1;}
    else                        {status["energyLevel"]=2;}
}

//Immune System: Average of the conductance of the Large Intestine(h6), Pancreas(f1), Lung(h1), and Lymph Vessel(h5)
void Indicators::calculateImmuneSystem(double max, double min){
    float immuneSystem = 0.0f;
    immuneSystem += meridianPoint["h1Left"];   immuneSystem += meridianPoint["h1Right"];
    immuneSystem += meridianPoint["h5Left"];   immuneSystem += meridianPoint["h5Right"];
    immuneSystem += meridianPoint["h6Left"];   immuneSystem += meridianPoint["h6Right"];
    immuneSystem += meridianPoint["f1Left"];   immuneSystem += meridianPoint["f1Right"];
    processedData["immuneSystem"] = immuneSystem/8;

    if(immuneSystem/8 < min)     {status["immuneSystem"]=0;}
    else if(immuneSystem/8 > max){status["immuneSystem"]=1;}
    else                         {status["immuneSystem"]=2;}
}

//Metabolism: Average of the conductance of the Liver(f2), Pancreas(f1), Stomach(f6), and Kidney(f3)
void Indicators::calculateMetabolism(double max, double min){
    float metabolism = 0.0f;
    metabolism += meridianPoint["f1Left"];   metabolism += meridianPoint["f1Right"];
    metabolism += meridianPoint["f2Left"];   metabolism += meridianPoint["f2Right"];
    metabolism += meridianPoint["f3Left"];   metabolism += meridianPoint["f3Right"];
    metabolism += meridianPoint["f6Left"];   metabolism += meridianPoint["f6Right"];
    processedData["metabolism"] = metabolism/8;

    if(metabolism/8 < min)     {status["metabolism"]=0;}
    else if(metabolism/8 > max){status["metabolism"]=1;}
    else                       {status["metabolism"]=2;}
}

//Psycho Emotional: Average of the conductance of the Heart(h3), Pericardium(h2), Kidney(f3), Lymph Vessel(h5), and Liver(f2)
void Indicators::calculatePsychoEmotional(double max, double min){
    float psychoEmotional = 0.0f;
    psychoEmotional += meridianPoint["h2Left"];   psychoEmotional += meridianPoint["h2Right"];
    psychoEmotional += meridianPoint["h3Left"];   psychoEmotional += meridianPoint["h3Right"];
    psychoEmotional += meridianPoint["h5Left"];   psychoEmotional += meridianPoint["h5Right"];
    psychoEmotional += meridianPoint["f2Left"];   psychoEmotional += meridianPoint["f2Right"];
    psychoEmotional += meridianPoint["f3Left"];   psychoEmotional += meridianPoint["f3Right"];
    processedData["psychoEmotional"] = psychoEmotional/10;

    if(psychoEmotional/10 < min)     {status["psychoEmotional"]=0;}
    else if(psychoEmotional/10 > max){status["psychoEmotional"]=1;}
    else                             {status["psychoEmotional"]=2;}
}

//Musculoskeletal: Average of the conductance of the Pancreas(f1), Stomach(f6), Liver(f2), Kidney(f3), Bladder(f4), Gallbladder(f5)
void Indicators::calculateMusculoskeletal(double max, double min){
    float musculoskeletal = 0.0f;
    musculoskeletal += meridianPoint["f1Left"];   musculoskeletal += meridianPoint["f1Right"];
    musculoskeletal += meridianPoint["f2Left"];   musculoskeletal += meridianPoint["f2Right"];
    musculoskeletal += meridianPoint["f3Left"];   musculoskeletal += meridianPoint["f3Right"];
    musculoskeletal += meridianPoint["f4Left"];   musculoskeletal += meridianPoint["f4Right"];
    musculoskeletal += meridianPoint["f5Left"];   musculoskeletal += meridianPoint["f5Right"];
    musculoskeletal += meridianPoint["f6Left"];   musculoskeletal += meridianPoint["f6Right"];
    processedData["musculoskeletal"] = musculoskeletal/12;

    if(musculoskeletal/12 < min)     {status["musculoskeletal"]=0;}
    else if(musculoskeletal/12 > max){status["musculoskeletal"]=1;}
    else                             {status["musculoskeletal"]=2;}
}

//////////////////////////
////Summary indicators////
//////////////////////////

// - Average level is the Energy Level

//Sum of all the meridian points on the left side of the body
void Indicators::calculateLeftMeridian(){
    float leftMeridian = 0.0f; 
    leftMeridian += meridianPoint["f1Left"];   leftMeridian += meridianPoint["h1Left"];
    leftMeridian += meridianPoint["f2Left"];   leftMeridian += meridianPoint["h2Left"];
    leftMeridian += meridianPoint["f3Left"];   leftMeridian += meridianPoint["h3Left"];
    leftMeridian += meridianPoint["f4Left"];   leftMeridian += meridianPoint["h4Left"];
    leftMeridian += meridianPoint["f5Left"];   leftMeridian += meridianPoint["h5Left"];
    leftMeridian += meridianPoint["f6Left"];   leftMeridian += meridianPoint["h6Left"];
    processedData["leftMeridian"] = leftMeridian;
}

//Sum of all the meridian points on the right side of the body
void Indicators::calculateRightMeridian(){
    float rightMeridian = 0.0f; 
    rightMeridian += meridianPoint["f1Right"];   rightMeridian += meridianPoint["h1Right"];
    rightMeridian += meridianPoint["f2Right"];   rightMeridian += meridianPoint["h2Right"];
    rightMeridian += meridianPoint["f3Right"];   rightMeridian += meridianPoint["h3Right"];
    rightMeridian += meridianPoint["f4Right"];   rightMeridian += meridianPoint["h4Right"];
    rightMeridian += meridianPoint["f5Right"];   rightMeridian += meridianPoint["h5Right"];
    rightMeridian += meridianPoint["f6Right"];   rightMeridian += meridianPoint["h6Right"];
    processedData["rightMeridian"] = rightMeridian;
}

//Sum of all the meridian points on the upper body (all hand points)
void Indicators::calculateUpperMeridian(){
    float upperMeridian = 0.0f; 
    upperMeridian += meridianPoint["h1Left"];   upperMeridian += meridianPoint["h1Right"];
    upperMeridian += meridianPoint["h2Left"];   upperMeridian += meridianPoint["h2Right"];
    upperMeridian += meridianPoint["h3Left"];   upperMeridian += meridianPoint["h3Right"];
    upperMeridian += meridianPoint["h4Left"];   upperMeridian += meridianPoint["h4Right"];
    upperMeridian += meridianPoint["h5Left"];   upperMeridian += meridianPoint["h5Right"];
    upperMeridian += meridianPoint["h6Left"];   upperMeridian += meridianPoint["h6Right"];
    processedData["upperMeridian"] = upperMeridian;
}

//Sum of all the meridian points on the lower body (all feet points)
void Indicators::calculateLowerMeridian(){
    float lowerMeridian = 0.0f; 
    lowerMeridian += meridianPoint["f1Left"];   lowerMeridian += meridianPoint["f1Right"];
    lowerMeridian += meridianPoint["f2Left"];   lowerMeridian += meridianPoint["f2Right"];
    lowerMeridian += meridianPoint["f3Left"];   lowerMeridian += meridianPoint["f3Right"];
    lowerMeridian += meridianPoint["f4Left"];   lowerMeridian += meridianPoint["f4Right"];
    lowerMeridian += meridianPoint["f5Left"];   lowerMeridian += meridianPoint["f5Right"];
    lowerMeridian += meridianPoint["f6Left"];   lowerMeridian += meridianPoint["f6Right"];
    processedData["lowerMeridian"] = lowerMeridian;
}

//Returns the map of calculated averages of the meridians points
std::map<std::string, float> Indicators::getProcessedData() const{
    return processedData;
}

//Returns the map of calculating whether the averages were below norm, normal, or above norm
std::map<std::string, float> Indicators::getStatus() const{
    return status;
}
