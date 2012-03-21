# Triggertrap Source Code

Triggertrap is an open-source camera triggering platform based on Arduino. 

It comes in two versions: Triggertrap v1 (TTv1), which is a device aimed at consumers; and Triggertrap Shield for Arduino (TT-k) which is aimed at advanced users, hackers, and geeks (like us!). 

The codebase you are looking at is for both the above devices. 

_NOTE_ - there *are* differences between the TTv1 and TT-k. By default, the code ships for the Triggertrap Shield for Arduino (TT-k), because we anticipate that most people downloading and using this code will be using the Triggertrap Shield. If you wish to compile and install this code on your TTv1 device, remember to go into TTConfig.h, and read the section *Choose your version of the Triggertrap*. 

## Triggertrap v1 (TTv1)

The Triggertrap v1 is a battery-powered device with several sensors built-in:

* Laser sensor
* Ambient Light sensor
* Sound sensor

It also has a fully configurable AUX port, in the shape of a RCA socket, designed to accept additional external inputs and sensors. 

_Form Factor_ - The Triggertrap v1 comes in a perspex casing, and uses touch-sensitive buttons for control and operation. 

## Triggertrap Shield for Arduino (TT-k)

The Triggertrap Shield for Arduino is an Arduino-powered device with several sensors built-in:

* Laser sensor
* Ambient Light sensor
* Sound sensor

It also has a fully configurable AUX port, in the shape of a RCA socket, designed to accept additional external inputs and sensors. 

_Form Factor_ - The Triggertrap Shield for Arduino is an Arduino shield, and uses physical buttons for its interface. 

# Open Source & Licencing information 

This file is part of Triggertrap. See Triggertrap.com for more information.

Triggertrap is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 
Triggertrap is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Triggertrap. If not, see <http://www.gnu.org/licenses/>.

## Restrictions

The only thing we are keeping control over is the Triggertrap name and logo. Full details of the Triggertrap licence is available on http://triggertrap.com/license/

# Changenotes

## 0.3.31 (21 March 2012)

First open-source release of the Triggertrap codebase. 
