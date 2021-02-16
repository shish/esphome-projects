
Get the code
```
git clone https://github.com/shish/esphome-projects
cd esphome-projects
```

Get the M5StickC libraries
```
git clone https://github.com/airy10/esphome-m5stickC
ln -s esphome-m5stickC/components custom_components
```

Install esphome
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

Build / install / run (with m5stick connected via USB the first time)
```
./.venv/bin/esphome officedesk.yaml run
```