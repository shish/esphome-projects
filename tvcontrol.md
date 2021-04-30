ESPHome TV Control
==================

The Problem
-----------
- My TV gets left on overnight, disturbing the cat's sleep.
- If all devices would cut the signal when they are idle, then it would turn itself off after 30 seconds of "no signal"... but the Chromecast INSISTS on showing a slideshow 24/7 :/
  - This Chromecast behaviour also messes up the "automatically switch to whichever input is active" behaviour :/
- The TV has a "turn itself off after N minutes even if there are active inputs" feature... but the limit is 3 hours, which isn't long enough to watch Lord of the Rings without interruption; and it's too long for the average day where we stop watching at midnight and want it auto-off by 1am.

The Solution
------------
- Have a wifi-connected infrared transmitter which will turn the TV off at 1am
- Also be able to switch HDMI inputs via Home Assistant which is more reliable than the auto-detection.