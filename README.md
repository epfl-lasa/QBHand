# QBHand
Installation instructions and software for interfacing with the QBHand.  (Unix/Linux instructions)

(For Windows pull the -Windows branch.)



## Step 1: Verify USB/Serial Driver

List connected usb devices
  ```
  ~$ lsusb
  ```
  Note Vendor and Product
  ```
  Bus 001 Device 022: ID 0403:60155 Future Technology Devices International, Ltd Bridge(I2C/SPI/UART/FIFO)
  ```

Modprobe your usbserial adapter
  ```
  ~$ modprobe usbserial vendor=0x0403 product=0x60155
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

## Step 2: Install qbAPI
The qbAPI is tghe C++ library needed to communicate and control the qbHand.We provide this folder in this repo, but if you need to update to a newer version download from: [qbAPI](https://github.com/qbrobotics/qbAPI)

```
~$ cd ~/QBHand/qbAPI/src
~$ make

```

## Step 3: Install qbAdmin
This package contains command-line tools for easy debugging and open/close of the hand. Downloaded from here: [qbAdmin](https://github.com/qbrobotics/qbadmin)

```
~$ cd ~/QBHand/qbadmin/src
~$ make

```
To test API installation:

```
~$ cd ~/QBHand/qbadmin/bin_linux/
~$ ./qbmove_init

```


