# PUTM_EV_STEERING_WHEEL_2022

An overview of how I see this thing working:
[kiera_mock_v1.drawio.pdf](https://github.com/PUT-Motorsport/PUTM_EV_STEERING_WHEEL_2022/files/8193537/kiera_mock_v1.drawio.pdf)

* IMU should be broadcasting continuously
* Interrupt will be triggered if a button is released. Then program will check if there is another button pressed. If not, it will determine which button was pressed and send corresponding data. If there is another button pressed, then it will wait for it to be released, determie what combinations was submitted and send adequate data.
* For the scroll-like switches there are going to be 4 voltage thresholds. Interrupts will be triggered if voltage value will cross min or max range of given level.
