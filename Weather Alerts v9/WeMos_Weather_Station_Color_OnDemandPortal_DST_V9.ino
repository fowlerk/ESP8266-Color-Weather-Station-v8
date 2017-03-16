/**The MIT License (MIT)
Copyright (c) 2015 by Daniel Eichhorn
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
See more at http://blog.squix.ch

Modifed to include configuration portal on-demand when the WeMos reset button is pressed twice.
    Modified by DK Fowler ... 20-Nov-2016
    
    This method works well on Wemos boards which have a single reset button on board. It avoids using a pin for launching the configuration portal.

    How It Works
    When the ESP8266 loses power all data in RAM is lost but when it is reset the contents of a small region of RAM is preserved. So when the device 
    starts up it checks this region of ram for a flag to see if it has been recently reset. If so it launches a configuration portal, if not it sets 
    the reset flag. After running for a while this flag is cleared so that it will only launch the configuration portal in response to closely spaced resets.

    Settings
    There are two values to be set in the sketch.

    DRD_TIMEOUT - Number of seconds to wait for the second reset. Set to 10 in the example.
    DRD_ADDRESS - The address in RTC RAM to store the flag. This memory must not be used for other purposes in the same sketch. Set to 0 in the example.

    This example, contributed by DataCute needs the Double Reset Detector library from https://github.com/datacute/DoubleResetDetector .

Modified by DK Fowler ... 22-Nov-2016
    Added code to allow configuration of additional parameters, including Weather Underground parameters (API key, language, city/PWS,
    and country), and UTC offset for adjusting time from GMT.
    Note:  This version works with the exception of the time-setting routines.  The TimeClient class is instantiated with the UTC offset, which
    is an issue when the UTC offset is obtained within the SETUP function (vs. globally; the local scope causes issues when the time is obtained
    later on update).  Rather than resolve this, I've moved to another version that uses a different method and library to get the date/time,
    but more importantly, also adjusts for DST.

Modified by DK Fowler ... 30-Nov-2016
    Added a number of customizations made by Neptune, as per the latest example on Github for the WeatherStation.  This hopefully addresses
    two issues from the last version:  the global scope for the TimeClient class (no longer used), and the automatic adjustment for DST.
    Following are comments from this release made by Neptune (though not all of these changes have been added here).
 * Customizations by Neptune (NeptuneEng on Twitter, Neptune2 on Github)
 *  
 *  Added Wifi Splash screen and credit to Squix78
 *  Modified progress bar to a thicker and symmetrical shape
 *  Replaced TimeClient with built-in lwip sntp client (no need for external ntp client library)
 *  Added Daylight Saving Time Auto adjuster with DST rules using simpleDSTadjust library
 *  https://github.com/neptune2/simpleDSTadjust
 *  Added Setting examples for Boston, Zurich and Sydney
 *  Selectable NTP servers for each locale
 *  DST rules and timezone settings customizable for each locale
 *  See https://www.timeanddate.com/time/change/ for DST rules
 *  Added AM/PM or 24-hour option for each locale
 *  Changed to 7-segment Clock font from http://www.keshikan.net/fonts-e.html   (***Note: font not implemented for the ILI9341 TFT display here...DKF)
 *  Added Forecast screen for days 4-6 (requires 1.1.3 or later version of esp8266_Weather_Station library)
 *  Added support for DHT22, DHT21 and DHT11 Indoor Temperature and Humidity Sensors
 *  Fixed bug preventing display.flipScreenVertically() from working
 *  Slight adjustment to overlay

Modified by DK Fowler ... 01-Dec-2016
    This version implements timers (tickers) for setting when updates are required for the current
    weather, forecast, and astronomy information.  This allows setting different frequencies for 
    the updates, as the forecast data only needs updating every couple hours, and the astronomy
    data only a couple of times each day.  This helps to cut down on the number of calls to the
    API for WeatherUnderground, which is restricted for free use.

    Also implemented a timer for scrolling through the various mid-panel displays, alowing for 
    cycling through 9-day forecast information (vs. the original 3-days).

    Finally, implemented a timer for scrolling through various bottom-panel displays, allowing for
    display of current observation details and detailed current forecast text (future) along with
    astronomy data.

Modified by DK Fowler ... 04-Dec-2016
    Added significant additional information and tweaks to the display.  Added routines to display
    panels for the forecast text for the next 3 forecast periods (36 hours or so).  Added feels-like
    temperature and weather station to the display.  Display of the forecast text required implementing
    a word-wrap function to allow cleanly breaking the text on the display panel.

Modified by DK Fowler ... 08-Dec-2016
    Added custom parameter to the configuration screen for WiFiManager to allow designating city locale for
    setting time zone and DST rules.  Added custom labels for the new input fields on the form, including
    a new header file (TimeZone.h) that displays several lines of input help for this field.  Note that
    the only current legitimate options for this field are for the US time-zones, though this can easily
    be extended for international locales/rules.  Also cleaned up a couple of other display bugs, including
    overwrites on the text for the progress bar for .BMP downloads.

    Finally, corrected counts for the downloaded icons (currrent 19 specified for the weather icons, large
    and small; 23 for the moon-phase icons.)
 	
Modified by DK Fowler ... 12/15-Dec-2016
	Corrected errors with DST rules for Hawaii (DST not currently implemented), and Arizona (most of state 
	does not currently implement DST).  Corrected minor display overwrite issue with "am/pm".
	
Modified by DK Fowler ... 16-Dec-2016
	Added Japan/Tokyo time-zone.

Modified by DK Fowler ... 19-22-Dec-2016
  Added logic for enhancements to request active weather alerts.  This includes several new functions added
  to the Weather Underground library to obtain this information.

  Corrected several bugs from prior release, which included improperly setting the METRIC flag used for retrieving
  Weather Underground data.  This included adding a new function to the Weather Underground client library
  to allow initialization of this setting after instantiation of the client (after retrieving the settings from
  either the SPIFFS config file or the config web-page.  Added functions to the WU client library to allow 
  retrieving forecast details based on this setting (previously, only non-metric forecast details were returned
  regardless).

  Corrected a minor display formatting issue with sunrise/sunset, moonrise/moonset in the WU client library
  which previously returned blanks for the hours/minutes when these were "0".

  Added additional functionality to the display word-wrap routine that allows breaking long text displays either
  at a blank or ellipses (...); these were frequently returned in the weather alerts data just added.  This
  function also now returns the number of display panels for the display string processed (useful for determining 
  additional processing that may need to be performed as a result of multiple display panels).

  Added probability of precipitation (PoP) to both WU client library and to the display of the forecast panels.

Modified by DK Fowler ... 02-Jan-2017
  Temporarily corrected minor bug with missing icon for "chancetstorms".  Added code to check for this condition and
  replace the icon with the one for "tstorms" as a temporary workaround until the correct icon is loaded.

Modified by DK Fowler ... -5-Jan-2017
  Minor tweak to the last observation date/time to prevent it from being partially truncated at bottom of the screen.
    
*/
#include <FS.h>
#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library
#include <SPI.h>
// Additional UI functions
#include "GfxUi.h"

