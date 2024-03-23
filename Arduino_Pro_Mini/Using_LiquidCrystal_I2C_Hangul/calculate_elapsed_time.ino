// Gets the millisecond count since the MCU booted and converts to hours, minutes and seconds into variables declared in the main ino file
void calculate_elapsed_time() {
  currentMillis = millis();        // get the current number of milliseconds
  seconds = currentMillis / 1000;  // convert milliseconds to seconds
  minutes = seconds / 60;          // convert seconds to minutes
  hours = minutes / 60;            // convert minutes to hours
  seconds %= 60;                   // calculate remaining seconds after minutes are removed
  minutes %= 60;                   // calculate remaining minutes after hours are removed
}