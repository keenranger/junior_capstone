#include <Stepper.h>
#include <math.h>
const int steps_per_rev = 200; //모터 자체는 1스텝당 1.8도 회전 step/rev
const int delay_const = 200; //1스텝당 딜레이 시간
const float step_index[3] = {5, 0.15, 0.036}; //xy는 1스텝에 5미리, 회전은 1스텝에 0.15도, 틸팅은 1스텝에 0.036도
int i;

int command_parsing(String command, int command_nums[]);
void check_dividable(int stepper_index, float divided_num, int command_nums[]);
////////////////////////////////////////////////////////
//  회전 시킬 떄 xy모터는 5um의 배수                     //
//  회전모터는 0.15도의 배수                            //
//  기울기 모터는 0.036도의 배수로 입력시켜주세요          //
////////////////////////////////////////////////////////

// initialize the stepper
Stepper stepper1(steps_per_rev, 0, 1, 2, 3);//TODO 핀번호 알맞게
Stepper stepper2(steps_per_rev, 4, 5, 6, 7);//TODO 핀번호 알맞게
Stepper stepper3(steps_per_rev, 8, 9, 10, 11);//TODO 핀번호 알맞게
Stepper stepper4(steps_per_rev, 12, 13, 14, 15);//TODO 핀번호 알맞게
Stepper stepper5(steps_per_rev, 16, 17, 18, 19);//TODO 핀번호 알맞게
Stepper steppers[] = {stepper1, stepper2, stepper3, stepper4, stepper5};

void setup() {
  Serial.begin(9600);
}

void loop() {
  int command_nums[2];//숫자로 된 명령어
  int command_code = 0;
  int motor_change[3] = {0, 0, 0};
  String command;
  Serial.println("Please enter command");//명령어를 입력해주세요
  if (Serial.available() > 0) { //시리얼이 연결된다면
    command = Serial.readStringUntil('\n');
    command_code = command_parsing(command, command_nums);

    if (command_code == 1 ) { //모터회전
      steppers[command_nums[0]].step(command_nums[1]);
      if (command_nums[0] < 3)
        motor_change[command_nums[0]] -= command_nums[1];
      else if (command_nums == 4) {
        steppers[command_nums[0] - 2].step(-command_nums[1]); //steppers[2]=4번이랑 역방향으로 도는 모터 회전
      }
    }
    //////////////////////////////////////////////////////////
    else if (command_code == 2) {//원래대로 움직이게 하는 부분
      for (i = 0; i < 3; i++) {
        steppers[i].step(motor_change[i]);
        motor_change[i] = 0;
        delay(2000);
      }
    }
    /////////////////////////////////////////////////////////
    else {//파싱하다가 문제있을때
      Serial.println("ERROR while command parsing");
    }
  }
  //    delay(command_nums[1] * 100); //step 마다 0.1초 대기
  ///////////////////////////////////////////////////////////
  else { //연결 안된다면
    Serial.println("ERROR:Check Serial connect");//연결 상태를 확인해주세요
  }
  delay(1000);
}

/////////////////////////////////////////////////////////////
int command_parsing(String command, int command_nums[]) {
  int index[2] = {command.indexOf('/'), command.length()};

  char command1 = command.substring(0, index[0]).charAt(0);
  command1 = (char) tolower(command1);
  float command2 = command.substring(index[0] + 1, index[1]).toFloat();

  switch (command1) {
    case 'x': //x방향 이동
      check_dividable(0, command2, command_nums);
      if (fmodf(command3, step_index[0]) == 0.0) {
        command_nums[0] = 0;
        command_nums[1] = (int)(negative_flag * command3 / step_index[0]);
        Serial.print("Moving to X-Direction ");
        Serial.print(negative_flag * command3 / step_index[0]);
        Serial.println(" step(s)");
      }
      else {
        Serial.print("ERROR:Number can`t be divided into ");
        Serial.println(step_index[0]);
        return 0;
      }
      break;

    case 'y': //y방향 이동
      if (fmodf(command3, step_index[0]) == 0.0) {
        command_nums[0] = 1;
        command_nums[1] = (int)(negative_flag * command3 / step_index[0]);
        Serial.print("Moving to Y-Direction ");
        Serial.print(negative_flag * command3 / step_index[0]);
        Serial.println(" step(s)");
      }
      else {
        Serial.print("ERROR:Number can`t be divided into ");
        Serial.println(step_index[0]);
        return 0;
      }
      break;

    case 'r': //회전
      if (fmodf(command3, step_index[1]) == 0.0) {
        command_nums[0] = 2;
        command_nums[1] = (int)(negative_flag * command3 / step_index[1]);
        Serial.print("Rotating ");
        Serial.print(negative_flag * command3 / step_index[1]);
        Serial.println(" step(s)");
      }
      else {
        Serial.print("ERROR:Number can`t be divided into ");
        Serial.println(step_index[1]);
        return 0;
      }
      break;

    case 't': //기울이기
      if (fmodf(command3, step_index[2]) == 0.0) {
        command_nums[0] = 3;
        command_nums[1] = (int)(negative_flag * command3 / step_index[2]);
        Serial.print("Tilting ");
        Serial.print(negative_flag * command3 / step_index[2]);
        Serial.println(" step(s)");
      }
      else {
        Serial.print("ERROR:Number can`t be divided into ");
        Serial.println(step_index[2]);
        return 0;
      }
      break;
    case 'z': //엇갈려 회전
      break;
    case 'h':
      return 2;


    default:
      Serial.println("ERROR:Enter X,Y,R,T,Z, OR H");
      return 0;
  }

  return 1;
}

////////////////////////////////////////////////////
void check_dividable(int stepper_index, float divided_num, int command_nums[]) {
  if (fmodf(divided_num, step_index[stepper_index]) == 0.0) {
    command_nums[0] = stepper_index;
    command_nums[1] = (int)(divided_num / step_index[0]);
    switch (stepper_index) {
      case 0:
        Serial.print("Moving to X-Direction ");
        break;
      case 1:
        Serial.print("Moving to Y-Direction ");
        break;
      case 2:
        Serial.print("Rotating ");
        break;
      case 3:
        Serial.print("Tilting ");
        break;
      case 4:
        Serial.print("Self Rotating ");
        break;
    }
    Serial.print(command_nums[1]);
    Serial.println(" step(s)");
  }
  else {
    Serial.print("ERROR:Number can`t be divided into ");
    Serial.println(step_index[stepper_index]);
    return 0;
  }


}