// Fonts created by http://oleddisplay.squix.ch/
#include "ArialRoundedMTBold_14.h"
#include "ArialRoundedMTBold_36.h"
#include "Droid_Sans_10.h"
#include "Droid_Sans_Bold_10.h"
#include "Droid_Sans_12.h"
//#include "DSEG7Classic-BoldFont.h"

// Download helper
#include "WebResource.h"

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Helps with connecting to internet
#include <WiFiManager.h>

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

// check settings.h for adapting to your needs
#include "settings.h"
#include <JsonListener.h>
#include <WundergroundClient.h>
#include "time.h"
#include <DoubleResetDetector.h>  //https://github.com/datacute/DoubleResetDetector
#include <simpleDSTadjust.h>
#include "TimeZone.h"

/*********************************************************
 * Important: see settings.h to configure your settings!!!
 * *******************************************************/

DoubleResetDetector drd(DRD_TIMEOUT, DRD_ADDRESS);

// Debug for panel updates
int lastPanelMillis = 0;

//flag for saving data
bool shouldSaveConfig = false;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
GfxUi ui = GfxUi(&tft);

// flag changed in the ticker function every 1 minute
bool readyForDHTUpdate = false;

WebResource webResource;

// Indicates whether ESP has WiFi credentials saved from previous session, or double reset detected
bool initialConfig = false;

// Set to false, if you prefere imperial/inches, Fahrenheit
WundergroundClient wunderground(isMetric);

// Initialize the temperature/ humidity sensor
DHT dht(DHTPIN, DHTTYPE);
float humidity = 0.0;
float temperature = 0.0;

// Instantiate tickers for updating current conditions, forecast, and astronomy info
volatile int forecastTimer  = 0;  // Part of workaround for ticker timer limit
volatile int astronomyTimer = 0;
volatile int alertsTimer = 0;
Ticker ticker;
Ticker forecastTicker;
Ticker astronomyTicker;
Ticker alertsTicker;
Ticker midPanelTicker;
Ticker botPanelTicker;


//declaring prototypes
void configModeCallback (WiFiManager *myWiFiManager);
void saveConfigCallback ();
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal);
ProgressCallback _downloadCallback = downloadCallback;
void downloadResources();
void updateData();
void setDSTRules(String TZCity);
void updateForecastData();
void updateAstronomyData();
void updateAlertsData();
void updateMidPanel();
void updateBotPanel();
void drawProgress(uint8_t percentage, String text);
void drawDateTime();
void drawCurrentWeather();
void drawCurrentObservations();
void drawAlerts();
void drawAlertsPanels();
void drawAlertMsg(int alertIndex);
void drawForecast();
void drawForecast2();
void drawForecast3();
void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex);
String getMeteoconIcon(String iconText);
void drawAstronomy();
void drawForecastText(int forecastTextPeriod);
void drawSeparator(uint16_t y);
void setReadyForWeatherUpdate();
void setReadyForForecastUpdate();
void setReadyForAstronomyUpdate();
void setReadyForAlertsUpdate();
void setReadyForMidPanelUpdate();
void setReadyForBotPanelUpdate();
byte stringWordWrap(String displayString);

long lastDownloadUpdate = millis();

