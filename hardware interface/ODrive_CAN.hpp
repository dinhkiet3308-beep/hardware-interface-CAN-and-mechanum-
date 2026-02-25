#include <linux/can.h>
#include <cstdint> //for uint8_t

class ODriveCAN {
    private:

        int NODE_ID;
        int socket_ID;

        void CAN_send (int commmand_ID, uint8_t* data, int dlc);

    public:

        ODriveCAN(int node, int socket); //constructor (object setup function)

        void set_velocity(float velocity, float torque = 0);

}