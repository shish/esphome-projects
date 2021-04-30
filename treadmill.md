Wifi Treadmill
==============
Because why not have a remote-control for a device which literally only makes
sense to be operated in person?

Reverse-engineering the control codes
-------------------------------------
up / reset / down / on / off
```
[14:32:58][D][remote.lg:053]: Received LG: data=0x807F708F, nbits=32
[14:32:58][D][remote.nec:068]: Received NEC: address=0x807F, command=0x708F
[14:32:58][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x800E
[14:32:58][D][remote.lg:053]: Received LG: data=0x807F708F, nbits=32
[14:32:58][D][remote.nec:068]: Received NEC: address=0x807F, command=0x708F
[14:32:58][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x800E

[14:33:00][D][remote.lg:053]: Received LG: data=0x807F609F, nbits=32
[14:33:00][D][remote.nec:068]: Received NEC: address=0x807F, command=0x609F
[14:33:00][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x8006
[14:33:00][D][remote.lg:053]: Received LG: data=0x807F609F, nbits=32
[14:33:00][D][remote.nec:068]: Received NEC: address=0x807F, command=0x609F
[14:33:00][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x8006

[14:33:01][D][remote.lg:053]: Received LG: data=0x807F30CF, nbits=32
[14:33:01][D][remote.nec:068]: Received NEC: address=0x807F, command=0x30CF
[14:33:01][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x800C
[14:33:02][D][remote.lg:053]: Received LG: data=0x807F30CF, nbits=32
[14:33:02][D][remote.nec:068]: Received NEC: address=0x807F, command=0x30CF
[14:33:02][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x800C

[14:33:03][D][remote.lg:053]: Received LG: data=0x807F20DF, nbits=32
[14:33:03][D][remote.nec:068]: Received NEC: address=0x807F, command=0x20DF
[14:33:03][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x8004
[14:33:03][D][remote.lg:053]: Received LG: data=0x807F20DF, nbits=32
[14:33:03][D][remote.nec:068]: Received NEC: address=0x807F, command=0x20DF
[14:33:03][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x8004

[14:33:04][D][remote.lg:053]: Received LG: data=0x807FB847, nbits=32
[14:33:04][D][remote.nec:068]: Received NEC: address=0x807F, command=0xB847
[14:33:04][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x801D
[14:33:05][D][remote.lg:053]: Received LG: data=0x807FB847, nbits=32
[14:33:05][D][remote.nec:068]: Received NEC: address=0x807F, command=0xB847
[14:33:05][D][remote.pioneer:144]: Received Pioneer: rc_code_X=0x801D
```