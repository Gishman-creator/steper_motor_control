#include <Stepper.h>

// --- Configuration Constants ---

// For a 28BYJ-48 motor, the steps per revolution is typically 2048 in full-step mode.
const int stepsPerRevolution = 2048;

// Motor Pin definitions (adjust these to match your ULN2003 driver connections)
const int in1 = 8;
const int in2 = 9;
const int in3 = 10;
const int in4 = 11;

// Initialize the Stepper library with the number of steps and the pins
Stepper myStepper(stepsPerRevolution, in1, in3, in2, in4); 
// Note: The order for the 28BYJ-48/ULN2003 driver is often 
// (Pin1, Pin3, Pin2, Pin4) or (Pin1, Pin2, Pin3, Pin4) depending on wiring. 
// I'm using (in1, in3, in2, in4) which often works for ULN2003.

// --- Global State Variables ---

// Variable to track the current absolute position of the motor, starting at 0 degrees.
float currentPositionDegrees = 0.0;

// The speed at which the motor will rotate (steps per second)
const int motorSpeedRPM = 10; // 10 revolutions per minute

// --- Utility Function to Convert Degrees to Steps ---

/**
 * @brief Converts a desired rotation in degrees to the required number of steps.
 * * @param degrees The angle of rotation (positive or negative).
 * @return long The calculated number of steps (positive or negative).
 */
long degreesToSteps(float degrees) {
    // Calculation: steps = degrees * (stepsPerRevolution / 360)
    return (long)(degrees * ((float)stepsPerRevolution / 360.0));
}

// --- Setup Function ---

void setup() {
    // Initialize serial communication at 9600 bits per second:
    Serial.begin(9600);
    
    // Set the motor speed
    myStepper.setSpeed(motorSpeedRPM);
    
    Serial.println("--- Stepper Motor Serial Controller ---");
    Serial.println("Motor initialized. Current position: 0.0 degrees.");
    Serial.println("Enter commands in the format: rotate <degrees>");
    Serial.println("Example: rotate 180");
    Serial.println("Example: rotate -90.5");
}

// --- Main Loop ---

void loop() {
    // Check if data has been sent from the computer (Serial Monitor)
    if (Serial.available() > 0) {
        // Read the incoming string until a newline character is received
        String input = Serial.readStringUntil('\n');
        input.trim(); // Remove any leading/trailing whitespace

        // Find the index of the space separating the command and the value
        int spaceIndex = input.indexOf(' ');

        if (spaceIndex != -1) {
            String command = input.substring(0, spaceIndex);
            String valueStr = input.substring(spaceIndex + 1);

            // 1. Check if the command is "rotate"
            if (command.equalsIgnoreCase("rotate")) {
                // 2. Try to convert the remaining part of the string to a float (degrees)
                float degrees = valueStr.toFloat();

                if (degrees != 0.0 || valueStr == "0" || valueStr == "0.0") {
                    // 3. Convert the requested degrees into steps
                    long stepsToMove = degreesToSteps(degrees);

                    Serial.print("Received command: Rotate ");
                    Serial.print(degrees);
                    Serial.print(" degrees. Steps calculated: ");
                    Serial.println(stepsToMove);

                    // 4. Drive the motor!
                    myStepper.step(stepsToMove);

                    // 5. Update the current position
                    currentPositionDegrees += degrees;
                    
                    // Normalize the position to stay within 0.0 to 360.0 degrees for cleaner display
                    // The fmod() function calculates the remainder of division
                    currentPositionDegrees = fmod(currentPositionDegrees, 360.0); 
                    if (currentPositionDegrees < 0.0) {
                        currentPositionDegrees += 360.0;
                    }
                    
                    Serial.print("Rotation complete. New absolute position: ");
                    Serial.print(currentPositionDegrees);
                    Serial.println(" degrees (relative to start).");

                } else {
                    Serial.print("ERROR: Invalid degree value provided: ");
                    Serial.println(valueStr);
                }
            } else {
                Serial.print("ERROR: Unknown command: ");
                Serial.println(command);
            }
        } else {
            Serial.println("ERROR: Command format must be 'rotate <degrees>'.");
        }
    }
}