void setup() {
  Serial.begin(115200);

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter custom_WUNDERGROUND_API_KEY("WUkey", "WU API key", WUNDERGROUND_API_KEY, 18, "<p>Weather Underground API Key</p");
    WiFiManagerParameter custom_WUNDERGROUND_LANGUAGE("WUlanguage", "WU language", WUNDERGROUND_LANGUAGE, 4, "<p>Weather Underground Language</p");
    WiFiManagerParameter custom_WUNDERGROUND_COUNTRY("WUcountry", "WU country", WUNDERGROUND_COUNTRY, 4, "<p>Weather Underground Country</p");
    WiFiManagerParameter custom_WUNDERGROUND_CITY("WUcity", "WU city/PWS", WUNDERGROUND_CITY, 20, "<p>Weather Underground City/PWS</p");
    WiFiManagerParameter custom_TZ_CITY("TZCity", "TZ City", TZ_CITY, 30, TimeZoneConfig);
  
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    // Uncomment following lines to delete the config.json file...
    // if (SPIFFS.remove("/config.json")) {
    //   Serial.println("***Successfully deleted config.json file...***");
    // } else {
    //   Serial.println("***...error deleting config.json file...***");
    // }
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(WUNDERGROUND_API_KEY, json["WUNDERGROUND_API_KEY"]);
          strcpy(WUNDERGROUND_LANGUAGE, json["WUNDERGROUND_LANGUAGE"]);
          strcpy(WUNDERGROUND_COUNTRY, json["WUNDERGROUND_COUNTRY"]);
          strcpy(WUNDERGROUND_CITY, json["WUNDERGROUND_CITY"]);
          strcpy(TZ_CITY, json["TZ_CITY"]);
          Serial.println("...finished...");
        } else {
          Serial.println("failed to load json config");
        }
      }
    } else {
      Serial.println("/config.json does not exist.");
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read
  
  tft.begin();
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
//  ui.drawString(120, 160, "Connecting to WiFi");

  String hostname;
  hostname = "ESP" + String(ESP.getChipId(), HEX);
//  hostname.remove(3,1);
  Serial.print("Hostname:  "); Serial.println(hostname);
  
  if (WiFi.SSID()==""){
    Serial.println("No stored access-point credentials; initiating configuration portal.");   
    tft.fillScreen(ILI9341_BLACK);
    ui.drawString(120, 56, "No stored access-point credentials...");
    ui.drawString(120, 70, "Initiating configuration portal.");
    delay(1000);
    initialConfig = true;
  }
  if (drd.detectDoubleReset()) {
    Serial.println("Double-reset detected...");
    tft.fillScreen(ILI9341_BLACK);
    ui.drawString(120, 56, "Double-reset detected...");
    ui.drawString(120, 70, "Initiating configuration portal.");
    delay(1000);
    initialConfig = true;
  }
  if (initialConfig) {
    Serial.println("Starting configuration portal.");

    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;

    //set config save notify callback
    wifiManager.setSaveConfigCallback(saveConfigCallback);

// Uncomment for testing wifi manager
//  wifiManager.resetSettings();
    wifiManager.setAPCallback(configModeCallback);

    //add all your parameters here
    wifiManager.addParameter(&custom_WUNDERGROUND_API_KEY);
    wifiManager.addParameter(&custom_WUNDERGROUND_LANGUAGE);
    wifiManager.addParameter(&custom_WUNDERGROUND_COUNTRY);
    wifiManager.addParameter(&custom_WUNDERGROUND_CITY);
    wifiManager.addParameter(&custom_TZ_CITY);

    //it starts an access point 
    //and goes into a blocking loop awaiting configuration
    
    // Serial.print("Portal: "); Serial.print(hostname); Serial.print("  pass:  "); Serial.println(configPortalPassword);
    if (!wifiManager.startConfigPortal(hostname.c_str(), configPortalPassword)) {
      Serial.println("Not connected to WiFi but continuing anyway.");
    } else {
      //if you get here you have connected to the WiFi
      Serial.println("Connected to WiFi.");

        //read updated parameters
      strcpy(WUNDERGROUND_API_KEY, custom_WUNDERGROUND_API_KEY.getValue());
      strcpy(WUNDERGROUND_LANGUAGE, custom_WUNDERGROUND_LANGUAGE.getValue());
      strcpy(WUNDERGROUND_COUNTRY, custom_WUNDERGROUND_COUNTRY.getValue());
      strcpy(WUNDERGROUND_CITY, custom_WUNDERGROUND_CITY.getValue());
      strcpy(TZ_CITY, custom_TZ_CITY.getValue());

  //save the custom parameters to FS
      if (shouldSaveConfig) {
        Serial.println("saving config");
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.createObject();
        json["WUNDERGROUND_API_KEY"] = WUNDERGROUND_API_KEY;
        json["WUNDERGROUND_LANGUAGE"] = WUNDERGROUND_LANGUAGE;
        json["WUNDERGROUND_COUNTRY"] = WUNDERGROUND_COUNTRY;
        json["WUNDERGROUND_CITY"] = WUNDERGROUND_CITY;
        json["TZ_CITY"] = TZ_CITY;

        Serial.print("TZ City:  "); Serial.println(TZ_CITY);

        File configFile = SPIFFS.open("/config.json", "w");
        if (!configFile) {
          Serial.println("failed to open config file for writing");
        }

        json.printTo(Serial);
        json.printTo(configFile);
        Serial.println("fnished saving configuration file, closing");
        configFile.close();
      //end save

      }

    }
    ESP.reset(); // This is a bit crude. For some unknown reason webserver can only be started once per boot up 
                 // so resetting the device allows to go back into config mode again when it reboots.
    delay(5000);
  }

  WiFi.mode(WIFI_STA); // Force to station mode because if device was switched off while in access point mode it will start up next time in access point mode.
  unsigned long startedAt = millis();
  Serial.print("After waiting ");
  tft.fillScreen(ILI9341_BLACK);
  ui.drawString(120, 160, "Connecting to WiFi");

  int connRes = WiFi.waitForConnectResult();
  float waited = (millis()- startedAt);
  Serial.print(waited/1000);
  Serial.print(" secs in setup() connection result is ");
  Serial.println(connRes);
  if (WiFi.status()!=WL_CONNECTED){
    Serial.println("Failed to connect, finishing setup anyway.");
  } else {
    Serial.print("Connected...local ip: ");
  
    Serial.println(WiFi.localIP());
    tft.fillScreen(ILI9341_BLACK);
    ui.drawString(120, 160, "Connected...");
    delay(1000);
  }
  
  // OTA Setup
  ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();
//  SPIFFS.begin();
  
  //Uncomment if you want to update all internet resources
  //SPIFFS.format();

  // download images from the net. If images already exist don't download
  downloadResources();

//  Debug alerts...
//  strcpy(TZ_CITY, "Mountain");
//  strcpy(WUNDERGROUND_CITY,"pws:KNDELLEN3");  // for testing weather alerts processing
//  strcpy(WUNDERGROUND_COUNTRY,"NO");
//  strcpy(WUNDERGROUND_CITY,"Svene");          // for testing weather alerts processing
  
  // set the start/end rules for Daylight Savings Time based on the city selected
  setDSTRules(TZ_CITY);
  
  // load the weather information
  updateData();               // Current observations
  updateForecastData();       // 10-day forecast data
  updateAstronomyData();      // Moon phase, etc.
  updateAlertsData();         // Active weather alerts
  
  ticker.attach(UPDATE_INTERVAL_SECS, setReadyForWeatherUpdate);            // Now for current observations update only
  midPanelTicker.attach(UPDATE_MID_PANEL_INTERVAL_SECS, setReadyForMidPanelUpdate); // To trigger updating mid-panel on screen
  botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen

//  Note the triggering times (60 secs) for each of the following tickers.  This is a workaround for the
//  maximum timer limitation for the ticker function (approximately 71 minutes).
  forecastTicker.attach(60, setReadyForForecastUpdate);           // For controlling 10-day forecast update
  astronomyTicker.attach(60, setReadyForAstronomyUpdate);         // For controlling astronomy data update
  alertsTicker.attach(60, setReadyForAlertsUpdate);               // For controlling weather alerts data update
  
}

long lastDrew = 0;
void loop() {
  // Handle OTA update requests
  ArduinoOTA.handle();

  // Call the double reset detector loop method every so often,
  // so that it can recognise when the timeout expires.
  // You can also call drd.stop() when you wish to no longer
  // consider the next reset as a double reset.
  drd.loop();

  // Check if we should update the clock
//  if (millis() - lastDrew > 30000 && wunderground.getSeconds() == "00") {
  if (millis() - lastDrew > (UPDATE_CLOCK_SECS * 1000)) {
    drawDateTime();
    lastDrew = millis();
  }

  // Check if we should update weather information
//  if (millis() - lastDownloadUpdate > 1000 * UPDATE_INTERVAL_SECS) {
  if (readyForWeatherUpdate) {
      updateData();
//      lastDownloadUpdate = millis();
  }
  if (readyForForecastUpdate) {
    updateForecastData();
  }

  if (readyForAstronomyUpdate) {
    updateAstronomyData();
  }
  
  if (readyForAlertsUpdate) {
    updateAlertsData();
  }

  if (readyForMidPanelUpdate) {
    updateMidPanel();
  }

  if (readyForBotPanelUpdate) {
    updateBotPanel();
  }

}

// Called if WiFi has not been configured yet
void configModeCallback (WiFiManager *myWiFiManager) {
  ui.setTextAlignment(CENTER);
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  tft.setTextColor(ILI9341_ORANGE);
  ui.drawString(120, 28, "Wifi Manager");
  ui.drawString(120, 42, "Please connect to AP");
  tft.setTextColor(ILI9341_WHITE);
  ui.drawString(120, 56, myWiFiManager->getConfigPortalSSID());
  // ui.drawString(120, 56, WiFi.hostname());
  tft.setTextColor(ILI9341_ORANGE);
  ui.drawString(120, 70, "To setup Wifi Configuration");
  tft.setFont(&Droid_Sans_12);
  tft.setTextColor(ILI9341_CYAN);
  ui.drawString(120, 98,  "If configuration page does not");
  ui.drawString(120, 116, "appear automatically, connect to");
  ui.drawString(120, 134, "address 192.168.4.1 in browser.");
}

// callback called during download of files. Updates progress bar
void downloadCallback(String filename, int16_t bytesDownloaded, int16_t bytesTotal) {
  Serial.println(String(bytesDownloaded) + " / " + String(bytesTotal));

  int percentage = 100 * bytesDownloaded / bytesTotal;
  if (percentage == 0) {
    ui.drawString(120, 160, filename);
  }
  if (percentage % 5 == 0) {
    ui.setTextAlignment(CENTER);
    ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
    //ui.drawString(120, 160, String(percentage) + "%");
    ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
  }

}

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

