#include "Recommendations.h"

////Constructor
Recommendations::Recommendations(std::map<std::string, float> meridianPoint): meridianPoint(meridianPoint){}

//Calculates if the functions of the internals associated with the meridian points are insufficient, hyperactive or irregular
void Recommendations::calculateAbnormities(double max, double min){
    //If one side is over max, and the other side is over min = hyperactively
    //if one side is below min, and the other side is below max  = Insufficiency
    //If one side is above max and the other side is below min = Irregular

    if(meridianPoint["h1Left"] < min){
        if(meridianPoint["h1Right"] < min || (meridianPoint["h1Right"] > min && meridianPoint["h1Right"] < max)){
            recommend.push_back("The lung is working insufficiently, if abnormities persist over time, we recommend that you see a lung specialist\n");
        }else{
            recommend.push_back("The lung is working irregularly, if abnormities persist over time, we recommend that you see a lung specialist\n");
        }
    }else if(meridianPoint["h1Left"] > max){
        if(meridianPoint["h1Right"] > max || (meridianPoint["h1Right"] > min && meridianPoint["h1Right"] < max)){
            recommend.push_back("The lung is working hyperactively, if abnormities persist over time, we recommend that you see a lung specialist\n");
        }else{
            recommend.push_back("The lung is working irregularly, if abnormities persist over time, we recommend that you see a lung specialist\n");
        }
    }else if(meridianPoint["h1Right"] < min){
        recommend.push_back("The lung is working insufficiently, if abnormities persist over time, we recommend that you see a lung specialist\n");
    }else if(meridianPoint["h1Right"] > max){
        recommend.push_back("The lung is working hyperactively, if abnormities persist over time, we recommend that you see a lung specialist\n");
    }


    if(meridianPoint["h2Left"] < min){
        if(meridianPoint["h2Right"] < min || (meridianPoint["h2Right"] > min && meridianPoint["h2Right"] < max)){
            recommend.push_back("The pericardium is working insufficiently, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
        }else{
            recommend.push_back("The pericardium is working irregularly, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
        }
    }else if(meridianPoint["h2Left"] > max){
        if(meridianPoint["h2Right"] > max || (meridianPoint["h2Right"] > min && meridianPoint["h2Right"] < max)){
            recommend.push_back("The pericardium is working hyperactively, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
        }else{
            recommend.push_back("The pericardium is working irregularly, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
        }
    }else if(meridianPoint["h2Right"] < min){
        recommend.push_back("The pericardium is working insufficiently, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
    }else if(meridianPoint["h2Right"] > max){
        recommend.push_back("The pericardium is working hyperactively, if abnormities persist over time, we recommend that you see a pericardium specialist\n");
    }


    if(meridianPoint["h3Left"] < min){
        if(meridianPoint["h3Right"] < min || (meridianPoint["h3Right"] > min && meridianPoint["h3Right"] < max)){
            recommend.push_back("The heart is working insufficiently, if abnormities persist over time, we recommend that you see a heart specialist\n");
        }else{
            recommend.push_back("The heart is working irregularly, if abnormities persist over time, we recommend that you see a heart specialist\n");
        }
    }else if(meridianPoint["h3Left"] > max){
        if(meridianPoint["h3Right"] > max || (meridianPoint["h3Right"] > min && meridianPoint["h3Right"] < max)){
            recommend.push_back("The heart is working hyperactively, if abnormities persist over time, we recommend that you see a heart specialist\n");
        }else{
            recommend.push_back("The heart is working irregularly, if abnormities persist over time, we recommend that you see a heart specialist\n");
        }
    }else if(meridianPoint["h3Right"] < min){
        recommend.push_back("The heart is working insufficiently, if abnormities persist over time, we recommend that you see a heart specialist\n");
    }else if(meridianPoint["h3Right"] > max){
        recommend.push_back("The heart is working hyperactively, if abnormities persist over time, we recommend that you see a heart specialist\n");
    }


    if(meridianPoint["h4Left"] < min){
        if(meridianPoint["h4Right"] < min || (meridianPoint["h4Right"] > min && meridianPoint["h4Right"] < max)){
            recommend.push_back("The small intestine is working insufficiently, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
        }else{
            recommend.push_back("The small intestine is working irregularly, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
        }
    }else if(meridianPoint["h4Left"] > max){

        if(meridianPoint["h4Right"] > max || (meridianPoint["h4Right"] > min && meridianPoint["h4Right"] < max)){
            recommend.push_back("The small intestine is working hyperactively, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
        }else{
            recommend.push_back("The small intestine is working irregularly, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
        }
    }else if(meridianPoint["h4Right"] < min){
        recommend.push_back("The small intestine is working insufficiently, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
    }else if(meridianPoint["h4Right"] > max){
        recommend.push_back("The small intestine is working hyperactively, if abnormities persist over time, we recommend that you see a small intestine specialist\n");
    }


    if(meridianPoint["h5Left"] < min){
        if(meridianPoint["h5Right"] < min || (meridianPoint["h5Right"] > min && meridianPoint["h5Right"] < max)){
            recommend.push_back("The lymph vessel is working insufficiently, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
        }else{
            recommend.push_back("The lymph vessel is working irregularly, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
        }
    }else if(meridianPoint["h5Left"] > max){

        if(meridianPoint["h5Right"] > max || (meridianPoint["h5Right"] > min && meridianPoint["h5Right"] < max)){
            recommend.push_back("The lymph vessel is working hyperactively, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
        }else{
            recommend.push_back("The lymph vessel is working irregularly, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
        }
    }else if(meridianPoint["h5Right"] < min){
        recommend.push_back("The lymph vessel is working insufficiently, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
    }else if(meridianPoint["h5Right"] > max){
        recommend.push_back("The lymph vessel is working hyperactively, if abnormities persist over time, we recommend that you see a lymph vessel specialist\n");
    }


    if(meridianPoint["h6Left"] < min){
        if(meridianPoint["h6Right"] < min || (meridianPoint["h6Right"] > min && meridianPoint["h6Right"] < max)){
            recommend.push_back("The large intestine is working insufficiently, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
        }else{
            recommend.push_back("The large intestine is working irregularly, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
        }
    }else if(meridianPoint["h6Left"] > max){
        if(meridianPoint["h6Right"] > max || (meridianPoint["h6Right"] > min && meridianPoint["h6Right"] < max)){
            recommend.push_back("The large intestine is working hyperactively, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
        }else{
            recommend.push_back("The large intestine is working irregularly, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
        }
    }else if(meridianPoint["h6Right"] < min){
        recommend.push_back("The large intestine is working insufficiently, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
    }else if(meridianPoint["h6Right"] > max){
        recommend.push_back("The large intestine is working hyperactively, if abnormities persist over time, we recommend that you see a large intestine specialist\n");
    }


    if(meridianPoint["f1Left"] < min){
        if(meridianPoint["f1Right"] < min || (meridianPoint["f1Right"] > min && meridianPoint["f1Right"] < max)){
            recommend.push_back("The pancreas is working insufficiently, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
        }else{
            recommend.push_back("The pancreas is working irregularly, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
        }
    }else if(meridianPoint["f1Left"] > max){
        if(meridianPoint["f1Right"] > max || (meridianPoint["f1Right"] > min && meridianPoint["f1Right"] < max)){
            recommend.push_back("The pancreas is working hyperactively, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
        }else{
            recommend.push_back("The pancreas is working irregularly, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
        }
    }else if(meridianPoint["f1Right"] < min){
        recommend.push_back("The pancreas is working insufficiently, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
    }else if(meridianPoint["f1Right"] > max){
        recommend.push_back("The pancreas is working hyperactively, if abnormities persist over time, we recommend that you see a pancreas specialist\n");
    }


    if(meridianPoint["f2Left"] < min){
        if(meridianPoint["f2Right"] < min || (meridianPoint["f2Right"] > min && meridianPoint["f2Right"] < max)){
            recommend.push_back("The liver is working insufficiently, if abnormities persist over time, we recommend that you see a liver specialist\n");
        }else{
            recommend.push_back("The liver is working irregularly, if abnormities persist over time, we recommend that you see a liver specialist\n");
        }
    }else if(meridianPoint["f2Left"] > max){
        if(meridianPoint["f2Right"] > max || (meridianPoint["f2Right"] > min && meridianPoint["f2Right"] < max)){
            recommend.push_back("The liver is working hyperactively, if abnormities persist over time, we recommend that you see a liver specialist\n");
        }else{
            recommend.push_back("The liver is working irregularly, if abnormities persist over time, we recommend that you see a liver specialist\n");
        }
    }else if(meridianPoint["f2Right"] < min){
        recommend.push_back("The liver is working insufficiently, if abnormities persist over time, we recommend that you see a liver specialist\n");
    }else if(meridianPoint["f2Right"] > max){
        recommend.push_back("The liver is working hyperactively, if abnormities persist over time, we recommend that you see a liver specialist\n");
    }


    if(meridianPoint["f3Left"] < min){
        if(meridianPoint["f3Right"] < min || (meridianPoint["f3Right"] > min && meridianPoint["f3Right"] < max)){
            recommend.push_back("The kidney is working insufficiently, if abnormities persist over time, we recommend that you see a kidney specialist\n");
        }else{
            recommend.push_back("The kidney is working irregularly, if abnormities persist over time, we recommend that you see a kidney specialist\n");
        }
    }else if(meridianPoint["f3Left"] > max){
        if(meridianPoint["f3Right"] > max || (meridianPoint["f3Right"] > min && meridianPoint["f3Right"] < max)){
            recommend.push_back("The kidney is working hyperactively, if abnormities persist over time, we recommend that you see a kidney specialist\n");
        }else{
            recommend.push_back("The kidney is working irregularly, if abnormities persist over time, we recommend that you see a kidney specialist\n");
        }
    }else if(meridianPoint["f3Right"] < min){
        recommend.push_back("The kidney is working insufficiently, if abnormities persist over time, we recommend that you see a kidney specialist\n");
    }else if(meridianPoint["f3Right"] > max){
        recommend.push_back("The kidney is working hyperactively, if abnormities persist over time, we recommend that you see a kidney specialist\n");
    }


    if(meridianPoint["f4Left"] < min){
        if(meridianPoint["f4Right"] < min || (meridianPoint["f4Right"] > min && meridianPoint["f4Right"] < max)){
            recommend.push_back("The bladder is working insufficiently, if abnormities persist over time, we recommend that you see a bladder specialist\n");
        }else{
            recommend.push_back("The bladder is working irregularly, if abnormities persist over time, we recommend that you see a bladder specialist\n");
        }
    }else if(meridianPoint["f4Left"] > max){
        if(meridianPoint["f4Right"] > max || (meridianPoint["f4Right"] > min && meridianPoint["f4Right"] < max)){
            recommend.push_back("The bladder is working hyperactively, if abnormities persist over time, we recommend that you see a bladder specialist\n");
        }else{
            recommend.push_back("The bladder is working irregularly, if abnormities persist over time, we recommend that you see a bladder specialist\n");
        }
    }else if(meridianPoint["f4Right"] < min){
        recommend.push_back("The bladder is working insufficiently, if abnormities persist over time, we recommend that you see a bladder specialist\n");
    }else if(meridianPoint["f4Right"] > max){
        recommend.push_back("The bladder is working hyperactively, if abnormities persist over time, we recommend that you see a bladder specialist\n");
    }


    if(meridianPoint["f5Left"] < min){
        if(meridianPoint["f5Right"] < min || (meridianPoint["f5Right"] > min && meridianPoint["f5Right"] < max)){
            recommend.push_back("The gall bladder is working insufficiently, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
        }else{
            recommend.push_back("The gall bladder is working irregularly, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
        }
    }else if(meridianPoint["f5Left"] > max){
        if(meridianPoint["f5Right"] > max || (meridianPoint["f5Right"] > min && meridianPoint["f5Right"] < max)){
            recommend.push_back("The gall bladder is working hyperactively, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
        }else{
            recommend.push_back("The gall bladder is working irregularly, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
        }
    }else if(meridianPoint["f5Right"] < min){
        recommend.push_back("The gall bladder is working insufficiently, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
    }else if(meridianPoint["f5Right"] > max){
        recommend.push_back("The gall bladder is working hyperactively, if abnormities persist over time, we recommend that you see a gall bladder specialist\n");
    }

    if(meridianPoint["f6Left"] < min){

        if(meridianPoint["f6Right"] < min || (meridianPoint["f6Right"] > min && meridianPoint["f6Right"] < max)){
            recommend.push_back("The stomach is working insufficiently, if abnormities persist over time, we recommend that you see a stomach specialist\n");
        }else{
            recommend.push_back("The stomach is working irregularly, if abnormities persist over time, we recommend that you see a stomach specialist\n");
        }
    }else if(meridianPoint["f6Left"] > max){

        if(meridianPoint["f6Right"] > max || (meridianPoint["f6Right"] > min && meridianPoint["f6Right"] < max)){
            recommend.push_back("The stomach is working hyperactively, if abnormities persist over time, we recommend that you see a stomach specialist\n");
        }else{
            recommend.push_back("The stomach is working irregularly, if abnormities persist over time, we recommend that you see a stomach specialist\n");
        }
    }else if(meridianPoint["f6Right"] < min){

        recommend.push_back("The stomach is working insufficiently, if abnormities persist over time, we recommend that you see a stomach specialist\n");
    }else if(meridianPoint["f6Right"] > max){

        recommend.push_back("The stomach is working hyperactively, if abnormities persist over time, we recommend that you see a stomach specialist\n");
    }
}

//Returns the vector of strings that contains all of the recommendations
std::vector<std::string> Recommendations::getRecommendations(){
    return this->recommend;
}
