# vtimer

Virtual / Software timer app implemented on a single hardware timer.

## Hardware Timer

It is simulated using epoch values.
Generally, it is hardware tick or counter value generated by hardware clock on a microcontroller.

## Platform

This application is not platform independent. It is based on Unix/Linux.

## COMPILATION STEPS

    - On Unix/Linux platform, gcc compiler is used to compile the application.
    - Navigate to the root directory of the repository on the terminal.
    - Type this command- "make all"
    - Find the binary in the build folder.
    - Run the binary using- "./<binary_name>"