// Download the bitmaps
void downloadResources() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  char id[5];
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/" + wundergroundIcons[i] + ".bmp", wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 19; i++) {
    sprintf(id, "%02d", i);
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/wunderground/mini/" + wundergroundIcons[i] + ".bmp", "/mini/" + wundergroundIcons[i] + ".bmp", _downloadCallback);
  }
  for (int i = 0; i < 23; i++) {
    tft.fillRect(0, 120, 240, 45, ILI9341_BLACK);
    webResource.downloadFile("http://www.squix.org/blog/moonphase_L" + String(i) + ".bmp", "/moon" + String(i) + ".bmp", _downloadCallback);
  }
}

// Update the internet based information and update screen
void updateData() {
//  tft.fillScreen(ILI9341_BLACK);        // This seems to sometimes leave the screen in an "all-bright" condition??  Maybe a power issue supplying TFT backlight??
  tft.setFont(&ArialRoundedMTBold_14);
  drawProgress(33, "Updating time...");
  delay(500);
  configTime(UTC_OFFSET * 3600, 0, NTP_SERVERS);
  drawProgress(66, "Updating conditions...");
  wunderground.updateConditions(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
//  tft.fillScreen(ILI9341_BLACK);              // This seems to sometimes leave the screen in an "all-bright" condition??
  drawDateTime();
  tft.fillRect(0, 63, 240, 63, ILI9341_BLACK);  // Wipe prior data in the "current weather" panel
  drawCurrentWeather();
  readyForMidPanelUpdate = true;                // Update mid-panel since progress bar overwrites this.
  readyForWeatherUpdate = false;
}

void updateMidPanel() {
  midPanelNumber++;
  if (midPanelNumber > maxMidPanels) midPanelNumber = 1;
  switch (midPanelNumber) {
    case 1:
      drawForecast();
      break;
    case 2:
      drawForecast2();
      break;
    case 3:
      drawForecast3();
      break;
  }
  readyForMidPanelUpdate = false;
}

void updateBotPanel() {
  botPanelNumber++;
  if (botPanelNumber > maxBotPanels) botPanelNumber = 1;
  // Serial.print("*** Bottom panel update: "); Serial.print(millis()-lastPanelMillis); Serial.print(" ms; panel number: "); Serial.println(botPanelNumber);
  switch (botPanelNumber) {
    case 1:
      drawAstronomy();
      break;
    case 2:
      drawCurrentObservations();
      break;
    case 3:
      activeAlerts = wunderground.getActiveAlertsCnt();
      if (activeAlerts != 0) {
        drawAlerts();
        drawAlertsPanels();
        skipAlertUpdate = false;
      } else {
        skipAlertUpdate = true;         // If no alerts, do an immediate panel update to move to next panel
        readyForBotPanelUpdate = true;
      }
      break;
    case 4:
      drawForecastText(0);
      break;
    case 5:
      drawForecastText(1);
      break;
    case 6:
      drawForecastText(2);
      break;
  }
  if (skipAlertUpdate) {
    skipAlertUpdate = false;        // reset for next display cycle
  } else {
    readyForBotPanelUpdate = false;      
  }
  lastPanelMillis = millis();
}

void updateForecastData() {
  drawProgress(50, "Updating forecasts...");
  wunderground.updateForecast(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
  readyForMidPanelUpdate = true;       // Update mid-panel since progress bar overwrites this.
  readyForForecastUpdate = false;
}

void updateAstronomyData() {
  drawProgress(50, "Updating astronomy...");
  wunderground.updateAstronomy(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
  readyForMidPanelUpdate = true;       // Update mid-panel since progress bar overwrites this.
  readyForBotPanelUpdate = true;
  readyForAstronomyUpdate = false;
}

void updateAlertsData() {
  drawProgress(50, "Updating weather alerts...");
  wunderground.updateAlerts(WUNDERGROUND_API_KEY, WUNDERGROUND_LANGUAGE, WUNDERGROUND_COUNTRY, WUNDERGROUND_CITY);
  drawProgress(100, "Done...");
  delay(1000);
  drawAlerts();
  readyForMidPanelUpdate = true;       // Update mid-panel since progress bar overwrites this.
  readyForBotPanelUpdate = true;
  readyForAlertsUpdate = false;
}

// Progress bar helper
void drawProgress(uint8_t percentage, String text) {
  tft.setFont(&ArialRoundedMTBold_14); 
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  tft.fillRect(0, 140, 240, 45, ILI9341_BLACK);
  ui.drawString(120, 160, text);
  ui.drawProgressBar(10, 165, 240 - 20, 15, percentage, ILI9341_WHITE, ILI9341_BLUE);
}

// Called every 1 minute
void updateDHT() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature(!isMetric);
  readyForDHTUpdate = false;
}

void setDSTRules(String TZ_CITY) {
  if (TZ_CITY == "Boston") {
        UTC_OFFSET = -5;
        StartRule = (dstRule) {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
        EndRule = (dstRule) {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Louisville") {
        UTC_OFFSET = -5;
        StartRule = (dstRule) {"EDT", Second, Sun, Mar, 2, 3600}; // Eastern Daylight time = UTC/GMT -4 hours
        EndRule = (dstRule) {"EST", First, Sun, Nov, 1, 0};       // Eastern Standard time = UTC/GMT -5 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Chicago") {
        UTC_OFFSET = -6;
        StartRule = (dstRule) {"CDT", Second, Sun, Mar, 2, 3600}; // Central Daylight time = UTC/GMT -5 hours
        EndRule = (dstRule) {"CST", First, Sun, Nov, 1, 0};       // Central Standard time = UTC/GMT -6 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Mountain") {
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MDT", Second, Sun, Mar, 2, 3600}; // Mountain Daylight time = UTC/GMT -6 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};       // Mountain Standard time = UTC/GMT -7 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Arizona-DST") {		// Same as Mountain
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MDT", Second, Sun, Mar, 2, 3600}; // Mountain Daylight time = UTC/GMT -6 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};		    // Mountain Standard time = UTC/GMT -7 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Arizona-noDST") {		// Most of Arizona doesn't currently implement daylight saving time
        UTC_OFFSET = -7;
        StartRule = (dstRule) {"MST", First, Sun, Nov, 1, 0}; 	  // Mountain Standard time = UTC/GMT -7 hours
        EndRule = (dstRule) {"MST", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Pacific") {
        UTC_OFFSET = -8;
        StartRule = (dstRule) {"PDT", Second, Sun, Mar, 2, 3600}; // Pacific Daylight time = UTC/GMT -7 hours
        EndRule = (dstRule) {"PST", First, Sun, Nov, 1, 0};       // Pacific Standard time = UTC/GMT -8 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Alaska") {
        UTC_OFFSET = -9;
        StartRule = (dstRule) {"ADT", Second, Sun, Mar, 2, 3600}; // Alaskan Daylight time = UTC/GMT -8 hours
        EndRule = (dstRule) {"AST", First, Sun, Nov, 1, 0};       // Alaskan Standard time = UTC/GMT -9 hours
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Hawaii") {								                // Hawaii doesn't currently implement daylight saving time
        UTC_OFFSET = -10;
        StartRule = (dstRule) {"HST", First, Sun, Nov, 1, 0}; 	  // Hawaiian Standard time = UTC/GMT -10 hours
        EndRule = (dstRule) {"HST", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  } else if (TZ_CITY == "Zurich") {
        UTC_OFFSET = +1;
        StartRule = (dstRule) {"CEST", Last, Sun, Mar, 2, 3600};  // Central European Summer Time = UTC/GMT +2 hours
        EndRule = (dstRule) {"CET", Last, Sun, Oct, 2, 0};        // Central European Time = UTC/GMT +1 hour
        #define NTP_SERVERS "0.ch.pool.ntp.org", "1.ch.pool.ntp.org", "2.ch.pool.ntp.org"
        isMetric = true;
  } else if (TZ_CITY == "Tokyo") {								                // Japan doesn't implement daylight saving time
		    UTC_OFFSET = +9;
		    StartRule = (dstRule) {"JST", First, Sun, Nov, 1, 0}; 	  // Japan Standard time = UTC/GMT +9 hours
		    EndRule = (dstRule) {"JST", First, Sun, Nov, 1, 0}; 	    // NO DST
		    #define NTP_SERVERS "3.jp.pool.ntp.org", "0.jp.pool.ntp.org", "2.jp.pool.ntp.org"
        isMetric = true;
  } else if (TZ_CITY == "Sydney") {
        UTC_OFFSET = +10;
        StartRule = (dstRule) {"AEDT", First, Sun, Oct, 2, 3600}; // Australia Eastern Daylight time = UTC/GMT +11 hours
        EndRule = (dstRule) {"AEST", First, Sun, Apr, 2, 0};      // Australia Eastern Standard time = UTC/GMT +10 hours
        #define NTP_SERVERS "0.au.pool.ntp.org", "1.au.pool.ntp.org", "2.au.pool.ntp.org"
        isMetric = true;
  } else {						// No DST support
        UTC_OFFSET = 0;
        StartRule = (dstRule) {"GMT", First, Sun, Nov, 1, 0};     // GMT default
        EndRule = (dstRule) {"GMT", First, Sun, Nov, 1, 0};
        #define NTP_SERVERS "us.pool.ntp.org", "time.nist.gov", "pool.ntp.org"
        isMetric = false;
  }
  wunderground.initMetric(isMetric);      // (re-)initialize the metric settings for the Weather Underground library
}

void drawDateTime() {
  // Setup simpleDSTadjust Library rules
  simpleDSTadjust dstAdjusted(StartRule, EndRule);

  char *dstAbbrev;
  char time_str[11];
  time_t now = dstAdjusted.time(&dstAbbrev);
  struct tm * timeinfo = localtime (&now);

//  tft.drawRect(40, 0, 200, 28, ILI9341_WHITE);
  tft.fillRect(40, 0, 200, 28, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);
  String date = ctime(&now);
  date = date.substring(0,11) + String(1900+timeinfo->tm_year);
  ui.drawString(120, 20, date);

//  tft.setFont(&DSEG7_Classic_Bold_21);
  tft.setFont(&ArialRoundedMTBold_36);
  
#ifdef STYLE_24HR
  sprintf(time_str, "%02d:%02d:%02d\n",timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
  // tft.drawRect(40, 26, 200, 36, ILI9341_WHITE);
  tft.fillRect(40, 26, 200, 36, ILI9341_BLACK);
  ui.drawString(120, 56, time_str);
  drawSeparator(65);
#else
  int hour = (timeinfo->tm_hour+11)%12+1;  // take care of noon and midnight
  sprintf(time_str, "%2d:%02d:%02d\n", hour, timeinfo->tm_min, timeinfo->tm_sec);
  // tft.drawRect(40, 26, 200, 36, ILI9341_WHITE);
  tft.fillRect(40, 26, 200, 36, ILI9341_BLACK);
  ui.drawString(120, 56, time_str);
  drawSeparator(65);
#endif

//  ui.setTextAlignment(RIGHT);
  ui.setTextAlignment(LEFT);
//  ui.setFont(&ArialMT_Plain_10);
  tft.setFont(&ArialRoundedMTBold_14);
#ifdef STYLE_24HR
  sprintf(time_str, "%s", dstAbbrev);
  ui.drawString(202, 40, time_str);
#else
  sprintf(time_str, "%s", dstAbbrev);
  ui.drawString(202, 40, time_str);
  sprintf(time_str, "%s", timeinfo->tm_hour>=12?"pm":"am");
  ui.drawString(202, 56, time_str);
#endif

// Weather Station
ui.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
ui.setTextAlignment(RIGHT);
tft.setFont(&Droid_Sans_10);
String temp = "PWS: " + String(WUNDERGROUND_CITY);
ui.drawString(220, 75, temp);

}

// draws current weather information
void drawCurrentWeather() {
  // Weather Icon
  String weatherIcon = getMeteoconIcon(wunderground.getTodayIcon());
  ui.drawBmp(weatherIcon + ".bmp", 0, 55);
  
  // Weather Text
  tft.setFont(&ArialRoundedMTBold_14);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  ui.setTextAlignment(RIGHT);
  ui.drawString(220, 90, wunderground.getWeatherText());

  // Temperature display
  tft.setFont(&ArialRoundedMTBold_36);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  ui.setTextAlignment(RIGHT);
  String degreeSign = "F";
  if (isMetric) {
    degreeSign = "C";
  }
  String temp = wunderground.getCurrentTemp() + degreeSign;
  ui.drawString(220, 125, temp);
  drawSeparator(135);

  // Feels-like temperature display
  ui.setTextAlignment(RIGHT);
  tft.setFont(&Droid_Sans_10);
  temp = "Feels like: " + wunderground.getFeelsLike();
  ui.drawString(220, 138, temp);

}

// Draw current observations panel.
void drawCurrentObservations () {
  // In order to make it easy to adjust the position of this
  // display, the following x/y coordinates set the baseline
  // position for the remaining display lines.
  int x = 0;
  int y = 243;
  // The following control the current line/column/
  // font height and column width.  Adjust as desired
  // based on font used and column appearance.
  int fontHeight = 12;
  int fontWidth = 5;      // used to set position of dynamic information after labels
  int maxLabelWidth = 9;  // used as basedline for width of static lables
  int columnWidth = 120;
  float lineNumber = 1;
  int columnNumber = 1;
  
  tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);
  ui.setTextAlignment(CENTER);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(&Droid_Sans_10);
  String temp = "Current Observations";
  ui.drawString(120, y, temp);
  lineNumber = lineNumber + 1.5;    // add blank line after heading

  ui.setTextAlignment(LEFT);
  temp = "Humid:  ";
  temp = temp;
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  temp = wunderground.getHumidity();
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  String windString;
  if (wunderground.getWindSpeed() == "0.0") {
    windString = "Calm";
  } else {
    windString = wunderground.getWindDir() + " " + wunderground.getWindSpeed(); 
  }
  temp = "Wind:   ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  temp = windString;
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  temp = "Precip: ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  temp = wunderground.getPrecipitationToday();
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  temp = "Moon:   ";
  ui.drawString(x+(columnNumber-1), y+(fontHeight*(lineNumber-1)), temp);
  temp = wunderground.getMoonPhase();
  ui.drawString(x+(columnNumber-1)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

// Column 2...

  columnNumber++;
  lineNumber = 2.5;     // reset for second column

  temp = "Press: ";
  ui.drawString(x+((columnNumber-1)*columnWidth), y+(fontHeight*(lineNumber-1)), temp);
  temp = wunderground.getPressure();
  ui.drawString(x+((columnNumber-1)*columnWidth)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  temp = "UV:    ";
  ui.drawString(x+((columnNumber-1)*columnWidth), y+(fontHeight*(lineNumber-1)), temp);
  temp = wunderground.getUV();
  ui.drawString(x+((columnNumber-1)*columnWidth)+(maxLabelWidth*fontWidth), y+(fontHeight*(lineNumber-1)), temp);
  lineNumber++;

  temp = wunderground.getObservationTime();
  ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.setFont(&Droid_Sans_10);
  ui.setTextAlignment(CENTER);
  temp = temp.substring(5);
  ui.drawString(120, 312, temp);

}

// draws the three forecast columns
void drawForecast() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect 
//  tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);  // Frames the entire forecast area
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);    // Clear prior forecast panel
  drawForecastDetail(10, 165, 0);
  drawForecastDetail(95, 165, 2);
  drawForecastDetail(178, 165, 4);
  drawSeparator(165 + 65 + 10);
}

void drawForecast2() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect 
//  tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 6);
  drawForecastDetail(95, 165, 8);
  drawForecastDetail(178, 165, 10);
  drawSeparator(165 + 65 + 10);
}

void drawForecast3() {
//  tft.drawRect(0, 165, 240, 50, ILI9341_WHITE);  // May leave this for future; adds a nice effect
//  tft.drawRect(0, 150, 240, 82, ILI9341_WHITE);
  tft.fillRect(0, 150, 240, 82, ILI9341_BLACK);   // Clear prior forecast panel

  drawForecastDetail(10, 165, 12);
  drawForecastDetail(95, 165, 14);
  drawForecastDetail(178, 165, 16);
  drawSeparator(165 + 65 + 10);
}

// helper for the forecast columns
void drawForecastDetail(uint16_t x, uint16_t y, uint8_t dayIndex) {
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
//  tft.setFont(&ArialRoundedMTBold_14);
  tft.setFont(&Droid_Sans_12);
  ui.setTextAlignment(CENTER);
  String day = wunderground.getForecastTitle(dayIndex).substring(0, 3);
  day.toUpperCase();
  String forecastDate = wunderground.getForecastMonth(dayIndex/2) + "/" + 
                        wunderground.getForecastDay(dayIndex/2);
  String dayDate = day + "-" + forecastDate;
  ui.drawString(x + 25, y, dayDate);

  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.drawString(x + 25, y + 14, wunderground.getForecastLowTemp(dayIndex) + "|" + wunderground.getForecastHighTemp(dayIndex));
  
  String weatherIcon = getMeteoconIcon(wunderground.getForecastIcon(dayIndex));
  Serial.print("*** Forecast icon[day]:  "); Serial.print(weatherIcon); Serial.print("["); Serial.print(dayIndex); Serial.println("] ***");
  if (weatherIcon == "chancetstorms") weatherIcon = "tstorms";      // Temporary fix for missing "chancetstorms" mini .bmp
  ui.drawBmp("/mini/" + weatherIcon + ".bmp", x, y + 15);
  // overlay probability of precipitation
  String PoP = wunderground.getPoP(dayIndex) + "%";
  ui.setTextAlignment(RIGHT);
  ui.drawString(x + 55, y + 65, PoP);
}

// draw moonphase and sunrise/set and moonrise/set
void drawAstronomy() {
  tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);
  int moonAgeImage = 24 * wunderground.getMoonAge().toInt() / 30.0;
  Serial.print("Moon age image:  "); Serial.println(moonAgeImage);
  ui.drawBmp("/moon" + String(moonAgeImage) + ".bmp", 120 - 30, 250);
  
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(&ArialRoundedMTBold_14);  
  ui.setTextAlignment(LEFT);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  ui.drawString(20, 270, "Sun");
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.setTextAlignment(RIGHT);
  ui.drawString(50, 285, wunderground.getSunriseTime());
  ui.drawString(50, 300, wunderground.getSunsetTime());

  ui.setTextAlignment(RIGHT);
  ui.setTextColor(ILI9341_ORANGE, ILI9341_BLACK);
  ui.drawString(220, 270, "Moon");
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.drawString(220, 285, wunderground.getMoonriseTime());
  ui.drawString(220, 300, wunderground.getMoonsetTime());

//  tft.drawRect(0, 233, 240, 80, ILI9341_WHITE);
  
}

// draw weather alerts indicator (as they exist)
void drawAlerts() {
  int activeAlerts;
  byte indicBaseX;      // Initial indicator panel X coord
  byte indicBaseY;      // Initial indicator panel Y coord
  byte fontHeight;      // Font height; adjust for font changes

  // Set the initial indicator panel baseline location and font-height
  indicBaseX = 12;
  indicBaseY = 11;
  fontHeight = 12;
  
  tft.setCursor(indicBaseX, indicBaseY);
  
//  tft.drawRect(0, 0, 40, 78, ILI9341_WHITE);
  tft.fillRect(0, 0, 40, 78, ILI9341_BLACK);

  tft.setFont(&Droid_Sans_10);  
  ui.setTextAlignment(CENTER);
   
  activeAlerts = wunderground.getActiveAlertsCnt();
  Serial.print("Number of active alerts:  "); Serial.println(activeAlerts);

  if (activeAlerts != 0) {
    for (byte i=1; i<=activeAlerts; i++) {
      Serial.println();
      Serial.print("Alert brf:   "); Serial.println(wunderground.getActiveAlerts(i-1));
      Serial.print("Alert desc:  "); Serial.println(wunderground.getActiveAlertsText(i-1));
      Serial.print("Alert msg:   "); Serial.println(wunderground.getActiveAlertsMessage(i-1));
      Serial.print("Alert start: "); Serial.println(wunderground.getActiveAlertsStart(i-1));
      Serial.print("Alert end:   "); Serial.println(wunderground.getActiveAlertsEnd(i-1));
      Serial.print("Alert phen:  "); Serial.println(wunderground.getActiveAlertsPhenomena(i-1));
      Serial.print("Alert sign:  "); Serial.println(wunderground.getActiveAlertsSignificance(i-1));
      String temp = wunderground.getActiveAlerts(i-1);
      ui.setTextAlignment(CENTER);
      // Get significance code to determine alert color; red for warnings; yellow for watches;
      // cyan for everything else.
      String sign = wunderground.getActiveAlertsSignificance(i-1);
      if      (sign == "W" || sign == "3"){ ui.setTextColor(ILI9341_RED, ILI9341_BLACK); }
      else if (sign == "A" || sign == "2"){ ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK); }
      else                { ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK); }
      ui.drawString(indicBaseX, indicBaseY+(fontHeight*(i-1)), temp);
    }
  } 
}

// draw weather alerts panels (as they exist)
void drawAlertsPanels() {
  int activeAlerts;

  tft.setFont(&Droid_Sans_10);  
  ui.setTextAlignment(LEFT);
  String temp;

  activeAlerts = wunderground.getActiveAlertsCnt();
  // Serial.print("Number of active alerts:  "); Serial.println(activeAlerts);

  // temporarily detach the bottom panel ticker since we're going to control display
  // of the variable number of alert panels in this routine
  botPanelTicker.detach(); 
  if (activeAlerts != 0) {
    for (byte i=1; i<=activeAlerts; i++) {
      // Serial.println();
      // Serial.print("Alert brf:   "); Serial.println(wunderground.getActiveAlerts(i-1));
      // Serial.print("Alert desc:  "); Serial.println(wunderground.getActiveAlertsText(i-1));
      // Serial.print("Alert start: "); Serial.println(wunderground.getActiveAlertsStart(i-1));
      // Serial.print("Alert end:   "); Serial.println(wunderground.getActiveAlertsEnd(i-1));
      // Serial.print("Alert phen:  "); Serial.println(wunderground.getActiveAlertsPhenomena(i-1));
      // Serial.print("Alert sign:  "); Serial.println(wunderground.getActiveAlertsSignificance(i-1));
      drawAlertDetail(i-1);
      delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
      drawAlertMsg(i-1);
      // delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
    }
    temp = wunderground.getActiveAlertsAttribution(0);
    if (temp != "") {
      drawAlertAttribution();
    }
  }
  // reattach ticker for bottom panel update
  botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen
  // force a bottom panel update to get back in sync with the ticker
  readyForBotPanelUpdate = true;

}

// display current alert details
void drawAlertDetail(int alertIndex) {

  tft.fillRect(0, 231, 240, 87, ILI9341_BLACK);

  String temp = "(" + wunderground.getActiveAlerts(alertIndex) + ")  " + wunderground.getActiveAlertsText(alertIndex);
  tft.setCursor(120,245);
  ui.setTextAlignment(CENTER);
  // Get significance code to determine alert color; red for warnings; yellow for watches;
  // cyan for everything else.
  String sign = wunderground.getActiveAlertsSignificance(alertIndex);
  if      (sign == "W" || sign == "3"){ ui.setTextColor(ILI9341_RED, ILI9341_BLACK); }
  else if (sign == "A" || sign == "2"){ ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK); }
  else                { ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK); }
  ui.drawString(120, 245, temp);
  ui.setTextAlignment(LEFT);
  tft.setFont(&Droid_Sans_Bold_10);  
  temp = "Begins:  ";
  tft.setCursor(0,256);
  tft.println(temp);
  temp = "  " + wunderground.getActiveAlertsStart(alertIndex);
  tft.setFont(&Droid_Sans_10);  
  tft.println(temp);
  tft.setFont(&Droid_Sans_Bold_10);  
  temp = "Ends:    ";
  tft.println(temp);
  tft.setFont(&Droid_Sans_10);  
  temp = "  " + wunderground.getActiveAlertsEnd(alertIndex);
  tft.println(temp);
}

