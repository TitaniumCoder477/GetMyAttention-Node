#include "ServiceStates.h"

class Service : public ServiceStates {
    private:
        string state;
    public:
        Service() {
            off();
        }
    
        void on() {
            state = getOnState();
        };

        void off() {
            state = getOffState();
        };

        string getState() {
            return state;
        };
};