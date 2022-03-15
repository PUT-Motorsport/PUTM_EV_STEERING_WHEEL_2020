# PUTM_EV_STEERING_WHEEL_2022

An overview of how I see this thing working:
![kiera-schemat-blokowy-v2-1.png](https://postlmg.cc/ns6YXnjM)

* Data gathered from IMU will be calculated into degrees and stored in buffers. These will be send out when the buffer is filled up. 
* Interrupt will be triggered if a button is released. Then it will determine which button was pressed and send corresponding data. If, within specified, short time, there will occur another interrupt, then the programm will treat it as different action (two buttons pressed simultaneously). There will be 10 combinations: 4 single- and 6 simultaneous-presses.
* For the scroll-like switches there are going to be 4 voltage thresholds. Interrupts will be triggered if voltage value will cross min or max range of given level.
