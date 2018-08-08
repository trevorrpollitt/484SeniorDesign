# Chip-Pro-C


### Setup

#### Must connect to WiFi first.
Connect to another network besides ncsu. This will cause the next step to always fail. 

**Password for ChiP is:**  pentair5

#### Run update to be able to do the rest!
```bash
$ sudo apt-get update
```

#### Install nano to view text files
```bash
$ sudo apt-get install nano
```

#### Setup Wifi as an access point
[forum](https://bbs.nextthing.co/t/create-a-very-basic-wifi-access-point/1727)

[instructions from slack-files](https://slack-files.com/T02GVC9G6-F0H7G3WCT-25e7dfb781)

#### Install gcc to compile C code
```bash
$ sudo apt-get install gcc
```

#### Install header files for C code
```bash
$ sudo apt-get install build-essential
```

#### Install a networking/Wifi library
```bash
$ sudo apt-get install ifupdown
```

#### Install networking libraries for https
```bash
$ sudo apt-get install libssl-dev
$ sudo apt-get install libcurl4-openssl-dev
```
**Check to see if successfully installed**

Source: [techietown](http://www.techietown.info/2017/05/install-openssl-devel-ubuntu-using-apt-get/)

```bash
$ dpkg -s libssl-dev
$ dpkg -s libcurl4-openssl-dev
```
##### Install JSON Library
[Tutorial link](https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/)
```bash
$ sudo apt-get install libjson0 libjson0-dev
```

### How to compile C code

#### New (simple) way
There is a **script.sh** file which calls the **Make** program. Make then executes the included **makefile**.

You may have to give the script permission.
```bash
$ chmod +x script.sh
```
To run the script:
```bash
$ ./script.sh
```

If that fails or you want alternative run Make directly.
```bash
$ make
```

#### Old (manual) Way
The main program is called 'main' with the source file as 'main.c'.
There should be an associated c header file (.h) for each source file (except main.c).
```bash
$ gcc -o main main.c aws_lambda.c hx711.c ...  -lcurl -lssl -ljson
```

** To find location of a binary library **
This is for libssl-dev which has libssl.a. Change this
```bash
$ sudo apt-file update
$ LIBSSL_PATH=`apt-file list libssl-dev | grep libssl.a | awk '{ print $2 }'`
$ echo $LIBSSL_PATH
```

### Wiring Setup
To determine how to hook up your single strain gauge load cells to the combinator, measure the resistance between the three wires. You should find a larger resistance (close to double) between a pair. In our example the resistance between red and black was 1.6 kΩ, and the resistance between white/green and red was 800 Ω. Therefore, the center tap to the strain gauge is the white/green wire. The center tap or center pin of your strain gauge connects to the “C” pin on the combinator. The larger resistance wires (red and black in this example) connect to the “+” and “-” pins on the combinator.

* red to white = 1k - red for sure is C
* red to black = 1k - red for sure is C
* white to black = 2k = + and - OR - and + hmmm...

The larger resistance wires (red and black in this example) connect to the “+” and “-” pins on the combinator* 


### For BILL
To connect via serial:

```bash
cd /dev/serial/by-path
screen pci-0000:00:14.0-usb-0:2:1.2 115200
````

To exit screen,
```
Ctrl-A d or Ctrl-A Ctrl-D
```

### Error Codes Types

* 0 = Not an error
* 1 = AWS
* 2 = EagleIO
* 3 = Curl error
* 4 = HX711
* 5 = LEDs
* 6 = System error
