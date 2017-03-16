##  Enhanced ESP8266 Color Weather Station (based on original by Daniel Eichorn (squix78) )

This project is a series of enhancements made to the original color weather station for the ESP8266 created by Daniel Eichorn, with contributions by several others.  Though the original display layout came from squix78, I've made substantial changes to add additional information.

This version of the project was written specifically for the WeMos Mini D1 with the 2.2" color LCD display based on the ILI9341 controller that squix78 used.

##  Contributed Libraries (and links)

(*** Added 03/16/17 ***)

After several requests, I have added a summary of the contributed libraries with links below to make it easier to find and include these.  Most all of these come from the original examples on squix's site, but I admit that the list has become somewhat long and it took me some time to find all of these originally.

<FS.h>					Part of ESP8266 core, SPIFFS filesystem support

<Adafruit_GFX.h>   		Adafruit's core graphics library at <https://github.com/adafruit/Adafruit-GFX-Library>

<Adafruit_ILI9341.h> 	Adafruit's ILI9341 LCD hardware-specific library at <https://github.com/adafruit/Adafruit_ILI9341>

<WiFiManager.h>			WiFi Manager library by Tzapu at <https://github.com/tzapu/WiFiManager>

<ArduinoJson.h>         JSON library by Bblanchon at <https://github.com/bblanchon/ArduinoJson>

<JsonListener.h>		JSON streaming parser by Squix78 at <https://github.com/squix78/json-streaming-parser>

<WundergroundClient.h>	Weather Underground client library by Squix78 at <https://github.com/squix78/esp8266-weather-station>

<DoubleResetDetector.h> ESP8266 double-reset detector by Datacute at <https://github.com/datacute/DoubleResetDetector>

<simpleDSTadjust.h>		Daylight-savings time adjustment by Neptune at <https://github.com/neptune2/simpleDSTadjust>

"TimeZone.h"			Time zone conversion library by JChristensen at <https://github.com/JChristensen/Timezone>

(Note that though I included the SPIFFS library in the list above, I elected not to document all of the other core libraries.)

##  Weather Station Library

When I first began experimenting with squix78's original Weather Station, I quickly found that I wanted to add additional weather elements that were not in the existing library.  So, I dug through the existing code and found it quite easy to extend this to add the data elements I wanted.  So, fast-forward a few months and my modified library now contains a number of elements that may/may not be in the original.  Ideally these changes need to be merged back into the original, but I'm not quite sure the best way to go about this.  Some of the same elements have been added during the time I've been working on this version, but if you find you get errors in trying to use my enhanced version, I can either provide my enhancements (provided squix78 is okay with this of course), or I'll figure out how to get these merged into the original somehow.  Even if you elect to not use these additional elements, there is a lot of additional enhancements provided here that others may find useful.

(*** Update 12/10/16 ***) After several requests, I've uploaded my modified Weather Underground library components.  Though I don't recommend this as a long-term solution, you can use the two files contained in the folder "WU library" to replace those in squix78's library in your Arduino-libraries installation.  I'd hesistated to upload these because I didn't want to start another branch, but this may be an interim answer for those of you that want to try my enhancements before the changes can be merged into squix78's master.  

(*** Update 12/18/16 ***) My changes to the library have now been merged into the master, so this should no longer be an issue.  You should now be able to use the current version of the Weather Station library and compile my enhancement.

(*** Update 12/23/16 ***) New version available with significant additions to support display of weather alerts.  An indicator is displayed in a "panel" to the left of the date/time on the display if any active alerts exist for your selected Weather Underground city / PWS.  These alerts are described in detail on the Weather Underground site, and are invoked with the "/alerts" API call.  Note that these are only generally supported in the US and EU (through Meteoalarm) through this API.

The color of the indicator is set based on the severity of the alert (red for warning-level; yellow for watch-level; and cyan for general advisories / special weather statements).  In addition to the indicator panel, specific panels are displayed for each active alert, including start and expiration date/time, and details of the text of each alert (up to 512 characters, the current restriction of the JSON parsing library).  The indicator panel will show up to 6 concurrent alerts / advisories.  Please note that though the default update frequency for alerts is every 5 minutes (changeable), this feature should in no way be construed to be a replacement for a weather-alert radio, as in the event of severe weather warnings (such as tornado warnings), weather alert radios are activated on a "push" basis, whereas the alert display feature here is a "pull" API call (that may or may not succeed).  Though this may be obvious to most, I mention it for those where it may not be obvious.  It is useful as a general indicator, not as a safety feature!

For Meteoalarm (EU), an attribution panel is also displayed as required for the use of these alarms in the terms of use.

Further enhancements include the display of PoP (probability of precipitation) for each day of the extended forecast below and to the right of the forecast icon.

One other minor display bug on the sun/moon rise/setting times has been addressed in the Weather Underground library.  Previously, "00"'s were displayed as blanks; these should now be correctly formatted.

Finally, I've corrected a bug in the handling of the metric flag that caused several display elements to not be correctly shown in metric even when specified by setting the locale (time zone city) on the configuration portal.

I have uploaded the new version to the repository in the "Weather Alerts v9" folder for now.  This will likely replace my original version on the repository at some point in the near future.  New screenshot images have been added to the Images folder to show some of the latest enhancements.

(*** Update 01/03/17 ***) While the merging of my recent changes to the Weather Underground library to the main branch are pending, I have updated these library components (under the "WU library" folder) with the changes.  You can download these two files and replace those in your existing library folder if you want to compile and test v9 with the weather alerts; these should be backwardly compatible, that is, you should be able to compile any older weather display versions that utilize this library as I did not remove (or intentionally change!) any prior functionality.

