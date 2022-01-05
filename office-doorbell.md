ESPHome Office Doorbell
=======================


The Problem I Want to Solve
---------------------------
I'm working from home, which my family assumes is synonymous with
"on holiday". Sometimes I'm left to focus, other times (typically
around 15 minutes after announcing "I'm going to spend the next
hour giving a presentation over Zoom, please no interruptions"),
somebody will come into the room crying because they saw a cute
cat on instagram and they had to share it in person.


The Idea
--------
A blinking red sign on the door that says "Do not Disturb"


The Implementation Idea
-----------------------
Two M5Stick-C devices, one on my desk, one on the door

- The door LCD & LED show current status via text (Break / Work /
  Do not Disturb) and light (off / on / blinking)
- Clicking the door button will make the desk blink, and the door
  LCD shows "Ringing"
- If the desk is in "Ringing" mode, then clicking the button will
  stop it
- If the desk is not ringing, then clicking the button will toggle
  between Break / Work / Do not Disturb

### Who controls what data?

Each unit exposes their own buttons as `binary_sensor`s.

The "desk" unit has two bits of extra state:

- `state` - internally an integer, externally a `text_sensor` which
  exposes `Rest` / `Work` / `Focus`
- `request` - internally a boolean, externally a `switch` which can
  be `on` or `off`

The "door" unit has no state of its own, just some `sensor`s which
subscribe to Home Assistant's data feed to keep themselves in sync
with the "desk" sensors. Eg, whenever `desk.state` is changed, it
publishes that state to HA, then HA broadcasts it to the network,
the door sees that broadcast, and the door updates its own state to
match.

The "desk" also has virtual sensor monitoring the door's button,
which it uses to trigger the `request = true` state.


The Result
----------
![Door - Far](./.github/images/doorbell/door-far.jpg?raw=true)
![Door - Close](./.github/images/doorbell/door-close.jpg?raw=true)

![Desk - Far](./.github/images/doorbell/desk-far.jpg?raw=true)
![Desk - Close](./.github/images/doorbell/desk-close.jpg?raw=true)


Browse the Code
---------------
* [Office Desk](./officedesk.yaml)
* [Office Door](./officedoor.yaml)


Build & Run
-----------
Get the code
```
git clone https://github.com/shish/esphome-projects
cd esphome-projects
```

Get the M5StickC components for ESPHome
```
git clone https://github.com/airy10/esphome-m5stickC
ln -s esphome-m5stickC/components custom_components
```

Install ESPHome
```
python3 -m venv .venv
./.venv/bin/pip install esphome pillow
```

Populate `secrets.yaml`
```
wifi_ssid: "My Wifi Thing"
wifi_password: "o876tfvhy"
backup_ap_password: "jhgasdfvstr"
```

Pick a font in .ttf format and save it in the project folder (I've used
helvetica.ttf, but any should work)

Build / install / run (with m5stick connected via USB the first time, can
be updated over wifi later times)
```
./.venv/bin/esphome run officedesk.yaml
./.venv/bin/esphome run officedoor.yaml
```


Future Work
-----------
Somehow tie this into my office calendar to automatically have
`state = Work` during working hours and `state = Focus` during
meetings?
