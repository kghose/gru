# Gru: Integrated heat controls

A set of devices that control heating and cooling in the house. 

Based on outdoor temperature, the system automatically selects whether to
activate the heating or cooling systems and, for heating, automatically selects
whether to run the heat pump or the baseboard heat.


# Boxen

[Gru](gru) - ESP32 based box that measures outdoor temperature, controls the
boiler relay and selects heating and cooling modes. 

[Kevin, Stuart, Bob and Mel](minions) - ESP32 based boxes that measure indoor
temperature and command mini-split heads via IR.

# Appendix: Design decisions

The design decisions were made to reduce confusion for family members as well as
scope the work so that it would be feasible for me to implement as a hobby
project. 


## Browser based control

I considered and rejected a design where each room's controller (i.e. Kevin,
Stuart, Bob and Mel) used a touch screen. This would have been user friendly and
very powerful, but it would been too time consuming for me to add a properly
usable UI on a touch screen connected to the ESP32.

I decided it would be easier to have old devices in the house (say a retired
iPad) run a browser and let the interface be a lightweight webpage: something I
am proficient at creating. This would also allow family members to use their
phones to control the system, though I would have to teach them to bookmark the
control page.

## Retention of existing baseboard and mini-split controls 

I designed the integrated controls to run in parallel to the existing separate
climate controls (wall thermostat for baseboard heat and IR remote controls for the
mini-splits). If the homemade integrated controls failed it should be a simple
process of just unplugging all the new stuff to go back to the tried and tested
old stuff (Also see "Why not use the CN105" below).


## Why not use the CN105

Many Mitsubishi mini-split heads have a serial interface plug (CN105) that 
allows control of all head functions. The plug can also supply power. This is 
the interface that Mitsubishi uses for its own WiFi control. The homeowner 
community has detailed instructions on how to use a [WeMos D1 mini board 
(ESP8266) to interface via the CN105 port][D1minicontrol] 

[D1minicontrol]: https://github.com/SwiCago/HeatPump/issues/13#issuecomment-457897457 

(Also see [this thread][findcn105])

[findcn105]: https://www.geekzone.co.nz/forums.asp?forumid=73&topicid=303958


I decided against this method:

1. It involves opening up each head and attaching a home-made device. If ever a 
   company had an excuse not to honor a warranty, it is this.
1. There is a real possibility that a malfunction on my home-made board could 
   damage the head circuitry. (e.g. [see this post][cuttlefish])
1. It might confuse a tech coming to service or fix the head.
1. It will likely confuse the rest of my family: they'll be trying to set the 
   mini-split with the remote and Dad's secret circuit keeps changing things 
from some hidden location and the only way to make it stop is to switch off the 
heat pump breaker and dismantle the mini-split heads. 

While the IR method is inelegant (instead of a dongle tucked away in the head, 
drawing power from it, there's this ugly box nailed to the wall and plugged into 
an outlet) and open-loop (there is no way to get the current state of the head) 
it is non-invasive and far less obscure.

It's much easier to explain to my family that this box on the wall is an 
automatic remote and if you ever think it's wonky, just unplug it.

[cuttlefish]: https://cuttlefishblacknet.wordpress.com/2019/05/31/mitsubishi-heatpump-2/