I've also updated the main code to temporarily correct one additional (minor) bug I ran across.  Apparently a weather icon for the condition "chance of thunderstorms", or "chancetstorms" was omitted from the original icon set squix78 provided.  I noticed that a weather icon was not displayed when this condition was forecast.  As a quick fix for this, I modified the code to check for this condition and if detected, replace it with the icon for "thunderstorms"; there are only trivial differences between the two icons.  At some point when I get time to figure out how to create new icons I will try a more appropriate permanent fix.

## Weather Underground Timers

Since the free API from Weather Underground restricts the number of calls, I've modified the original code to implement a series of timers that control various portions of the data updates.  The current weather data and time are updated every 10 minutes by default.  The forecast data (9-days displayed in this version) is updated every 2 hours by default.  The astronomy data (for moon-phase) is updated twice per day by default.  All of these timers can easily be changed to your own preferences in the settings.h file.

(*** Update 12/23/16 ***) I've now moved the clock update setting to a variable on the settings.h header file to make it easier to locate / change if desired.

Note that I have carefully implemented the default timer frequency for updates to stay within the Weather Underground limit on (free) API calls to their service.  Though you can certainly change these, keep this restriction in mind or you may exceed the limit.  Currently, using the default update timers, the total calls / day are as follows:  (current observations every 10 minutes * 24 hours = (6 * 24), or 144) + (10-day forecasts every 2 hours, or 12) + (astronomy data twice each day, or 2) + (weather alerts updates every 5 minutes = 12 * 24 hours = (12 * 24), or 288) = 446 API calls daily.

## The Display

The display real-estate has been subdivided into several "panels" based on the original design by squix78.  The uppermost panel includes the date/time (including display of time-zone and "am/pm" if selected).  (BTW...I've modified the original code to have the time update every 15-seconds.)  Below this to the right I've added a new element to show the current WeatherUnderground city/personal weather station configured.  (I have several in my area and occasionally change this if my default is having issues.)

The next panel contains the current condition bitmap and temperature as per the original display.  To this I've added the "feels-like" temperature below (useful info when the wind-chill or heat-index becomes an issue).

The mid- and bottom-panels now contain rotating displays, whereas the top-two panels are "static" (other than for updates to the data).

The mid-panel rotates through 9-days of forecast information (predicted weather icon, high/low temperature).  I modified the original display for each day a bit to show both a day abbreviation and the month/day for clarity. 

The bottom-panel rotates through several panels, including the original panel by squix78 that shows the sun/moon rise/setting times and the current moon-phase icon.  I've added a detailed "current conditions" panel, showing humidity, windspeed/direction, precipitation since midnight, moon-phase (textual), barometric pressure, and UV index.  This panel also shows the date/time of the last update to this data.  Additional panels show the text forecast for each of the subsequent 12-hour forecast periods for the next 36 hours (including the current period).

The timing for how long the rotating displays appear before moving to the next are controlled by tickers (timers) and can easily be adjusted to your liking.  Also, adding additional displays to either of these panels is simply a matter of coding the display routine, changing the parameters for "...maxPanels" for either the mid/bot panels in setting.h, and extending the Switch statement in the main routine that calls the new display.  (See the existing code for a starting point.)

(*** Update 12/23/16 ***) See information above on new additions for weather alerts and PoP (probability of precipitation).

## WiFiManager

The original code by squix78 utilized the WiFiManager library by tzapu to set WiFi credentials via a configuration portal and connect to the AP.  I've extended this to add configuration parameters for the WeatherUnderground settings, as well as the city selected for the time-zone / daylight savings time rules.  Which brings me to DST...I've borrowed the same routine that neptune used in his enhancements to the Weather Station that sets DST rules based on the locale specified and updates the time via NTP.

*** Note:  In my haste to get these changes implemented, I only added a few time-zone city selections.  These include those for the US timezones (including my own), plus a few others (Zurich, Tokyo, and Sydney currently).  It is simple to extend the code to add additional cities / DST rules by editing the function setDSTRules if you need to do so.

### On-Demand Portal

This version utilizes a small library that allows bringing up the configuration portal on-demand by double-pressing the reset button on the WeMos.  This double-press must occur within 10 seconds (which is configurable and changeable in the code).  The library is by datacute (https://github.com/datacute/DoubleResetDetector).

*** The configuration portal is by default password-protected using the password of "portal#pass".  Change this to your own liking.

## Thingspeak / DHT22 Display

Since I don't currently use a local DHT22 attached to my Weather Station, nor upload readings to a Thingspeak channel, I've commented-out these code elements.  Feel free to un-comment if you're using them.

## Bug Fixes

Along the way I fixed a number of display and other bugs...for instance, several noted the overwrites occuring on the time occasionally.  I think I've taken care of this, along with some others.  I've also changed the counters / array sizes for some of the .BMP downloads to correspond with the number of bitmaps (at least the number currently available on squix78's site).  One particular nasty issue occurred with the array size for the moon-phase bitmaps; the routine was exceeding the array size during download and was causing occasional crashes of the module.

## Futures / TBD??

It might just be my setup, but occasionally the display on my TFT goes all-white.  I don't know if this is a cable issue, or perhaps the power requirements for the backlight are exceeding the rating of the output pins powering it from the WeMos.  Or...???

I also spent a considerable amount of time trying to implement a web-page pulldown for selecting the time-zone city, which never worked as I'd like.  (I could get the pulldown displayed, but couldn't get rid of the original input fields on the form.  This may be a restriction in the current WiFiManager routine, or maybe more likely a restriction with my limited HTML coding experience!)

I've noticed one additional remaining bug which I've not yet addressed.  It seems that if the moon-rise/set time is 0:00, only the ":" is displayed and nothing else.  The best place to correct this is probably in the Weather Station library itself instead of my code.

(*** Update 12/23/16 ***) I think this display bug should be squashed now.

 
