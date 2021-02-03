
1.  Write Raspbian Lite to SD card
2.  Remove, reinsert, and open terminal
3.  Create file `wpa_supplicant.conf` containing

    ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
    update_config=1
    country=US
    network={
        ssid="Boyo Arroyo"
        psk=""
    }

4.  Create an empty file named `ssh`
5.  Insert into Raspberry Pi Zero W and boot
6.  Find ip address on network and `ssh pi@192.168.7.XXX` using the password `raspberry`.
7.  Change password using `passwd` command.
8.  `sudo apt update`
9.  `sudo apt upgrade`
10.  `sudo reboot now`
11.  Reconnect via `ssh pi@192.168.7.XXX`
11.  `sudo raspi-config` and select Interface, then enable SPI and I2C.
12.  `sudo reboot now`
13.  Reconnect via `ssh pi@192.168.7.XXX`
14.  Run `lsmod | grep spi` and confirm that you see `spi_bcm2835`.
15.  Run `sudo i2cdetect -y 0` and confirm that 68 is visible.
15.  `sudo apt install git python3-pip i2c-tools`
16.  `pip3 install spidev mfrc522 smbus`
17.  Install pi-rc522 using

    git clone https://github.com/ondryaso/pi-rc522.git
    cd pi-rc522
    sudo python3 setup.py install

18.  `cd` back to home and install RFIDbee using `git clone https://github.com/wgrover/RFIDbee.git`
19.  `cd RFIDbee` and run `python3 read3.py`.  Confirm that RFID tags read OK and are added to `logfile.txt`.
20.  sudo Create file `/lib/systemd/system/RFIDbee.service` containing

    [Unit]
    Description=RFIDbee
    After=multi-user.target

    [Service]
    ExecStart=/usr/bin/python3 /home/pi/RFIDbee/read.py

    [Install]
    WantedBy=multi-user.target

21.  `sudo systemctl daemon-reload`  (this must be run after any edits to `RFIDbee.service`)
22.  `sudo systemctl enable RFIDbee.service`
23.  `sudo reboot now`
