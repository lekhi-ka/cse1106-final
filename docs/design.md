•	The C program starts and initializes the pigpio interface (gpioInitialise())
•	The designated GPIO pin (e.g., GPIO 17) is configured as an output mode (gpioSetMode(PIN, PI_OUTPUT)).
•	The software queries the operating system metrics to calculate the current percentage of Memory Usage and Storage Usage. 
The program compares each metric against the active warning threshold (e.g., 80%).
•	If any metric exceeds the threshold, the system state evaluates to WARNING; otherwise, it evaluates to HEALTHY. 
•	If status is WARNING, the program writes a high signal (gpioWrite(PIN, 1)) to turn the LED ON. 
•	If status is HEALTHY, the program writes a low signal (gpioWrite(PIN, 0)) to turn the LED OFF.
•	The calculated percentages, timestamp, and resulting status are appended to a local diagnostic text file (diag_results.txt). 
•	Upon user exit (e.g., pressing ENTER), the program resets the GPIO pin to low (safe state) and terminates the pigpio session cleanly (gpioTerminate()). 

