# Self Balancing Robot (Arduino + MPU6050 + L298N)

Arduino se bana hua 2-wheel self balancing robot jo MPU6050 sensor aur PID control use karta hai.

---

## 🔧 Required Components

| Component | Quantity |
|---|---|
| Arduino Uno / Nano | 1 |
| MPU6050 (Gyro + Accelerometer) | 1 |
| L298N Motor Driver | 1 |
| DC Gear Motors | 2 |
| Wheels | 2 |
| Li-ion Battery (7.4V or 11.1V) | 1 |
| Jumper Wires | — |
| Robot Chassis (2-wheel) | 1 |

---

## 📌 Pin Connections

### MPU6050 (I2C)
| MPU6050 Pin | Arduino Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |

### L298N Motor Driver
| L298N Pin | Arduino Pin |
|---|---|
| ENA (Motor A speed) | 5 (PWM) |
| IN1 | 2 |
| IN2 | 3 |
| ENB (Motor B speed) | 6 (PWM) |
| IN3 | 10 |
| IN4 | 11 |

---

## ⚙️ PID Settings

| Parameter | Value |
|---|---|
| Kp | 60 |
| Ki | 0.0 |
| Kd | 60 |
| Setpoint | 0 (upright position) |
| Output Limit | -120 to +120 |

> **Tip:** Agar robot balance nahi kar raha toh Kp aur Kd values adjust karein.

---

## 📚 Required Libraries

Libraries ka `libraries.txt` file dekho.

---

## 🔄 How It Works

1. MPU6050 se angle read hota hai (Accelerometer + Gyroscope)
2. Complementary Filter se accurate angle calculate hota hai (98% gyro + 2% accel)
3. PID controller angle ko 0 (upright) pe rakhne ki koshish karta hai
4. L298N motor driver ke zariye motors forward/backward chalti hain

---

## ⚠️ Notes

- Robot ko pehle flat surface pe test karein
- Battery fully charged honi chahiye
- `setpoint = 0` matlab robot bilkul seedha khara ho — agar thoda aage/peeche jhukta ho toh setpoint adjust karein (e.g. 2 ya -2)
