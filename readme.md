
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
12.  `sudo raspi-config` and select Interface, then enable SPI and I2C.
13.  `sudo reboot now`
14.  Reconnect via `ssh pi@192.168.7.XXX`
15.  Run `lsmod | grep spi` and confirm that you see `spi_bcm2835`.
16.  Run `sudo i2cdetect -y 0` and confirm that 68 is visible.
17.  `sudo apt install git python3-pip i2c-tools`
18.  `sudo pico /boot/config.txt` and add to the end of the file:  `dtoverlay=i2c-rtc,ds3231`
19.  `sudo reboot now`
20.  Run `sudo i2cdetect -y 0` and confirm that 68 has been replaced with UU.
21.  `sudo apt-get -y remove fake-hwclock`
22.  `sudo update-rc.d -f fake-hwclock remove`
23.  `sudo systemctl disable fake-hwclock`
24.  `sudo nano /lib/udev/hwclock-set` and comment out these lines:

    #if [ -e /run/systemd/system ] ; then
    #    exit 0
    #fi

25.  Also comment out the two lines commented here:

    if [ yes = "$BADYEAR" ] ; then
    #    /sbin/hwclock --rtc=$dev --systz --badyear
        /sbin/hwclock --rtc=$dev --hctosys --badyear
    else
    #    /sbin/hwclock --rtc=$dev --systz
        /sbin/hwclock --rtc=$dev --hctosys
    fi

26.  With the Raspberry Pi on a network so you know it knows the right time, run `sudo hwclock -w` to write the current time to the real time clock.
27.  Run `sudo hwclock -r` to confirm that the real time clock's time has been updated.  That's all, the real time clock should be set and will be used for timekeeping in the future.
28.  `pip3 install spidev mfrc522 smbus`
29.  Install pi-rc522 using

    git clone https://github.com/ondryaso/pi-rc522.git
    cd pi-rc522
    sudo python3 setup.py install

30.  `cd` back to home and install RFIDbee using `git clone https://github.com/wgrover/RFIDbee.git`
31.  `cd RFIDbee` and run `python3 read3.py`.  Confirm that RFID tags read OK and are added to `logfile.txt`.
32.  sudo Create file `/lib/systemd/system/RFIDbee.service` containing

    [Unit]
    Description=RFIDbee
    After=multi-user.target

    [Service]
    ExecStart=/usr/bin/python3 /home/pi/RFIDbee/read.py

    [Install]
    WantedBy=multi-user.target

33.  `sudo systemctl daemon-reload`  (this must be run after any edits to `RFIDbee.service`)
34.  `sudo systemctl enable RFIDbee.service`
35.  `sudo reboot now` and the reader program should start automatically after reboot.
36.   `sudo pico /boot/config.txt` and add the following at the end of the file:

  dtoverlay=gpio-shutdown,gpio_pin=26,active_low=1,gpio_pull=up

System will now shutdown whenever GPIO 26 is grounded.
37.  Shorting the RUN pins together can be used for a startup switch
