#include <Stepper.h>
#include <math.h>
const int steps_per_rev = 200; //모터 자체는 1스텝당 1.8도 회전 step/rev
const int delay_const = 30; //1스텝당 딜레이 시간
const double step_index[5] = {5, 5, 90, 0.036, 0.15}; //xy는 1스텝에 5미리, 회전은 1스텝에 0.15도, 틸팅은 1스텝에 0.036도
int steps[2] = {0, 0};

int i;

int command_parsing(String command);
int check_dividable(int stepper_index, double divided_num);
void rotate(int cw_flag);
////////////////////////////////////////////////////////
//  회전 시킬 떄 xy모터는 5um의 배수                     //
//  회전모터는 0.15도의 배수                            //
//  기울기 모터는 0.036도의 배수로 입력시켜주세요          //
////////////////////////////////////////////////////////

// initialize the stepper
Stepper stepper1(steps_per_rev, 0, 1, 2, 3);//TODO 핀번호 알맞게//x모터
Stepper stepper2(steps_per_rev, 4, 5, 6, 7);//TODO 핀번호 알맞게//y모터
Stepper stepper3(steps_per_rev, 8, 9, 10, 11);//TODO 핀번호 알맞게//90도회전
Stepper stepper4(steps_per_rev, 12, 13, 14, 15);//TODO 핀번호 알맞게//틸팅모터
Stepper stepper5(steps_per_rev, 16, 17, 18, 19);//TODO 핀번호 알맞게//상호회전

void setup() {
  Serial.begin(9600);
  stepper1.setSpeed(10);
  stepper2.setSpeed(10);
  stepper3.setSpeed(10);
  stepper4.setSpeed(10);
  stepper5.setSpeed(10);
}
int flag = 0;
void loop() {
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
    command_code = command_parsing(command);

    if (command_code == 1 ) { //모터회전
      Serial.print("X: ");
      Serial.print(steps[0]);
      Serial.print(", Y: ");
      Serial.println(steps[1]);
    }
  }
  ///////////////////////////////////////////////////////////
  delay(100);
}

/////////////////////////////////////////////////////////////
int command_parsing(String command) {
  int status_flag = 1;

  int index[2] = {command.indexOf('/'), command.length()};

  char command1 = command.substring(0, index[0]).charAt(0);
  command1 = (char) tolower(command1);
  double command2 = command.substring(index[0] + 1, index[1]).toDouble();

  switch (command1) {
    case 'x': //x방향 이동
      status_flag = check_dividable(0, command2);
      break;
    case 'y': //y방향 이동
      status_flag = check_dividable(1, command2);
      break;
    case 'r': //회전
      status_flag = check_dividable(2, command2);
      break;
    case 't': //기울이기
      status_flag = check_dividable(3, command2);
      break;
    case 'z': //엇갈려 회전
      status_flag = check_dividable(4, command2);
      break;
    case 'i':
      steps[0] = 0;
      steps[1] = 0;
      Serial.println("This point is 0,0");
      return 0;

    default:
      Serial.println("ERROR:Enter X,Y,R,T,Z, OR I");
      return 0;
  }
  return status_flag;
}

////////////////////////////////////////////////////
