int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int16_t AcX_2,AcY_2,AcZ_2,Tmp_2,GyX_2,GyY_2,GyZ_2;
int buzzer;
int16_t DiffGyX;
int16_t DiffGyY;

void setup() {

 AcX=0;
 AcY=0;
 AcZ=0;
 GyX=0;
 GyY=0;
 GyZ=0;

 GyX_2=0;
 GyY_2=0;
 GyZ_2=0;
 buzzer =3;
}
//void beeper_frequency(DiffGxY,DiffGxX);


void loop() {
  
  
  GyX_2=GyX_2+5;
  GyY_2=GyY_2+5;
  GyZ_2=GyZ_2+5;
 
  DiffGyX=GyX_2-GyX;
  DiffGyY=GyY_2-GyY;
  
  delay(2000);
  beeper_frequency(DiffGyY,DiffGyX,buzzer);
}

void beeper_frequency(int16_t DiffGxY, int16_t DiffGxX, int buzzer){
  
  if((DiffGyY>=10&& DiffGyY<=25) && (DiffGyX>=10||DiffGyX<=25)){
    analogWrite(buzzer,255);
  }
  else if((DiffGyY>=26&& DiffGyY<=35) && (DiffGyX>=26||DiffGyX<=35)){
    analogWrite(buzzer,50);
  }
}








  
