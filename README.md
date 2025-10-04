# 🌀 Stepper Motor Serial Controller

This Arduino sketch allows you to **control a 28BYJ-48 stepper motor** (via a ULN2003 driver board) using **simple serial commands**. You can rotate the motor by a specified number of degrees — both clockwise and counterclockwise — directly from the Arduino Serial Monitor.

---

## ⚙️ Features

* Control your stepper motor by typing commands in the Serial Monitor.
* Rotate the motor **by any angle in degrees** (e.g. 90, -180, 45.5).
* Keeps track of the **absolute position** (0°–360°).
* Provides feedback for each command, including:

  * Steps calculated
  * Direction of rotation
  * New absolute position

---

## 🧰 Hardware Requirements

| Component           | Description                            |
| ------------------- | -------------------------------------- |
| **Microcontroller** | Arduino Uno (or compatible board)      |
| **Stepper Motor**   | 28BYJ-48 stepper motor                 |
| **Driver Board**    | ULN2003 driver module                  |
| **Wiring**          | Jumper wires (Male-to-Female)          |
| **Power**           | 5V DC (via Arduino or external supply) |

---

## 🔌 Wiring Diagram

| ULN2003 Pin | Arduino Pin |
| ----------- | ----------- |
| IN1         | 8           |
| IN2         | 9           |
| IN3         | 10          |
| IN4         | 11          |

**Note:**
The sequence order in the code is `(in1, in3, in2, in4)` which matches most 28BYJ-48 + ULN2003 configurations.
If your motor rotates erratically or in the wrong direction, try swapping to `(in1, in2, in3, in4)`.

---

## 🧠 How It Works

1. The motor’s full revolution has **2048 steps** (for 28BYJ-48 in full-step mode).
2. You send commands through the Serial Monitor, such as:

   ```
   rotate 180
   rotate -90.5
   ```
3. The program:

   * Converts degrees to steps.
   * Rotates the motor accordingly.
   * Updates and prints the new angular position.

---

## 🧮 Conversion Formula

```
steps = degrees × (stepsPerRevolution / 360)
```

For a 28BYJ-48:

```
steps = degrees × (2048 / 360)
```

---

## 💻 Serial Command Format

**Command syntax:**

```
rotate <degrees>
```

| Example      | Description                    |
| ------------ | ------------------------------ |
| `rotate 180` | Rotate clockwise by 180°       |
| `rotate -90` | Rotate counterclockwise by 90° |
| `rotate 360` | Full revolution                |
| `rotate 0`   | No movement                    |

**Invalid commands** (missing value, wrong format, or non-number) trigger an error message.

---

## 🧩 Code Overview

| Section                  | Description                                            |
| ------------------------ | ------------------------------------------------------ |
| `degreesToSteps()`       | Converts degrees to step count                         |
| `setup()`                | Initializes serial connection and motor speed          |
| `loop()`                 | Waits for serial input and processes rotation commands |
| `currentPositionDegrees` | Tracks motor’s absolute angular position               |

---

## 🧾 Serial Output Example

```
--- Stepper Motor Serial Controller ---
Motor initialized. Current position: 0.0 degrees.
Enter commands in the format: rotate <degrees>

> rotate 90
Received command: Rotate 90 degrees. Steps calculated: 512
Rotation complete. New absolute position: 90.00 degrees (relative to start).

> rotate -45
Received command: Rotate -45 degrees. Steps calculated: -256
Rotation complete. New absolute position: 45.00 degrees (relative to start).
```

---

## ⚡ Notes

* The default motor speed is **10 RPM** — you can adjust it with:

  ```cpp
  const int motorSpeedRPM = 10;
  ```
* Do not exceed **15–20 RPM** for 28BYJ-48 motors to avoid missed steps.
* Always **power the motor from a stable 5V source** (either from Arduino’s 5V pin or an external supply with shared ground).
* If the motor vibrates instead of rotating, check the **wiring order** and **power source**.

