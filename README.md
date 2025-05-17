Overview
This project implements a translator between ASCII and a custom encoding called "stak" on an Arduino platform. The system not only translates between these two encoding formats but also includes special commands for hardware interaction, including potentiometer reading and infrared transmission.
Features

Bidirectional Translation:

ASCII to stak conversion
stak to ASCII conversion


Visual Output:

Yellow LED displays ASCII to stak translations
Red LED displays stak to ASCII translations


Special Commands:

PSV (!^=!^!=@@@ in stak): Reads and displays the current potentiometer value
TI followed by a number: Activates the infrared transmitter the specified number of times


Error Handling: Displays "?" for unrecognized characters
Adaptive Timing: Uses potentiometer to control signal timing (15ms-300ms)

Hardware Requirements

Arduino board
Components connected to the following pins:

Yellow LED: Pin 9
Red LED: Pin 5
Potentiometer: Analog pin A0
Infrared transmitter: Analog pin A1
Infrared receiver: Digital pin 2



How It Works
Translation System
The project uses lookup tables to efficiently translate between ASCII characters and their corresponding stak codes:

ASCII characters include: a-z, 0-9, and special characters (.,'?!+-*/=)
stak uses combinations of three symbols: "!", "@", and "^"
"=" is used as a letter separator in stak
Space is used as a word separator in both formats

LED Signaling
When transmitting stak codes via LEDs:

"!": Short pulse (controlled by potentiometer)
"@": Medium pulse (2x the base duration)
"^": Long pulse (4x the base duration)
"=": Letter separator pause (3x the base duration)
" ": Word separator pause (6x the base duration)

Special Commands

PSV Command (!^=!^!=@@@ in stak):

Reads the current value from the potentiometer (0-1023)
Formats the value as a 4-digit number with leading zeros if needed
Translates this value to stak and displays it on the yellow LED


TI Command (followed by a digit 0-9):

Activates the infrared transmitter at 38kHz
Pulses the transmitter the specified number of times
Displays "Detected" message for each pulse



Usage

Connect your Arduino with the required components
Upload the ASCII2STAK code
Open the Serial Monitor at 9600 baud
Enter ASCII text to see its stak translation (displayed on yellow LED)
Enter stak code (using !, @, ^) to see its ASCII translation (displayed on red LED)
Enter special commands to interact with hardware:

!^=!^!=@@@ (PSV in stak) to read the potentiometer
^=!!=^!!@ followed by a digit (TI3 in stak) to pulse the infrared transmitter



Implementation Details

Uses defensive programming techniques with boolean flags for error handling
Implements efficient translation using array lookups
Leverages the tolower() function to make input case-insensitive
Uses Serial.readStringUntil('\n') to handle input properly

License
This project was created as an academic assignment.
