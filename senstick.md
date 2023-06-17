SenStick
========
I want a lot of environmental sensors in the form factor of a USB stick.

Somehow this is not already a thing??

Time to learn PCB design???


Goals
-----
- Sense temperature, humidity, pressure, light, air quality, perhaps other things?
- Report back to Home Assistant (so presumably running esphome?)
- Be able to plug directly into a USB-A power socket, without blocking adjacent sockets
  - so 10mm wide, 5mm tall, probably not more than 50mm long to minimise risk of snapping?
  - Maaaaaybe using a cable is actually a better idea so that the unit can be
    placed in a better location? That's slightly less elegant, but perhaps more
	practical...

v1
--
As a proof-of-concept, Wemos make a bunch of environmental shields which cover a lot of needs

- [Wemos D1 Mini](https://www.wemos.cc/en/latest/d1/d1_mini.html)
- [Ambient Light Shield](https://www.wemos.cc/en/latest/d1_mini_shield/ambient_light.html)
- [Barometric Pressure Shield](https://www.wemos.cc/en/latest/d1_mini_shield/barometric_pressure.html)
- [SHT30 Shield (Temperature & Humidity)](https://www.wemos.cc/en/latest/d1_mini_shield/sht30.html)
- [SGP30 Shield (TVOC & eCO2)](https://www.wemos.cc/en/latest/d1_mini_shield/sgp30.html)

This fundamentally works, and might actually be "good enough", but could be better:

- It's 50x170x10mm
- Bluetooth proxy would be nice (requires esp32, and d1 mini is esp8266)

![Breadboard](./.github/images/senstick/breadboard.jpg?raw=true)

v1.1
----
Replacing the breadboard with stackable headers, size is now 30x25x50mm. It
could easily be half the size with more efficient headers, or a quarter the
size if the four sensor boards were replaced with a single multi-sensor board,
but this is sufficient to leave in a corner of my office to collect data.

![Stack 1](./.github/images/senstick/stack1.jpg?raw=true)
![Stack 2](./.github/images/senstick/stack2.jpg?raw=true)



Build & Run
-----------
Get the code
```
git clone https://github.com/shish/esphome-projects
cd esphome-projects
```

Install ESPHome
```
python3 -m venv .venv
./.venv/bin/pip install esphome
```

Populate `secrets.yaml`
```
wifi_ssid: "My Wifi Thing"
wifi_password: "o876tfvhy"
backup_ap_password: "jhgasdfvstr"
```

Build / install / run (With the D1 Mini connected via USB the first time, can be updated over wifi once it's running), using `-s` to give each instance a unique name:
```
./.venv/bin/esphome -s devicename senstick-office run senstick.yaml
```
