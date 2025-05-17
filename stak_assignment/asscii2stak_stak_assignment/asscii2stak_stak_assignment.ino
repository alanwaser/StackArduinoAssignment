#define yellowLedPin 9 //defines yellow led
#define redLedPin 5 //defines red led
#define potento A0 // defines potentiometer
#define transmitter A1 // infrared transmitter
#define reciever 2 // infrared reciever

const char* stakCode[] = {
  "@", "!!@", "!@^", "@!", "!", "@@", "!@@", "!@!", "!!", "@!!", "@^", "!!^", "!@", "^@", "^!",
  "!^", "@@^", "!^@", "!^!", "^", "^^", "@@@", "@@!", "@!^", "!^^", "@^!", "^!!@", "^!!^",
  "^!@!", "^!@@", "^!@@", "^!@^", "^!^!", "^!^@", "^!^^", "^@!!", "^!!!", "^!!", "^!@", "^@!",
  "^@@", "^@^", "^^!", "^^@", "^^^", "^!^", "^@@@^"
};
const char characters[] = "abcdefghijklmnopqrstuvwxyz1234567890.,'?!+-*/=";//lookup table with ascii and stak

String ascii2stak(String s) {//translates ascii to stak 
  String stakString = ""; // main string where the users input is translated into
  for (int i = 0; i < s.length(); i++) { 
    char c = tolower(s[i]);// goes through each charecter that the user has inputed
    if (c == ' ') { 
      stakString += " "; // if user inputs a space a space is added to the main string
    } else {
      bool found = false; // boolean value to help with error value 
      for (int j = 0; j < sizeof(characters) / sizeof(characters[0]); j++) {
        if (characters[j] == c) { //cycles through each value in charecters to match to the userinput
          stakString += stakCode[j];
          if (i < s.length() - 1 && s[i + 1] != ' ') {
            stakString += "="; //ev
          }
          found = true; 
          break;
        }
      }
      if (!found) { // error message, if boolean remains false then the ? is outputted
        stakString += "?"; // Add error indicator for unknown character
      }
    }
  }
  return stakString; // return the stakString translation back to the funtion to be outputted
}

String stak2ascii(String m) { //function for translation from stak to ascii
  String asciiString = "";
  String stakChar = "";
  for (int i = 0; i < m.length(); i++) { // this goes through each charecter in the inputed stak code
    if (m[i] == ' ' || m[i] == '=') { // checks to see if a space of a word sperator has been inputed
      if (stakChar.length() > 0) { // checks to see if the length of stakChar is above 0
        bool found = false; // boolean useful in error messages see below
        for (int j = 0; j < sizeof(stakCode) / sizeof(stakCode[0]); j++) {
          if (stakChar == stakCode[j]) { // checks to see which value in stakCode array matches the userInput
            asciiString += characters[j]; // corresponds to other array to find which asciii charecter it matches to
            found = true; // stops error message being relayed
            break; // stops for loop
          }
        }
        if (!found) { // if found is false
          asciiString += "?"; // prints error message
        }
        stakChar = ""; // clears stakChar
      }
      if (m[i] == ' ') {
        asciiString += " "; // adds space when space entered
      }
    } else {
      stakChar += m[i]; // adds to overall stakChar
    }
  }
  if (stakChar.length() > 0) {
    bool found = false;
    for (int j = 0; j < sizeof(stakCode) / sizeof(stakCode[0]); j++) {
      if (stakChar == stakCode[j]) {
        asciiString += characters[j]; // if the stakChar isnt " " or "="then this happens
        found = true;
        break;
      }
    }
    if (!found) {
      asciiString += "?";// error message
    }
  }
  return asciiString;// return the finished overall string back to function to be outpitted
}

