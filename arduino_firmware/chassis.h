#if !defined(CHASSIS_H)
#define CHASSIS_H


class Motor
{
private:
    int forward;
    int backward;
public:
    Motor(int forward, int backward){
        this->forward = forward;
        this->backward = backward;

        pinMode(forward, OUTPUT);
        pinMode(backward, OUTPUT);
    }
    ~Motor(){

    }

    void drive(float speed){
        speed = min(max(speed, -1), 1);
        if(speed > 0){
            analogWrite(this->forward, speed * 255);
            analogWrite(this->backward, 0);
        } else {
            analogWrite(this->forward, 0);
            analogWrite(this->backward, abs(speed) * 255);
        }
    }

};


class Car
{
private:
    Motor* motor_left = NULL;
    Motor* motor_right = NULL;
public:
    Car(int m1, int m2, int m3, int m4){
        motor_left = new Motor(m1, m2);
        motor_right = new Motor(m3, m4);
    }

    ~Car(){
        motor_left->drive(0);
        motor_right->drive(0);
        delete motor_left;
        delete motor_right;
    }

    void drive(float speed, float turn){
        speed = min(max(speed, -1), 1);
        turn = min(max(turn, -1), 1);

        float left = speed + turn;
        float right = speed - turn;
        left = min(max(left, -1), 1);
        right = min(max(right, -1), 1);

        motor_left->drive(left);
        motor_right->drive(right);

    }
};


#endif // CHASSIS_H
