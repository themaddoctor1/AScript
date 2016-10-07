#AScript

AScript (ArduinoScript, or An AScript) is my attempt to create a custom language that can be converted into assembly for use by the Arduino UNO's ATmega328P processor. I decided to do this while in CS383 (Computer Organization and Programming), in which we had to learn assembly language for use on an Arduino UNO with an ATmega328P processor.

##Syntax

####Variables

Currently, the only datatype supported is byte. They can be instantiated like so:

```
byte foo = 12;
```

If a variable is not given an initial value, then it will automatically default to 0 in the compiler. It is important to note that one should not name variables after tokens or numbers, since the compiler will treat those cases incorrectly. For example,

```
byte 123;
byte byte;
byte if;
```

are all extremely terrible things that you should not do in any program.

####Loops

AScript allows for for loops and while loops. Both must be terminated with a semicolon when present.

```
byte factorial = 1;
byte n = 6;
while(i) {
    factorial = factorial * n;
    n = n - 1;
};
```

This code snippet will iterate and compute n!. In the function, n will continuously decrement until it equals zero, in which case the loop will terminate.

```
byte input = 6;
if(input) {
    input = 0;
};
```

This segment of code takes the value of input and sets it to zero if it is not already zero.

####Arithmetic

The compiler is written so that it will have the program run through the correct order of operations. The program will use the same order of operations used by the C family of operations. That is, ```() -> * -> + or - -> & -> |```. Currently, division is not supported, but that and other operations will eventually be supported.

##Usage

Currently, there is no available builds of the compiler. However, the compiler source can be downloaded from here and compiled. To build the compiler, run the included makefile with 

```
make all
```

This will compile the source into an executable. Then, run the executable with your sourcefile as a parameter. Note that the source file must be a .scr file. The program will output the result into a .asm file in the current directory as long as it compiled correctly. The asm file can then be used in your IDE of choice.

##Future

In the future, I hope to add more functionality to the language. At this point, the language still needs to undergo a lot of testing and improvement. Future changes will hopefully include optimization and I/O through the Arduino's I/O ports.
