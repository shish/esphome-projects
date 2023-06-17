HP303B Support for ESPHome
==========================

The [Barometric Pressure Shield](https://www.wemos.cc/en/latest/d1_mini_shield/barometric_pressure.html) is pretty neat, but not supported by vanilla esphome.

There is a [4 year old custom component using old and inelegant APIs](https://github.com/dmptrluke/esphome_hp303b)

There is also a [pull request for upstream support](https://github.com/esphome/esphome/pull/4864) which I was unable to get working with a half hour of fiddling.

So I ended up making my own component, which has the downside of depending on an external library - but it is an external library which can be included via yaml, no need for manually copy-pasting .h files.

Given a basically-working D1 Mini yaml config, these bits can be added to make the pressure shield work (Check my projects at the root of the repo to find a complete yaml example):

```
esphome:
  libraries:
    - Wire
    - SPI
    - "https://github.com/wemos/LOLIN_HP303B_Library.git"

external_components:
  - source: github://shish/esphome-projects
    components: [ hp303b ]

sensor:
  - platform: hp303b
    temperature:
      name: "My Temperature"
    pressure:
      name: "My Pressure"
    address: 0x77
    update_interval: 60s
```
