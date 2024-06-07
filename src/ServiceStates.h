#include <iostream>
using std::string;

#ifndef SERVICESTATES_H
#define SERVICESTATES_H

class ServiceStates {
    protected:
        virtual string getOnState() {
            return "On";
        }
        virtual string getOffState() {
            return "Off";
        }   
};

#endif