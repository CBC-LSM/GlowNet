# What is GlowNet?

GlowNet is a non-profit idea born out of a desire from other makers to develop a simple device that would go to protect people in volunerable areas, such as schools or churches.

After the shooting in Nashville in 2023, one item I picked up on watching the footage is that the children were locked down in the rooms. Unfortunately this was not 100% the case but it could have been much worse had the school not had some form of lock down procedure and early notification system. This device sets out to provide that to others. 

The basics of the design is a ESP32-POE device with a ring LED will mount to ceiling tile with the 3D printed pieces. The device will power and gain network communication via a POE switch/injector in the building infrastructure. Once the devices are on the network a main control unit (raspberry pi) will be installed on the network in a control room where someone is actively observing. The observe and notify in the event of an emergency.

Notifiation/Alert guide:
- Blue/1hz buzzer: Tornado
- Red/2hz Buzzer : Fire
- White/1hz Buzzer: Shelter In Place
- Green: All Clear


3D Models can be found here on [OnShape](https://cad.onshape.com/documents/83dc1b9d5794af9812a20a6e/w/6933c0ec732fa80879dfa10f/e/393d556f8704af149a6ea895)<br>
You can fork the design here and make your own version, or simply export out what has been designed

BOM:<br>
- (4) - [M3 Thread Inserts](https://a.co/d/j4Eid2X)
- (3) - [M2 Thread Inserts](https://a.co/d/)
- (2) - [M3x6 SHCS](https://a.co/d/eeJVUk0)
- (2) - [M3x10 FHCS](https://a.co/d/9gWt7kn)
- (3) - [1/4-20 x 2-1/2" Machine Screw](https://a.co/d/iGymJyz)
- (3) - [1/4" Fender Washer](https://a.co/d/4D97qNB)
- (3) - [1/4-20 Wing Nut](https://a.co/d/4D97qNB)
- (1) - [Adafruit 12 RGB LED](https://a.co/d/511ZWrK)
- (1) - [Piezo Buzzer](https://a.co/d/37FOyo1)
- (1) - [Olimex ESP32-POE](https://www.digikey.com/en/products/detail/olimex-ltd/ESP32-POE/10258717)


[GlowNet Fit Testing Video ](https://youtube.com/shorts/XqAFsF1926E?feature=share)<br>

![glownet 3d model](/img/glownet3d.png)<br>


How to turn lights on with API call - for init testing
http://<i>ipaddress</i>/api?profile=01 - will turn the lights yellow and turn on buzzer for 1 second
