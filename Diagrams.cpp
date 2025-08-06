#include "Diagrams.h"

////Constructor
Diagrams::Diagrams(std::map<std::string, float> meridianPoint){
    this->meridianPoint = meridianPoint; 
}

//Calculates if the internals associated with the meridian points are functioning insufficiently, hyperactively or normally
void Diagrams::calculateBodyChartData(double max, double min){
    if(meridianPoint["h1Left"] < min)       {/*LEFT LUNG BLUE*/     bodyChartData["lungL"]=0;}
    else if(meridianPoint["h1Left"] > max)  {/*LEFT LUNG RED*/      bodyChartData["lungL"]=1;}
    else                                    {/*LEFT LUNG GREEN*/    bodyChartData["lungL"]=2;}
    if(meridianPoint["h1Right"] < min)      {/*RIGHT LUNG BLUE*/    bodyChartData["lungR"]=0;}
    else if(meridianPoint["h1Right"] > max) {/*RIGHT LUNG RED*/     bodyChartData["lungR"]=1;}
    else                                    {/*RIGHT LUNG GREEN*/   bodyChartData["lungR"]=2;}
    
    if(meridianPoint["h2Left"] < min)       {/*LEFT PERICARDIUM BLUE*/  bodyChartData["periL"]=0;}
    else if(meridianPoint["h2Left"] > max)  {/*LEFT PERICARDIUM RED*/   bodyChartData["periL"]=1;}
    else                                    {/*LEFT PERICARDIUM GREEN*/ bodyChartData["periL"]=2;}
    if(meridianPoint["h2Right"] < min)      {/*RIGHT PERICARDIUM BLUE*/ bodyChartData["periR"]=0;}
    else if(meridianPoint["h2Right"] > max) {/*RIGHT PERICARDIUM RED*/  bodyChartData["periR"]=1;}
    else                                    {/*RIGHT PERICARDIUM GREEN*/bodyChartData["periR"]=2;}
    
    if(meridianPoint["h3Left"] < min)       {/*LEFT HEART BLUE*/    bodyChartData["heartL"]=0;}
    else if(meridianPoint["h3Left"] > max)  {/*LEFT HEART RED*/     bodyChartData["heartL"]=1;}
    else                                    {/*LEFT HEART GREEN*/   bodyChartData["heartL"]=2;}
    if(meridianPoint["h3Right"] < min)      {/*RIGHT HEART BLUE*/   bodyChartData["heartR"]=0;}
    else if(meridianPoint["h3Right"] > max) {/*RIGHT HEART RED*/    bodyChartData["heartR"]=1;}
    else                                    {/*RIGHT HEART GREEN*/  bodyChartData["heartR"]=2;}
   
    if(meridianPoint["h4Left"] < min)       {/*LEFT S.INTEST BLUE*/ bodyChartData["smallInL"]=0;}
    else if(meridianPoint["h4Left"] > max)  {/*LEFT S.INTEST RED*/  bodyChartData["smallInL"]=1;}
    else                                    {/*LEFT S.INTEST GREEN*/bodyChartData["smallInL"]=2;}
    if(meridianPoint["h4Right"] < min)      {/*RIGHT S.INTEST BLUE*/bodyChartData["smallInR"]=0;}
    else if(meridianPoint["h4Right"] > max) {/*RIGHT S.INTEST RED*/ bodyChartData["smallInR"]=1;}
    else                                    {/*RIGHT S.INTEST GREEN*/bodyChartData["smallInR"]=2;}
    
    if(meridianPoint["h5Left"] < min)       {/*LEFT LYMPH BLUE*/    bodyChartData["lymphL"]=0;}
    else if(meridianPoint["h5Left"] > max)  {/*LEFT LYMPH RED*/     bodyChartData["lymphL"]=1;}
    else                                    {/*LEFT LYMPH GREEN*/   bodyChartData["lymphL"]=2;}
    if(meridianPoint["h5Right"] < min)      {/*RIGHT LYMPH BLUE*/   bodyChartData["lymphR"]=0;}
    else if(meridianPoint["h5Right"] > max) {/*RIGHT LYMPH RED*/    bodyChartData["lymphR"]=1;}
    else                                    {/*RIGHT LYMPH GREEN*/  bodyChartData["lymphR"]=2;}
   
    if(meridianPoint["h6Left"] < min)       {/*LEFT L.INTEST BLUE*/ bodyChartData["largeInL"]=0;}
    else if(meridianPoint["h6Left"] > max)  {/*LEFT L.INTEST RED*/  bodyChartData["largeInL"]=1;}
    else                                    {/*LEFT L.INTEST GREEN*/bodyChartData["largeInL"]=2;}
    if(meridianPoint["h6Right"] < min)      {/*RIGHT L.INTEST BLUE*/bodyChartData["largeInR"]=0;}
    else if(meridianPoint["h6Right"] > max) {/*RIGHT L.INTEST RED*/ bodyChartData["largeInR"]=1;}
    else                                    {/*RIGHT L.INTEST GREEN*/bodyChartData["largeInR"]=2;}
  
    if(meridianPoint["f1Left"] < min)       {/*LEFT SPLEEN BLUE*/   bodyChartData["pancL"]=0;}
    else if(meridianPoint["f1Left"] > max)  {/*LEFT SPLEEN RED*/    bodyChartData["pancL"]=1;}
    else                                    {/*LEFT SPLEEN GREEN*/  bodyChartData["pancL"]=2;}
    if(meridianPoint["f1Right"] < min)      {/*RIGHT SPLEEN BLUE*/  bodyChartData["pancR"]=0;}
    else if(meridianPoint["f1Right"] > max) {/*RIGHT SPLEEN RED*/   bodyChartData["pancR"]=1;}
    else                                    {/*RIGHT SPLEEN GREEN*/ bodyChartData["pancR"]=2;}
  
    if(meridianPoint["f2Left"] < min)       {/*LEFT LIVER BLUE*/    bodyChartData["liverL"]=0;}
    else if(meridianPoint["f2Left"] > max)  {/*LEFT LIVER RED*/     bodyChartData["liverL"]=1;}
    else                                    {/*LEFT LIVER GREEN*/   bodyChartData["liverL"]=2;}
    if(meridianPoint["f2Right"] < min)      {/*RIGHT LIVER BLUE*/   bodyChartData["liverR"]=0;}
    else if(meridianPoint["f2Right"] > max) {/*RIGHT LIVER RED*/    bodyChartData["liverR"]=1;}
    else                                    {/*RIGHT LIVER GREEN*/  bodyChartData["liverR"]=2;}
 
    if(meridianPoint["f3Left"] < min)       {/*LEFT KIDNEY BLUE*/   bodyChartData["kidneyL"]=0;}
    else if(meridianPoint["f3Left"] > max)  {/*LEFT KIDNEY RED*/    bodyChartData["kidneyL"]=1;}
    else                                    {/*LEFT KIDNEY GREEN*/  bodyChartData["kidneyL"]=2;}
    if(meridianPoint["f3Right"] < min)      {/*RIGHT KIDNEY BLUE*/  bodyChartData["kidneyR"]=0;}
    else if(meridianPoint["f3Right"] > max) {/*RIGHT KIDNEY RED*/   bodyChartData["kidneyR"]=1;}
    else                                    {/*RIGHT KIDNEY GREEN*/ bodyChartData["kidneyR"]=2;}
 
    if(meridianPoint["f4Left"] < min)       {/*LEFT BLADDER BLUE*/  bodyChartData["bladderL"]=0;}
    else if(meridianPoint["f4Left"] > max)  {/*LEFT BLADDER RED*/   bodyChartData["bladderL"]=1;}
    else                                    {/*LEFT BLADDER GREEN*/ bodyChartData["bladderL"]=2;}
    if(meridianPoint["f4Right"] < min)      {/*RIGHT BLADDER BLUE*/ bodyChartData["bladderR"]=0;}
    else if(meridianPoint["f4Right"] > max) {/*RIGHT BLADDER RED*/  bodyChartData["bladderR"]=1;}
    else                                    {/*RIGHT BLADDER GREEN*/bodyChartData["bladderR"]=2;}
  
    if(meridianPoint["f5Left"] < min)       {/*LEFT GALL BLUE*/ bodyChartData["gBladderL"]=0;}
    else if(meridianPoint["f5Left"] > max)  {/*LEFT GALL RED*/  bodyChartData["gBladderL"]=1;}
    else                                    {/*LEFT GALL GREEN*/bodyChartData["gBladderL"]=2;}
    if(meridianPoint["f5Right"] < min)      {/*RIGHT GALL BLUE*/bodyChartData["gBladderR"]=0;}
    else if(meridianPoint["f5Right"] > max) {/*RIGHT GALL RED*/ bodyChartData["gBladderR"]=1;}
    else                                    {/*RIGHT GALL GREEN*/bodyChartData["gBladderR"]=2;}
  
    if(meridianPoint["f6Left"] < min)       {/*LEFT STOMACH BLUE*/  bodyChartData["stomachL"]=0;}
    else if(meridianPoint["f6Left"] > max)  {/*LEFT STOMACH RED*/   bodyChartData["stomachL"]=1;}
    else                                    {/*LEFT STOMACH GREEN*/ bodyChartData["stomachL"]=2;}
    if(meridianPoint["f6Right"] < min)      {/*RIGHT STOMACH BLUE*/ bodyChartData["stomachR"]=0;}
    else if(meridianPoint["f6Right"] > max) {/*RIGHT STOMACH RED*/  bodyChartData["stomachR"]=1;}
    else                                    {/*RIGHT STOMACH GREEN*/bodyChartData["stomachR"]=2;}
}

