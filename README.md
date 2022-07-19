# PUTM_EV_STEERING_WHEEL_2022

An overview of how I see this thing working:
![kiera_schemat_blokowy_v2 (1)](https://user-images.githubusercontent.com/28296861/158438419-2575dc49-7128-4a67-a6cc-f2714906d276.png)

* ~~Data gathered from IMU will be calculated into degrees and stored in buffers. These will be send out when the buffer is filled up.~~ 
* Interrupt will be triggered if a button is pressed. Then it will determine which button was pressed and send corresponding data. If, within specified, short time, there will occur another interrupt, then the programm will treat it as different action (two buttons pressed simultaneously). There will be 10 combinations: 4 single- and 6 simultaneous-presses.
* The scroll-like switches will trigger an interrupt if its position changes.
