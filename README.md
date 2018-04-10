# NanoBot

Arduino Nano Robot from thingy verse

Source: http://www.thingiverse.com/thing:1582398

![Alt text](/nanobot-1.jpg?raw=true "Screen Shot")

![Alt text](/nanobot-2.jpg?raw=true "Screen Shot")


## NanoBot Assembly

1) Insert HC-SR04 module inside of NanoBot top with sensors exposed throught cut-out
    a) You need to secure it with some hot glue to affix it in place.
    b) Solder two wires to the NTE 54‑140 SPST On‑Off Subminiature Toggle switch.
    c) Insert the toggle switch through the hole cutout on the NanoBot top.
    d) Drill a small hole on the plastic top to allow the IR receiver module to fit through.
    e) Use hot glue to secure the IR receiver module in place.

2) Position the smart drive gear motor on the bottom of chassis printed part (see photo)
   a) Use two black tie strips through the chassis cutout holes to secure each motor in place.
        https://cdn.thingiverse.com/renders/36/ae/e2/46/54/1f91e9375c722ec8bd31ae379bfcbdfb_preview_featured.jpg
        https://cdn.thingiverse.com/renders/b8/17/ee/d0/f4/0d76dbddf703870af4104a86281642a4_preview_featured.jpg
   b) Solder two wires on each motors and note the polarity.

4) Insert Arduino Nano into bottom base of chassis
   https://cdn.thingiverse.com/renders/d4/6c/21/cd/e2/e45a019ef535a0f58a4e8c72f931e917_preview_featured.jpg

3) Build a ground bridge and a power bridge
    a) Take a 5 pin single row header and wrap a bare wire around  (see photo )
        https://cdn.thingiverse.com/renders/da/87/5c/29/3a/d2b5ca33bd970f64a6301fa75ae2eb22_preview_featured.jpg
    b) Solder the bare wire to the bottom of the single row header. (see photo)
        https://cdn.thingiverse.com/renders/cd/9c/f5/ed/67/d2b5ca33bd970f64a6301fa75ae2eb22_preview_featured.jpg

5) The wiring portion is highly subjective since you can use any GPIO's that you want and this is highly dependent 
on which software you are using for controling the NanoBot