//Calculates the difference in percentages of the conductance of a meridian point to the average conductuce value of the user
void Diagrams::calculateBarChartData(double max, double min){
    double avgConductance = (max + min)/2;
    
    barChartData["h1Left"] = (meridianPoint["h1Left"]/avgConductance)*100;
    barChartData["h1Right"] = (meridianPoint["h1Right"]/avgConductance)*100;

    barChartData["h2Left"] = (meridianPoint["h2Left"]/avgConductance)*100;
    barChartData["h2Right"] = (meridianPoint["h2Right"]/avgConductance)*100;

    barChartData["h3Left"] = (meridianPoint["h3Left"]/avgConductance)*100;
    barChartData["h3Right"] = (meridianPoint["h3Right"]/avgConductance)*100;
   
    barChartData["h4Left"] = (meridianPoint["h4Left"]/avgConductance)*100;
    barChartData["h4Right"] = (meridianPoint["h4Right"]/avgConductance)*100;
    
    barChartData["h5Left"] = (meridianPoint["h5Left"]/avgConductance)*100;
    barChartData["h5Right"] = (meridianPoint["h5Right"]/avgConductance)*100;
   
    barChartData["h6Left"] = (meridianPoint["h6Left"]/avgConductance)*100;
    barChartData["h6Right"] = (meridianPoint["h6Right"]/avgConductance)*100;
  
    barChartData["f1Left"] = (meridianPoint["f1Left"]/avgConductance)*100;
    barChartData["f1Right"] = (meridianPoint["f1Right"]/avgConductance)*100;
  
    barChartData["f2Left"] = (meridianPoint["f2Left"]/avgConductance)*100;
    barChartData["f2Right"] = (meridianPoint["f2Right"]/avgConductance)*100;

    barChartData["f3Left"] = (meridianPoint["f3Left"]/avgConductance)*100;
    barChartData["f3Right"] = (meridianPoint["f3Right"]/avgConductance)*100;

    barChartData["f4Left"] = (meridianPoint["f4Left"]/avgConductance)*100;
    barChartData["f4Right"] = (meridianPoint["f4Right"]/avgConductance)*100;

    barChartData["f5Left"] = (meridianPoint["f5Left"]/avgConductance)*100;
    barChartData["f5Right"] = (meridianPoint["f5Right"]/avgConductance)*100;

    barChartData["f6Left"] = (meridianPoint["f6Left"]/avgConductance)*100;
    barChartData["f6Right"] = (meridianPoint["f6Right"]/avgConductance)*100;
}

//Returns a map that contains the functional status of the internal organs; insufficient, hyperactive and normal
const std::map<std::string, float>& Diagrams::getbodyChartData(){
    return this->bodyChartData;
}

//Returns a map that contains the difference in percentages of the conductance of each meridian point to the average conductuce value of the user
const std::map<std::string, float>& Diagrams::getbarChartData(){
    return this->barChartData;
}
