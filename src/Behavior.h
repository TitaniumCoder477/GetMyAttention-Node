#include <iostream>
using std::string;

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

class Behavior {
    public:
        virtual void loop() = 0;
        void setOn() {
            status = 1;
        };
        void setOff() {
            status = 0;
        };        
        bool isOn() {
            return (status == 1);
        }
    private:
        int status = 0;
};

#endif