// display current message panel
void drawAlertMsg(int alertIndex) {
  String temp;
  byte msgPanelCnt = 0;
  
  tft.fillRect(0, 231, 240, 87, ILI9341_BLACK);

  // String temp = "(" + wunderground.getActiveAlerts(alertIndex) + ")  " + wunderground.getActiveAlertsText(alertIndex);
  // tft.setCursor(120,245);
  // ui.setTextAlignment(CENTER);
  // Get significance code to determine alert color; red for warnings; yellow for watches;
  // cyan for everything else.
  String sign = wunderground.getActiveAlertsSignificance(alertIndex);
  if      (sign == "W" || sign == "3"){ ui.setTextColor(ILI9341_RED, ILI9341_BLACK); }
  else if (sign == "A" || sign == "2"){ ui.setTextColor(ILI9341_YELLOW, ILI9341_BLACK); }
  else                { ui.setTextColor(ILI9341_CYAN, ILI9341_BLACK); }
  // ui.drawString(120, 240, temp);
  ui.setTextAlignment(LEFT);
  tft.setFont(&Droid_Sans_10);  
  tft.setCursor(0,250);
  temp = wunderground.getActiveAlertsMessage(alertIndex);
  // stripUnicode(temp);       // strip any unicode characters and replace with blanks
  msgPanelCnt = stringWordWrap(temp);
  // If the possible message truncation flag is set, display an indicator of such.  (This is due to the fact that the
  // JSON parser allocates a maximum of 512-bytes, which may not be enough for the full text of the message.
  if (wunderground.getActiveAlertsMessageTrunc(alertIndex)) {
    tft.print("<...>");
  }
  // If only one message alert panel was displayed, then pause to display the screen (since the ticker isn't used to
  // control these panels.
  if (msgPanelCnt == 1) delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
}

