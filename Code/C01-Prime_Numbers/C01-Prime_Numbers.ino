void setup() {
   Serial.begin(115200); 
   delay(2000);

   Serial.println("The first 30 prime numbers are:");

   int count = 0; 
   int num = 2; 

   while (count < 30) {
      bool isPrime = true; 

      
      for (int i = 2; i <= sqrt(num); i++) {
         if (num % i == 0) {
            isPrime = false;
            break;
         }
      }

      if (isPrime) {
         Serial.println(num);
         count++;
      }

      num++; 
   }

  
}

void loop() {
   
}
