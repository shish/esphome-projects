ESPHome Smart Dehumidifier
==========================


The Problem I Want to Solve
---------------------------
- My shower room gets pretty humid
- I don't want to run a dehumidifier 24/7
- Smart dehumidifiers are expensive


The Idea
--------
- Take a regular cheap dehumidifier
- Replace the on/off switch with an esp8266
- Have Home Assistant turn on the dehumidifier whenever the sensor
  in the room detects a certain amount of humidity.


The Hardware
------------
- [A generic dehumidifier](https://smile.amazon.co.uk/gp/product/B08BHH7LHQ/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
- Replacement controller hardware is detailed below, as it took a few approaches...

![Dehumidifier](./.github/images/dehumidifier/dehumidifier.jpg?raw=true)


The Software
------------
- [ESPHome Config File](dehumidifier.yaml)


Quick Investigation
-------------------
- Taking the dehumidifier apart. First hurdle was that I didn't know how to
  take it apart, and ended up taking it apart too much, by force, breaking
  the plastic a little bit. It turns out that I could've just removed the
  circular top panel by itself, and that's all that I actually needed to do.

![Broken Open](./.github/images/dehumidifier/break-open.jpg?raw=true)
- With the top panel removed, we can see that there isn't actually an on/off
  switch... there's a capacitive touch sensor connected to a microcontroller
  which cotrols the motor and the LEDs, so we'll need to either work with
  that or replace it completely :/

![Control Panel](./.github/images/dehumidifier/control-panel.jpg?raw=true)
- On the plus side: this default control panel runs at 5V while all the
  mechanical parts run at 9V. The relay board we use expects 5V control input
  (not sure why - the ESP is a 3.3V chip, maybe the relay itself needs 5V?),
  so we should be able to steal the control panel's 5V input for ourselves?
- As a quick hack based on a hunch, I jammed some jumper wire in there to
  connect the "OUT" and "GND" pins - it started running, overriding the
  control panel \o/

![Jump to GND](./.github/images/dehumidifier/jump-to-gnd.jpg?raw=true)
- Interestingly, with the control panel disconnected completely, it starts
  running by default - so it seems like the control panel needs to be present
  and sending signal down the OUT pin in order for it to turn _off_??
  - With the control panel removed, connecting GND to OUT no longer
    turns it on - but connecting 5V to OUT does turn it off.

Hardware Hack
-------------
The first thing I note is that this existing control panel is a pain in the ass, and there's very little wiggle room or wire to work with. So my first step is going to be to desolder the stock panel, and replace `hardware <-> control panel` with `hardware <-> dupont wires M <-> dupont wires F <-> control panel`. This way I can keep the existing setup working to confirm that I haven't broken anything; I can stick some measuring tools in between the dupont wires to analyse the signals; and I can completely replace the control panel very easily. As a proof-of-concept, I first replaced the control panel with a tiny breadboard that used a simple button to send power to a single LED.

![Dupont 1](./.github/images/dehumidifier/dupont1.jpg?raw=true)
![Dupont 2](./.github/images/dehumidifier/dupont2.jpg?raw=true)
![Dupont 3](./.github/images/dehumidifier/dupont3.jpg?raw=true)
![Dupont 4](./.github/images/dehumidifier/dupont4.jpg?raw=true)

Deeper Investigation
--------------------
Now that I can insert a breadboard in between the hardware and the control panel, I can start easily taking a lot more readings to figure out what exactly is going on here.

Multimeter readings for each control line in each state:

State          | +5V | GND | R   | G   | B   | OUT | HR
---------------|-----|-----|-----|-----|-----|-----|-----
Off            | 5.0 | -   | 5.0 | 5.0 | 5.0 | 4.8 | 5.0
Motor          | 5.0 | -   | 4.9 | 4.9 | 4.9 | 0   | 5.0
Motor + Green  | 4.7 | -   | 2.0 | 4.3 | 4.7 | 0   | 4.7
Motor + Yellow | 4.5 | -   | 2.1 | 2.4 | 4.5 | 0   | 4.5

The "R" pin is connected to the Green LED; the "G" pin is connected to the Red LED; and the "B" pin is connected to the Blue LED.

Extra things which show up with an oscilloscope:

![Scope 1](./.github/images/dehumidifier/scope1.jpg?raw=true)
![Scope 2](./.github/images/dehumidifier/scope2.jpg?raw=true)

- R/G/B LEDs are pulse-width-controlled, not voltage-controlled.
- 3.96kHz, ~4V high, ~0V low
- The "R" pin cycles 0-55-0% every ~100s
- The "G" pin cycles 0-95-0% every ~15s
- The "B" pin cycles 0-95-0% every ~6m
- "HR" bounces 4.0 - 4.3V. Seems to go up when
  the LED duty goes up, down when it goes down.

Custom Controller, v1
---------------------
- The Hardware
  - [ESP-01S](https://www.aliexpress.com/item/32821697306.html?spm=a2g0s.9042311.0.0.27424c4dKBLXth)
  - [ESP-01 Relay Board](https://www.aliexpress.com/item/32821697306.html?spm=a2g0s.9042311.0.0.27424c4dKBLXth)

![ESP01](./.github/images/dehumidifier/esp01.jpg?raw=true)
- Plugging in the relay board to the 5V and GND pins, it doesn't boot :(
  - Measuring 5V-GND just on the pins, it shows up as 5V
    - For reference, 5V-GND on my test breadboard shows 5.1V
  - Measuring 5V-GND while the board is plugged into the pins, shows 2.5V?
    - OK maybe plugging something else into a circuit divides the voltage,
	  is that how it works??
	  - But when I plug in the board to my test breadboard, the same
	    measurement only drops to 4.9V???
    - Looks like the dehumidifier's built-in power regulator only has a tiny
      amount of power - the default control panel is 5V @ 25mA; the ESP uses
      75mA so it gets 1.8V. Is there a type of power supply that gives
      constant watts, and varies the voltage based on amps??

Custom Controller, v2
---------------------
- The Hardware
  - [ESP-12E]()
  - A [LN1117V33 Voltage Regulator]()
  - A bunch of [BC337 Transistors]()
- Trying another approach: I only actually need 3.3V - what if I use a 3.3V
  regulator on the 5V input, then use transistors so that the chip's 3.3V
  signals can control the 5V hardware?
  - This all works fine up until the same point as the last approach: as long
    as the ESP is externally powered (eg via USB) it all works fine - but when
    trying to power the ESP from the dehumidifier's internal power, the
    internal power drops to ~2V :(

Custom Controller, v3
---------------------
- The Hardware
  - [Wemos D1 Mini](https://www.wemos.cc/en/latest/d1/d1_mini.html)
  - [D1 Power Sheild](https://www.wemos.cc/en/latest/d1_mini_shield/dc_power.html)
  - [DC Power Barrel](https://smile.amazon.co.uk/gp/product/B077YGV3F4/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1)
- OK so I don't know electronics well enough to be sure, but it definitely
  looks like there's some kind of hard limit on how much power the built-in
  power supply can push. What if instead of taking power out of the unit,
  we intercept the power going into it?
  - Power: `Mains <-> Control panel <-> Dehumidifier`
  - Logic: `Dehumidifier <-> Control panel`
- The D1 mini has a power sheild which accepts 7-24V input, and gives three
  outputs for 3.3V, 5V, and VIN. Perhaps we can plug the mains (9V adapter)
  into this board, use 3.3V for ourselves, 5V to control the hardware, and
  forward the 9V back outside and into the regular power input?
  - Seems to work, but a challenge: the power barrels I bought are too big for the unit's in-set power socket :|
    - Solution: craft knife \o/

![Power 1](./.github/images/dehumidifier/power1.jpg?raw=true)
![Power 2](./.github/images/dehumidifier/power2.jpg?raw=true)
![Power 3](./.github/images/dehumidifier/power3.jpg?raw=true)
![Power 4](./.github/images/dehumidifier/power4.jpg?raw=true)

Analog Detour
-------------
All these circuits seem to be working inconsistently, so I want to try creating the smallest possible working circuit to make sure each individual part works.

![Analog 1](./.github/images/dehumidifier/analog1.jpg?raw=true)
![Analog 2](./.github/images/dehumidifier/analog2.png?raw=true)

I'm not sure what I did differently this time (maybe the position of the resistor??) but everything appears to work reliably now.

The two wires on the side of the ribbon cable are just there to make sure the ribbon is inserted in the correct position, as I don't want an off-by-one to end up frying my parts :)

Putting it all Together
-----------------------
Now with everything working, I try to get it all into a single package on a D1 Mini Duo base.


![Together 1](./.github/images/dehumidifier/together1.png?raw=true)
![Together 2](./.github/images/dehumidifier/together2.png?raw=true)
![Together 3](./.github/images/dehumidifier/together3.jpg?raw=true)
![Together 4](./.github/images/dehumidifier/together4.jpg?raw=true)

Soldering this all together on a mini board was a huge pain in the ass.

On the plus side, I learned that ESPHome allows you to combine three R/G/B pins into a single RGB light.

![Finished 1](./.github/images/dehumidifier/finished1.jpg?raw=true)
![Finished 2](./.github/images/dehumidifier/finished2.png?raw=true)

Ideally I'd also add a case to protect the electronics from getting splashed, but I don't have access to a 3D printer or laser cutter...

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

Build / install / run (with the ESP-01 board connected to the flashing dongle
the first time, can be updated over wifi later times)
```
./.venv/bin/esphome run dehumidifier.yaml
```


Future Work
-----------
- Add some effects to the software config (blink, rainbow, etc)
- Add a DHT11 sensor (or similar) to the control panel so that we can detect humidity for ourselves, and automatically turn on in a self-contained way.
- Have the light mean something (maybe map the spectrum onto the humidity levels?)
- Figure out why the dehumidifier's 5V rail is not sufficient to power the D1 mini via the 5V pin
- I notice that the fan sounds like it's going slower when I use a DC extension cable. The multimeter still shows 9V though. Is this extension cable limiting current?