// display the attribution required for use of European weather alerts
void drawAlertAttribution() {
  tft.fillRect(0, 231, 240, 87, ILI9341_BLACK);
  tft.setFont(&Droid_Sans_10);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  ui.setTextAlignment(LEFT);
  tft.setCursor(0,245);
  String temp = wunderground.getActiveAlertsAttribution(0);      // attribution should be the same regardless of the alert
  Serial.print("Alert attrib:  "); Serial.println(temp);
  stringWordWrap(temp);
}

// display current forecast text
void drawForecastText(int forecastTextPeriod) {
  tft.fillRect(0, 231, 240, 87, ILI9341_BLACK);
  ui.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft.setFont(&Droid_Sans_10);  
  ui.setTextAlignment(LEFT);
  tft.setTextWrap(true);
  String temp;

  tft.setCursor(0,245);
  temp = wunderground.getForecastTitle(forecastTextPeriod) + ":";             // Current period
  tft.println(temp);
  temp = wunderground.getForecastText(forecastTextPeriod);
  stringWordWrap(temp);

}
  
// Helper function, should be part of the weather station library and should disappear soon
String getMeteoconIcon(String iconText) {
  if (iconText == "F") return "chanceflurries";
  if (iconText == "Q") return "chancerain";
  if (iconText == "W") return "chancesleet";
  if (iconText == "V") return "chancesnow";
  if (iconText == "S") return "chancetstorms";
  if (iconText == "B") return "clear";
  if (iconText == "Y") return "cloudy";
  if (iconText == "F") return "flurries";
  if (iconText == "M") return "fog";
  if (iconText == "E") return "hazy";
  if (iconText == "Y") return "mostlycloudy";
  if (iconText == "H") return "mostlysunny";
  if (iconText == "H") return "partlycloudy";
  if (iconText == "J") return "partlysunny";
  if (iconText == "W") return "sleet";
  if (iconText == "R") return "rain";
  if (iconText == "W") return "snow";
  if (iconText == "B") return "sunny";
  if (iconText == "0") return "tstorms";
  

  return "unknown";
}

