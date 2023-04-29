# Hardware PWM

The goal of this part of the lab was to fade an led using the timer module to generate an appropriate PWM signal. The path I chose was to use a signle timer module and cycle through the various led colors using a state machine in the timer interrupt. The state would first start out as 0 and whenever and interrupt would trigger from the timer module, the code for that state would be run and the state would then be increaes. I was unfortunatley unable to get this code to fully work, but I feel it is just a minor error which is causing my code to not fully function.
