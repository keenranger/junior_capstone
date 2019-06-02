#include <Stepper.h>
#include <math.h>
const int steps_per_rev = 200; //모터 자체는 1스텝당 1.8도 회전 step/rev
const int delay_const = 200; //1스텝당 딜레이 시간
const double step_index[5] = {5, 5, 90, 0.036, 0.15}; //xy는 1스텝에 5미리, 회전은 1스텝에 0.15도, 틸팅은 1스텝에 0.036도
int steps[2] = {0, 0};

int i;

int command_parsing(String command, int command_nums[]);
int check_dividable(int stepper_index, double divided_num, int command_nums[]);
void rotate(int cw_flag);
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
  stepper1.setspeed(30);
  stepper2.setspeed(30);
  stepper3.setspeed(30);
  stepper4.setspeed(30);
  stepper5.setspeed(30);
}
int flag = 0;
void loop() {
  int command_nums[2];//숫자로 된 명령어
  int command_code = 0;
  String command;
  if (!flag) {
    flag = 1;
    Serial.println("\n////////////////////");
    Serial.println("Please enter command");//명령어를 입력해주세요
    Serial.println("////////////////////");
  }
  if (Serial.available() > 0) { //시리얼이 연결된다면
    flag = 0;
    command = Serial.readStringUntil('\n');
    command_code = command_parsing(command, command_nums);

    if (command_code == 1 ) { //모터회전
      if (command_nums[0] == 2) { //회전후 보정이라면
        rotate(command_nums[1]);
      }
      else {//x,y이동, 틸트, 제자리회전이라면
        steppers[command_nums[0]].step(command_nums[1]);//해당 모터 회전
        if (command_nums[0] < 2) {//x,y는 이동한만큼 기록해야함
          steps[command_nums[0]] += command_nums[1];
        }
        else if (command_nums[0] == 4) {
          steppers[command_nums[0] - 2].step(-command_nums[1]); //steppers[2]=4번이랑 역방향으로 도는 모터 회전
        }
        delay(200 * comman_nums[1]);
      }
      Serial.print("X: ");
      Serial.print(steps[0]);
      Serial.print(", Y: ");
      Serial.println(steps[1]);
    }
    else if (command_code == 2) {//시작위치 보정
      steps[0] = 0;
      steps[1] = 0;
    }

  }
  //////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////
  delay(100);
}

/////////////////////////////////////////////////////////////
int command_parsing(String command, int command_nums[]) {
  int status_flag = 1;

  int index[2] = {command.indexOf('/'), command.length()};

  char command1 = command.substring(0, index[0]).charAt(0);
  command1 = (char) tolower(command1);
  double command2 = command.substring(index[0] + 1, index[1]).toDouble();

  switch (command1) {
    case 'x': //x방향 이동
      status_flag = check_dividable(0, command2, command_nums);
      break;
    case 'y': //y방향 이동
      status_flag = check_dividable(1, command2, command_nums);
      break;
    case 'r': //회전
      status_flag = check_dividable(2, command2, command_nums);
      break;
    case 't': //기울이기
      status_flag = check_dividable(3, command2, command_nums);
      break;
    case 'z': //엇갈려 회전
      status_flag = check_dividable(4, command2, command_nums);
      break;
    case 'i':
      Serial.println("This point is 0,0");
      return 2;

    default:
      Serial.println("ERROR:Enter X,Y,R,T,Z, OR I");
      return 0;
  }
  return status_flag;
}

////////////////////////////////////////////////////
int check_dividable(int stepper_index, double divided_num, int command_nums[]) {
  if (round(divided_num * 1000) % round(step_index[stepper_index] * 1000) == 0) {
    command_nums[0] = stepper_index;
    command_nums[1] = round(divided_num / step_index[stepper_index]);
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
