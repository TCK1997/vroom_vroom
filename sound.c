#define nosound 200
#define diff 100
#define input_300 A2
#define input_3000 A3
int sound_300 = 0, sound_3000 = 0;
void sound() {
    get_sound();
    if((sound_300 + sound_3000) < nosound){
        //continue colour challenge
    } else if (abs(sound_300 - sound_3000) < nosound){
        //continue u-turn
    } else if (sound_300 - sound_3000 > diff && sound_300 > 300){
        //continue left_turn
    } else if (sound_3000 - sound_300 > diff && sound _3000 > 200){
        //continue right_turn
    }
}
void get_sound(){
    sound_300 = analogRead(input_300);
    sound_3000 = analogRead(input_3000);
}