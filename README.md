What is GlowNet?
After the shooting in Nashville, I set out to design and program an entire system that will use off the shelf components found in the maker industry and utilize 3D printing to develop a early action notification system for churches, schools, non-profits. The design is open source and when officially released will only benefit from the continued development of the maker community. This tool will easily install any class room using a single POE-ethernet connection. A on-prem or cloud server interface will communicate what the emergency state is. By visual and auditory cues the teacher/instructors will understand the best response. 

Example: In the event of a fire, you can delay evacuation until fire is confirmed and then you can use this system to indicate that evacuation is necessary.  Or if a tornado is approaching early warning services will send to a central office where  person can evaulate and signal for shelter in place.

3D Models can be found here on [OnShape](https://cad.onshape.com/documents/83dc1b9d5794af9812a20a6e/w/6933c0ec732fa80879dfa10f/e/393d556f8704af149a6ea895)<br><br>
You can fork the design here and make your own version, or simply export out what has been designed

BOM:<br>
(4) - [m3 Thread Inserts](https://a.co/d/j4Eid2X)<br>
(3) - [m2 Thread Inserts](https://a.co/d/)<br>
(2) - [m3x6 SHCS](https://a.co/d/eeJVUk0)<br>
(2) - [m3x10 FHCS](https://a.co/d/9gWt7kn)<br>
(3) - [1/4-20 x 2-1/2" Machine Screw](https://a.co/d/iGymJyz)<br>
(3) - [1/4" Fender Washer](https://a.co/d/4D97qNB)<br>
(3) - [1/4-20 Wing Nut](https://a.co/d/4D97qNB)<br>
(1) - [Adafruit 12 RGB LED](https://a.co/d/511ZWrK)<br>
(1) - [Piezo Buzzer](https://a.co/d/37FOyo1)<br>
(1) - [Olimex ESP32-POE](https://www.digikey.com/en/products/detail/olimex-ltd/ESP32-POE/10258717)<br>

 ![glownet 3d model](/img/glownet3d.png)<br>

How to turn lights on with API call - for init testing
http://<ip-address>/api?profile=01 - will turn the lights yellow and turn on buzzer for 1 second
