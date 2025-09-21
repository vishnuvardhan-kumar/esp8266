## Hello World

Super-basic program that prints a message on the serial console and blinks the onboard LED of the ESP8266 in 0.5s interval.

<img width="1197" height="211" alt="image" src="https://github.com/user-attachments/assets/c97f2d50-2682-4ebd-90f7-02144d84870b" />

![20250921_233546](https://github.com/user-attachments/assets/0d2f28f3-b47c-43ad-87be-26c1130480e3)

#### Module structure

The module also lays down the general C module structure that will be prevalent in this repository. More complex modules might have `components/` (for additional code) or `include/`.

```
module/                   <-- module directory
|- main/                  <-- code for main functionality
   |- main.c              <-- source file
   |- CMakeLists.txt      <-- component registration
   |- component.mk        <-- empty file for component reg
|- Makefile               <-- main make for module
|- CMakeLists.txt         <-- project include
```
