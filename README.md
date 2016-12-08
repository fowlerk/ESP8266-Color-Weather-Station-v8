
##Enhanced ESP8266 Color Weather Station (based on original by Daniel Eichorn (squix78) )

This project is a series of enhancements made to the original color weather station for the ESP8266 created by Daniel Eichorn, with contributions by several others.  Though the original display layout came from squix78, I've made substantial changes to add additional information.

This version of the project was written specifically for the WeMos Mini D1 with the 2.2" color LCD display based on the ILI9341 controller that squix78 used.

##  Weather Station Library

When I first began experimenting with squix78's original Weather Station, I quickly found that I wanted to add additional weather elements that were not in the existing library.  So, I dug through the existing code and found it quite easy to extend this to add the data elements I wanted.  So, fast-forward a few months and my modified library now contains a number of elements that may/may not be in the original.  Ideally these changes need to be merged back into the original, but I'm not quite sure the best way to go about this.  Some of the same elements have been added during the time I've been working on this version, but if you find you get errors in trying to use my enhanced version, I can either provide my enhancements (provided squix78 is okay with this of course), or I'll figure out how to get these merged into the original somehow.  Even if you elect to not use these additional elements, there is a lot of additional enhancements provided here that others may find useful.

## Weather Underground Timers

Since the free API from Weather Underground restricts the number of calls, I've modified the original code to implement a series of timers that control various portions of the data updates.  The current weather data and time are updated every 10 minutes by default.  The forecast data (9-days displayed in this version) is updated every 2 hours by default.  The astronomy data (for moon-phase) is updated twice per day by default.  All of these timers can easily be changed to your own preferences in the settings.h file.

## The Display

The display real-estate has been subdivided into several "panels" based on the original design by squix78.  The uppermost panel includes the date/time (including display of time-zone and "am/pm" if selected).  (BTW...I've modified the original code to have the time update every 15-seconds.)  Below this to the right I've added a new element to show the current WeatherUnderground city/personal weather station configured.  (I have several in my area and occasionally change this if my default is having issues.)

The next panel contains the current condition bitmap and temperature as per the original display.  To this I've added the "feels-like" temperature below (useful info when the wind-chill or heat-index becomes an issue).

The mid- and bottom-panels now contain rotating displays, whereas the top-two panels are "static" (other than for updates to the data).

The mid-panel rotates through 9-days of forecast information (predicted weather icon, high/low temperature).  I modified the original display for each day a bit to show both a day abbreviation and the month/day for clarity. 

The bottom-panel rotates through several panels, including the original panel by squix78 that shows the sun/moon rise/setting times and the current moon-phase icon.  I've added a detailed "current conditions" panel, showing humidity, windspeed/direction, precipitation since midnight, moon-phase (textual), barometric pressure, and UV index.  This panel also shows the date/time of the last update to this data.  Additional panels show the text forecast for each of the subsequent 12-hour forecast periods for the next 36 hours (including the current period).

The timing for how long the rotating displays appear before moving to the next are controlled by tickers (timers) and can easily be adjusted to your liking.  Also, adding additional displays to either of these panels is simply a matter of coding the display routine, changing the parameters for "...maxPanels" for either the mid/bot panels in setting.h, and extending the Switch statement in the main routine that calls the new display.  (See the existing code for a starting point.)

## WiFiManager

The original code by squix78 utilized the WiFiManager library by tzapu to set WiFi credentials via a configuration portal and connect to the AP.  I've extended this to add configuration parameters for the WeatherUnderground settings, as well as the city selected for the time-zone / daylight savings time rules.  Which brings me to DST...I've borrowed the same routine that neptune used in his enhancements to the Weather Station that sets DST rules based on the locale specified and updates the time via NTP.

*** Note:  In my haste to get these changes implemented, I only added a few time-zone city selections for the US timezones (including my own).  (Yes, go ahead and hit me up on my lack of consideration for all of you international contributors!)  It is simple to extend the code to add additional cities / DST rules by editing the function setDSTRules if you need to do so.

## Thingspeak / DHT22 Display

Since I don't currently use a local DHT22 attached to my Weather Station, nor upload readings to a Thingspeak channel, I've commented-out these code elements.  Feel free to un-comment if you're using them.

## Bug Fixes

Along the way I fixed a number of display and other bugs...for instance, several noted the overwrites occuring on the time occasionally.  I think I've taken care of this, along with some others.  I've also changed the counters / array sizes for some of the .BMP downloads to correspond with the number of bitmaps (at least the number currently available on squix78's site).  One particular nasty issue occurred with the array size for the moon-phase bitmaps; the routine was exceeding the array size during download and was causing occasional crashes of the module.

## Futures / TBD??

It might just be my setup, but occasionally the display on my TFT goes all-white.  I don't know if this is a cable issue, or perhaps the power requirements for the backlight are exceeding the rating of the output pins powering it from the WeMos.  Or...???

I also spent a considerable amount of time trying to implement a web-page pulldown for selecting the time-zone city, which never worked as I'd like.  (I could get the pulldown displayed, but couldn't get rid of the original input fields on the form.  This may be a restriction in the current WiFiManager routine, or maybe more likely a restriction with my limited HTML coding experience!



 