// if you want separators, uncomment the tft-line
void drawSeparator(uint16_t y) {
   //tft.drawFastHLine(10, y, 240 - 2 * 10, 0x4228);
}

void setReadyForWeatherUpdate() {
  Serial.println("Setting readyForUpdate to true");
  readyForWeatherUpdate = true;
}

void setReadyForForecastUpdate() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if the 10-day forecast interval has been reached, triggering an update
    of the forecast information. 
*/
  forecastTimer++;
//  Serial.print("***Checking 10-day forecast update timer, timer="); Serial.println(forecastTimer);
  if (forecastTimer >= (UPDATE_INTERVAL_FORECAST_SECS / 60)) {
    Serial.println("Setting readyForForecastUpdate to true");
    readyForForecastUpdate = true;
    forecastTimer = 0;
  }
}

void setReadyForAstronomyUpdate() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if the astronomy data update interval has been reached, triggering an update
    of this information. 
*/
  astronomyTimer++;
//  Serial.print("***Checking astronomy update timer, timer="); Serial.println(astronomyTimer);
  if (astronomyTimer >= (UPDATE_INTERVAL_ASTRO_SECS / 60)) {
    Serial.println("Setting readyForAstronomyUpdate to true");
    readyForAstronomyUpdate = true;
    astronomyTimer = 0;
  }
}

