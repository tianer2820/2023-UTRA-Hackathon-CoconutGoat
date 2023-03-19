
// a counter that counts every edge (every state changes)
class DigitalCounter
{
private:
    int pin;
    int count;
    bool last_state;
    bool changed;
public:
    
    DigitalCounter(int pin){
        pinMode(pin, INPUT);
        this->pin = pin;
        count = 0;
        last_state = digitalRead(pin);
        changed = true;
    }

    void update(){
        bool state = digitalRead(this->pin);
        if (state != this->last_state){
            count += 1;
            this->last_state = state;
            changed = true;
        }
    }

    void clear(){
        this->count = 0;
    }

    void clear_changed(){
        this->changed = false;
    }
    bool is_changed(){
        return this->changed;
    }

    int get_count(){
        return this->count;
    }
};
