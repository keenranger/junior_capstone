int check_dividable(int stepper_index, double divided_num) {
  if (round(divided_num * 1000) % round(step_index[stepper_index] * 1000) == 0) {
    int steps_temp = round(divided_num / step_index[stepper_index]);
    switch (stepper_index) {
      case 0:
        Serial.print("Moving to X-Direction ");
        Serial.print(steps_temp);
        Serial.println(" step(s)");
        stepper1.step(steps_temp);
        steps[0] += steps_temp;
        break;
      case 1:
        Serial.print("Moving to Y-Direction ");
        Serial.print(steps_temp);
        Serial.println(" step(s)");
        stepper2.step(steps_temp);
        steps[1] += steps_temp;
        break;
      case 2:
        Serial.print("Rotating ");
        Serial.print(steps_temp * 90);
        Serial.println(" degree(s)");
        stepper3.step(steps_temp);
        rotate(steps_temp);
        break;
      case 3:
        Serial.print("Tilting ");
        Serial.print(steps_temp);
        Serial.println(" step(s)");
        stepper4.step(steps_temp);
        break;
      case 4:
        Serial.print("Self Rotating ");
        Serial.print(steps_temp);
        Serial.println(" step(s)");
        stepper3.step(steps_temp);
        stepper5.step(-steps_temp);
        break;
    }
    return 1;
  }
  else {
    Serial.print("ERROR:");
    Serial.print(divided_num);
    Serial.print(" can`t be divided into ");
    Serial.println(step_index[stepper_index]);
    return 0;
  }
}
