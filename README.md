# QBHand
Installation instructions and software for interfacing with the QBHand.  (Unix/Linux instructions).

## Hardware Setup
- 2 power supplies with 12V each, one connected to channel 1 to power the dummy and the other one connected to channel 5 to power the hand.
- Bus connection from hand connected to Channel 5.
- Mini-usb cable connected from dummy to your PC.

<p align="center">
<img src="https://github.com/epfl-lasa/QBHand/blob/master/documentation/hand_connected.jpg" width="390"><img src="https://github.com/epfl-lasa/QBHand/blob/master/documentation/power_connections.jpg" width="390">
</p>

## Step 1: Verify USB/Serial Driver

List connected usb devices
  ```
  ~$ lsusb
  ```
  Note Vendor and Product
  ```
  Bus 001 Device 022: ID 0403:6015 Future Technology Devices International, Ltd Bridge(I2C/SPI/UART/FIFO)
  ```

Modprobe your usbserial adapter
  ```
  ~$ modprobe usbserial vendor=0x0403 product=0x6015
  ```

Find the number of your usbserial adapter driver
  ```
  ~$ dmesg | grep 'ttyUSB'
  ```
  Note ttyUSBX
  ```
  usb 1-1: FTDI USB Serial Device converter now attached to ttyUSB0
  ```

Enable adapter driver:
  ```
  ~$ sudo chmod 777 /dev/ttyUSB0
  ```

Check connection with serial port:
  ```
  ~$ cu -l /dev/ttyUSB0 -s 9600
  Connected.
  ```
  If you see the ```Connected.``` message everything's fine and you can disconnect by:
  ```
  ~$ ~.
  Disconnected.
  ```
  Otherwise something's wrong and you must debug it! :smiling_imp:
  

## Step 2: Try HandTool Gui
To use the GUI you should add user to dialout group as follows (only works under Ubuntu 16.04 LTS):
```
~$ sudo adduser user_name dialout
```
Then set permissions for the GUI:

```
~$ cd ~/QBHand/HandTool_unix_v2.2.3/
~$ chmod a+x HandTool
```
and run:

```
~$ ./HandTool
```

## Step 3: Install qbAPI
The qbAPI is the C++ library needed to communicate and control the qbHand.We provide this folder in this repo, but if you need to update to a newer version download from: [qbAPI](https://github.com/qbrobotics/qbAPI).

```
~$ cd ~/QBHand/qbAPI/src
~$ make

```
## Step 4: Install qbadmin
This package contains command-line tools for easy debugging and open/close of the hand. Downloaded from here: [qbadmin](https://github.com/qbrobotics/qbadmin)
```
~$ cd ~/QBHand/qbadmin/src
~$ make

```

The hand and the dummy must have different IDs. To check the IDs write the following commands:

```
~$ cd ~/QBHand/qbadmin/bin_linux/
~$ ./qbadmin -p

```

To test API installation (sequence of open/close commands):

```
~$ cd ~/QBHand/qbadmin/bin_linux/
~$ ./qbadmin -k

```

## Step 5: ROS Interface
This ROS node is an interface between the qbAPI and ROS, it was taken from here: https://github.com/qbrobotics/qb_interface_node and updated with the qbAPI compatible with our specific hand. After installing the package in your ```catkins_ws``` you can launch the node as follows:
```
~$ roslaunch qb_interface qb_interface.launch
```
We can read the motor measurements on the following topic: 
```
~$ rostopic echo /qb_class/hand_measurement
```
And send commands by publishing this topic

*To open:*
```
 rostopic pub --once /qb_class/hand_ref qb_interface/handRef '{closure:[0.0]}'
```
*To close:*
```
 rostopic pub --once /qb_class/hand_ref qb_interface/handRef '{closure:[19000.0]}'
```
To have partial closing, use numbers in between [7000,15000].
 
