# RFIDbee

Code for "RFIDbee2" bee trackers

## Installing (or updating) software on RFIDbee2 trackers

1. Download the RFIDbee2 software from this repository.

1. Download and install the Arduino IDE for your operating system from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)

1. Open the Arduino IDE.  Select *Tools > Manage Libraries*.  The "Library Manager" sidebar should appear.  In the "Filter your search" box, enter the name of each of these three libraries and search for and install them:
* `Adafruit PN532`  (by Adafruit)
* `RTClib` (by Adafruit)
* `SdFat` (by Bill Greiman)
If you are asked about installing library dependencies, select *INSTALL ALL*.

1.  In the Finder (MacOS) or File Explorer (Windows), navigate to wherever you saved the RFIDbee2 software, and open the `RFIDbee2` subfolder.  Inside, double-click on the file `RFIDBEE2.ino`.  It should open in the Arduino IDE.

1.  

Install Python - make sure “add python.exe to PATH” is checked
Clone the GitHub repo
Open Windows PowerShell
pip install pyserial
cd to cloned repo, Arduino folder
run   