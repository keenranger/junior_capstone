void rotate(int cw_flag) {
  int temp = 0;
  if (cw_flag == 1) {
    steppers[0].step(-steps[1] - steps[0]);
    delay(100 * abs(-steps[1] - steps[0]));
    steppers[1].step(steps[0] - steps[1]);
    delay(100 * abs(steps[0] - steps[1]));
    temp = steps[0];
    steps[0] = -steps[1];
    steps[1] = temp;
  }
  else if (cw_flag == -1) {
    steppers[0].step(steps[1] - steps[0]);
    delay(100 * abs(-steps[1] - steps[0]));
    steppers[1].step(-steps[0] - steps[1]);
    delay(100 * abs(steps[0] - steps[1]));
    temp = steps[0];
    steps[0] = steps[1];
    steps[1] = -temp;
  }
  else {
    Serial.println("FAILED : Enter 90 or -90");
  }
}