void setReadyForAlertsUpdate() {
/*
    This routine is triggered every 60 secs to increment and check to see
    if the alerts data update interval has been reached, triggering an update
    of this information. 
*/
  alertsTimer++;
//  Serial.print("***Checking weather alerts update timer, timer="); Serial.println(alertsTimer);
  if (alertsTimer >= (UPDATE_INTERVAL_ALERTS_SECS / 60)) {
    Serial.println("Setting readyForAlertsUpdate to true");
    readyForAlertsUpdate = true;
    alertsTimer = 0;
  }
}

void setReadyForMidPanelUpdate() {
  Serial.println("Setting readyForMidPanelUpdate to true");
  readyForMidPanelUpdate = true;
}

void setReadyForBotPanelUpdate() {
  Serial.println("Setting readyForBotPanelUpdate to true");
  readyForBotPanelUpdate = true;
}

// Routine to perform word-wrap display of text string
byte stringWordWrap(String displayString) {
  // The approach in this routine is as follows:
  //    1) Estimate the possible number of chars that can be displayed per line with the
  //       current font.  This is done by first calculating the width in pixels of a 10-character
  //       sample string using the current font.
  //    2) Divide the maximum pixels per line by the average pixel width/char.
  //    3) Now begin scanning the display string starting at the maximum character position per line,
  //       moving backwards from there looking for the first space or ellipse sequence.  If either is found,
  //       then this is an acceptable place to break the line, so display it.
  //    4) Otherwise, keep moving backwards until either is found; break there and display the
  //       segment.
  //    5) Rinse and repeat starting with the character following the break, through the end of
  //       the display string.
  //    6) Of course, need to handle null strings and lines with no break-points (break anyway).
  //    7) The routine keeps track of the number of lines in order to display multiple panels if
  //       required.  It will delay between panels using the default timing used for the bottom
  //       panel.
  // 
  //    The routine will return the count of panels displayed (which can be/is used to determine if
  //    additional panel delays are required.
   
  char sampleString[] = "aaaaaaaaaa";  // Adjust sample string as appropriate based on type of typical data
  int16_t x1, y1;
  uint16_t w, h, x, y;
  int breakLocation, spaceBreakLocation, ellipseBreakLocation;
  byte maxPanelLines = 6;              // Default maximum panel lines; adjust for font-size changes
  byte lineCnt = 0;                    // Used to track number of lines displayed for this panel
  bool resetTickerFlag = false;
  byte panelCnt = 0;

//  Serial.println("*** stringWordWrap called ***");
  if (displayString == "") return 0;     // Just return and do nothing if the passed display line is null.

  displayString.replace("  "," ");     // Replace all instances of multiple spaces with one space.
  
  x = 0; y = 20;                       // Sample coordinates used to calculate the max chars per line.
  tft.getTextBounds(sampleString, x, y, &x1, &y1, &w, &h);
  float pixelsChar = w/10;            // 10 characters in the sample string.
//  Serial.print("Sample string width (pixels):  "); Serial.println(w);
//  Serial.print("Average number of pixels/char (w):  "); Serial.println(pixelsChar,3);

  int charsPerLine = (240 / pixelsChar) - 1;
  int estBreak = charsPerLine;
  int beginLineIndex = 0;

  int displayStringLen = displayString.length();
//  Serial.println("***Beginning word wrap of passed string***");
//  Serial.print("Length of passed string:  "); Serial.println(displayStringLen);
//  Serial.print("Calc chars / line:  "); Serial.println(charsPerLine);
  while (estBreak < displayStringLen) {
    // Search backwards from the estimated line break for the first space or "..." sequence
//    Serial.print("Beginning line index:  "); Serial.println(beginLineIndex);
//    Serial.print("Estimated break location:  "); Serial.println(estBreak);
    spaceBreakLocation = (displayString.substring(beginLineIndex,estBreak)).lastIndexOf(" ",estBreak);
    ellipseBreakLocation = (displayString.substring(beginLineIndex,estBreak)).lastIndexOf("...",estBreak)+2;
    // set the break location at the greater of where a space is found or an ellipse sequence
    if (spaceBreakLocation >= ellipseBreakLocation) {
      breakLocation = spaceBreakLocation;
    } else {
      breakLocation = ellipseBreakLocation;
    }
//    Serial.print("Found break location at character:  "); Serial.println(breakLocation);
    // If no space is found in the line segment, force a break at the max chars / line position.
    if (breakLocation == -1) {
      breakLocation = estBreak;
    } else {
      breakLocation = breakLocation + beginLineIndex;
    }
    tft.println(displayString.substring(beginLineIndex,breakLocation));
    lineCnt++;
    if (lineCnt >= maxPanelLines) {
      panelCnt++;
      lineCnt = 0;
      // temporarily detach the bottom panel ticker since we're going to control display
      // of the variable number of message panels in this routine
      botPanelTicker.detach();
      delay(UPDATE_BOT_PANEL_INTERVAL_SECS * 1000);
      resetTickerFlag = true;       // flag that the ticker needs to be reattached
      tft.setCursor(0,245);         // reset cursor to top of panel
      tft.fillRect(0, 233, 240, 87, ILI9341_BLACK);   // clear panel for next display
    }
//    Serial.print("["); Serial.print(displayString.substring(beginLineIndex,breakLocation)); Serial.println("]");
    beginLineIndex = breakLocation + 1;
    estBreak = breakLocation + charsPerLine; 
  }
  if (estBreak >= displayStringLen) {
    // Display the last string segment
    tft.print(displayString.substring(beginLineIndex));
    panelCnt++;
  }  
//    Serial.print("["); Serial.print(displayString.substring(beginLineIndex)); Serial.println("]");
  if (resetTickerFlag) {
  // reattach ticker for bottom panel update
    botPanelTicker.attach(UPDATE_BOT_PANEL_INTERVAL_SECS, setReadyForBotPanelUpdate); // To trigger updating bottom-panel on screen
    // force a bottom panel update to get back in sync with the ticker
    readyForBotPanelUpdate = true;
  }
return panelCnt;
} 

