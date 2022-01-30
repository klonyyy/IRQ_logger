# STM32 interrupt logger (IRQ logger)

A simple, minimalistic tool for diagnosing interrupt execution times without additional hardware

![ ](./docs/photo/plot.gif)

## Motivation

I was fed up with the need to connect my scope or logic analyzer to the PCB whenever I wanted to check if the interrupts are executed the way I wanted them to. This made me create this super-easy tool to diagnose interrupts with just a little bit of RAM for the logs buffer and an overhead of 400ns per single log entry (or 200ns without logging turned on) @ 170 Mhz core clock. 

## How it works

Basically you put *IRQ_logger_add_entry(ID,ENTER)* and *IRQ_logger_add_entry(ID,LEAVE)* markers in the code, compile and download the firmware to the STM32 device. Then, with the ST-Link programmer attached, you run the Python script (src/python_decoder/main) which triggers logging, reads the log buffer, saves it to a \*.bin file, and plots the waveforms on a plot with cursors. 


## Step-by-step setup

1. Copy *src/IRQ_logger* folder to your STM32 project directory
2. Add the *IRQ_logger* folder to your project's include paths and make sure it is not marked as "excluded from compilation" (CUBE IDE)
3. Setup a timestamp timer with a resolution of 1 us and an update event taking place each 1 ms. Modify the IRQLOGGER_TIM definition accordingly (IRQ_logger.h file)
4. Place *IRQ_logger_millisecond();* function in the 1 ms interrupt handler
5. Add log markers in your code and set approperiate buffer size using MAX_ENTRIES define (IRQ_logger.h file)
6. Place *IRQ_logger_init();* somewhere in the init section of your program - this function just sets up the IRQLOGGER_TIM interrupt and turns on the counter
7. Compile and download the firmware
8. Proceed to the src/python_decoder project
9. Modify the user-defined section (probably the only thing you will have to change when running the example is the \*.elf file location, other parameters are described with comments)
10. Be sure the ST-Link is connected to the target and simply run the script
11. Place first cursor with left mouse button click and the second with right mouse button click. Second click releases them (be sure you aren't in the move or zoom mode, otherwise the cursors are not working)

## License
[MIT](https://choosealicense.com/licenses/mit/)

