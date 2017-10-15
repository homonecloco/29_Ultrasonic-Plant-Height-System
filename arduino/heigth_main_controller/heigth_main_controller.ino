
#define STORE_PRECISION 100.0f
#define SONIC_SENSOR_MAX 1
#define SONIC_BUFFER_SIZE 30
#define SONIC_EMIT_PIN 6
#define SONIC_ECHO_PIN 5
#define DEFAULT_MESSURES 10
#define BUTTON_DELETE 7
#define SPEAKER_PIN 3
#define BUTTON_SELECT 8
#define TILT_SWITCH 2

#define TILT_DELAY 100
#define TILT_REPEAT 10

#define MAX_TOTAL_MOVMENT 200

#define SONIC_OFFSET 2.45

bool refresh;
float height_offset;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sonic_sensor_setup(0, SONIC_EMIT_PIN, SONIC_ECHO_PIN);
  sonic_sensor_set_offset(0);
  button_setup( BUTTON_DELETE);
  button_setup( BUTTON_SELECT);
  //button_setup( TILT_SWITCH);
  speaker_setup();
  lcd_setup();
  gyro_setup();
  refresh=true;
  //lcd_get_baseline_height();
  height_offset=lcd_get_sensor_height(50);
}


bool is_horizontal(){
  for(int i = TILT_REPEAT; i--; i > 0){
    delay(TILT_DELAY);
    if(!gyro_is_still(MAX_TOTAL_MOVMENT)  ){
       return false;
    }
    refresh=true;
    lcd_print_horizontal_count(i* 10); 
  }
  return true;
}

void loop() {
  bool save;
  if(refresh){
    lcd_print_waiting();
    refresh = false;
  }

  if(!is_horizontal()){
    return;
  }
  
  lcd_print_measuring();
  //gyro_set_screen_color();
  float distance =  sonic_sensor_meassure();

  if(distance < 0){
    lcd_print_moved();
    delay(1000);
    refresh = true;
    return;
  }

  distance = distance - (SONIC_OFFSET) - height_offset  ;
  
  save = lcd_print_distance(distance);
  //sonic_sensor_print_reads();
  if(save){
   Serial.print(distance);
   Serial.print(":\n");
   }
  // put your main code here, to run repeatedly:
  
  delay(100);
}
