# ⚙️ Shoulder rehabilitation robot

#### It is a robot that implements shoulder rehabilitation exercises for Patients with stroke hemiplegia and adhesive capsulitis.

<br> 

## File description

- 📂 Shoulder-Rehabilitation-Robot : C, C++ project files (Visual studio 2017)
- 📂 hardware : circuit diagram

<br>

## Overview of research and development
### 1. Introduction
- Exoskeleton type 6 degree of freedom (DOF) robot arm
- Implementation of shoulder rehabilitation exercises : Abduction, Adduction, External rotation, Internal rotation, Flexion, Extension
<img width="1490" height="500" alt="intro" src="https://github.com/user-attachments/assets/48a76d4d-929c-444d-98ff-d09bcc3550ce" />

### 2. Design of mechanical parts
- brushless direct current motor - encoder-harmonic drive + 6 axis force-torque sensor : ①, ②, ③, ④
- Manual driven : ⑤, ⑥
<img width="736" height="554" alt="mechanical" src="https://github.com/user-attachments/assets/993bf34b-9513-41d9-959f-2e3b517e38a4" />

### 3. Design of Hardware
- Structure
<img width="1015" height="642" alt="structure" src="https://github.com/user-attachments/assets/f8f76b8a-c449-466d-907f-1ef549015faa" />

- Circuit diagram : [hardware](https://github.com/steveh78/Shoulder-Rehablilitation-Robot/blob/main/hardware/CIRCUIT.pdf)

### 4. Analyze movements
- Experiment for motion capture and marker tracking. (devices : Optitrack, tool : Motive)

https://github.com/user-attachments/assets/0b5e954c-195b-4ace-b7ca-dc25c8304fcb

- Realized of degree variation

<img width="1589" height="452" alt="analyze" src="https://github.com/user-attachments/assets/09049880-7f2e-4e6e-b994-42181816aef1" />

- Verification with 3D robot simulation model (tool: RoboAnalyzer)

https://github.com/user-attachments/assets/5de4553c-6842-49fd-bbc3-fc5a888a26d6

- For detailed information, please refer to this [paper](https://www.mdpi.com/2075-4418/12/12/3179)