void sendDigital(int pin, String stak) {
  int potNum = analogRead(potento); // read potentiometer 
  int mapToTime = map(potNum, 0, 1023, 15, 300); // maps potentiometer so that the range is 15 - 300 instead of 0 = 1023
  for (int i = 0; i < stak.length(); i++) { // seperates charecters into char
    if (stak[i] == '!') { 
      digitalWrite(pin, HIGH);
      delay(mapToTime); // turns on for maptoTime
      digitalWrite(pin, LOW);
      delay(mapToTime); // turns off for maptoTime
    } else if (stak[i] == '@') {
      digitalWrite(pin, HIGH);
      delay(2 * mapToTime); 
      digitalWrite(pin, LOW);
      delay(mapToTime); 
    } else if (stak[i] == '^') {
      digitalWrite(pin, HIGH);
      delay(4 * mapToTime); 
      digitalWrite(pin, LOW);
      delay(mapToTime); 
    } else if (stak[i] == ' ') { // word sepertor
      delay(6 * mapToTime);
    } else if (stak[i] == '=') { // letter seperator
      delay(3 * mapToTime);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Input: "); // output
  pinMode(yellowLedPin, OUTPUT); 
  pinMode(redLedPin, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    String userInput = Serial.readStringUntil('\n'); // reads input string until a new line 
    if (userInput == "\n" || userInput == "\r") {
      return; // checks if userInput is equal to a new line or a carraige input
    }
    for (int i = 0; i < userInput.length(); i++) {
      userInput[i] = tolower(userInput[i]);// lowers each char in userInput
    }
    if (userInput == "^=!!=^!!@" || userInput =="^=!!=^!!^"||userInput =="^=!!=^!@!"||userInput =="^=!!=^!@@"||userInput =="^=!!=^!@@"||userInput =="^=!!=^!@^"||userInput =="^=!!=^!^!"||userInput =="^=!!=^!^@"||userInput =="^=!!=^!^^"|| userInput =="^=!!=^@!!"){
      String stakNum = userInput.substring(4); // checks for stack Ti decimal numbers and removes the ti useing substring
      String stringNum = stak2ascii(stakNum); // translates number to ascii
      Serial.println(stak2ascii(userInput));
      sendDigital(redLedPin,userInput); // outputs userInput on red led
      int num = stringNum.toInt(); // converts stringNum to an int 
      for (int i=0; i < num; i++){ //repeats for  number specified in num
        tone(A1,38000); // pulses at 38khz
        delay(10); // pulses for 10 ms
        noTone(A1);
        delay(10);// off for 10ms
        Serial.println("Detected"); // prints detected when this cycle happens
      }
    }else if (userInput == "!^=!^!=@@@") {
      int pot = analogRead(potento); // read potentiometer
      String intStakPot = String(pot);// puts the int pot into string variable inStakPot
      String complete = intStakPot;
      String asciiTranslation = stak2ascii(userInput);//converts to ascii
      Serial.println(asciiTranslation); // prints ascii translation
      sendDigital(redLedPin, userInput); // outputs the code on red pin
      if (pot < 10) {
        complete = ( "000" + intStakPot); // allows code to be 4 digits
        String completeAscii = (ascii2stak(complete));
        Serial.println(completeAscii);
        sendDigital(yellowLedPin, completeAscii);// transmit to yellow led
      } else if (pot < 100) {
        complete = ( "00" + intStakPot);// allows code to be 4 digits
        String completeAscii = (ascii2stak(complete));
        Serial.println(completeAscii);
        sendDigital(yellowLedPin, completeAscii); // transmit to yellow led
      } else if (pot < 1000) {
        complete = ( "0" + intStakPot); // allows code to be 4 digits
        String completeAscii = (ascii2stak(complete));
        Serial.println(completeAscii);
        sendDigital(yellowLedPin, completeAscii);// transmit to yellow led
      } else {
        String completeAscii = (ascii2stak(complete));
        Serial.println(completeAscii);
        sendDigital(yellowLedPin, completeAscii);// transmit to yellow led
      }
    }else if (userInput.length() > 0 && (userInput[0] == '!' || userInput[0] == '@' || userInput[0] == '^')) {
      String asciiTranslation = stak2ascii(userInput);//checks if stak was inputed and changes it from stak to asccii
      Serial.println(asciiTranslation); //prints translated ascii
      sendDigital(redLedPin, userInput);// outputs this on red LED
    }
    else {
      String stakTranslation = ascii2stak(userInput);//if stak wasnt inputed then asccii translated to stak
      Serial.println(stakTranslation);//prints translated stak
      sendDigital(yellowLedPin, stakTranslation);//outputs stak code that the user has entered
    }
  }
}