## Hello World

Super-basic program that prints a message on the serial console and blinks the onboard LED of the ESP8266 in 0.5s interval.

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
