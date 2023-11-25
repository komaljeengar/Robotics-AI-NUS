String rcvString = "";
float height = 0;
float weight = 0;
float bmi = 0;
float question = 0;
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("What is your your weight?");
}

void loop() {
  float tempValue =0;
  float bmi = 0;
   if(Serial.available()) { //if number of bytes (characters) available for reading
      Serial.print("The weight you ent "); //print I received
      rcvString = Serial.readString();
      tempValue = rcvString.toFloat(); // read string until newline character
      Serial.print(tempValue); }
    
      if(question==0) {
  weight = tempValue;
  Serial.println("");
  Serial.print(weight);

  Serial.println("");
  Serial.print("What is your height?");
  question=1; 
}
else {
  height = tempValue;
  Serial.println("");
  Serial.print("Height you entered is ");

}

if (height > 0 && weight > 0) {
  float bmi = weight / sq(height);
  Serial.println("");
  Serial.print("Your BMI is");
  Serial.print(bmi);
  Serial.print(" & ");
  if (bmi >= 40)
  Serial.println("You are Morbidly Obese");
  else if (bmi >= 35 && bmi < 40)
  Serial.println(" You are Sevearly Obese");
  else if (bmi >= 30 && bmi < 35)
  Serial.println(" You are Moderately Obese");
  else if (bmi >= 25 && bmi < 3)
  Serial.println(" You are Obese");
}

}

   
