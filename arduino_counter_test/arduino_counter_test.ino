

// a counter that counts every edge (every state changes)
class DigitalCounter
{
private:
    int pin;
    int count;
    bool last_state;
public:
    
    DigitalCounter(int pin){
        pinMode(pin, INPUT);
        this->pin = pin;
        count = 0;
        last_state = digitalRead(pin);
    }

    void update(){
        bool state = digitalRead(this->pin);
        if (state != this->last_state){
            count += 1;
            this->last_state = state;
        }
    }

    void clear(){
        this->count = 0;
    }

    int get_count(){
        return this->count;
    }
};




void setup(){
    Serial.begin(9600);
}
DigitalCounter counter(12);

void loop(){
    counter.update();

    Serial.println(counter.get_count());
    delay(5);
}