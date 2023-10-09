#include <pgmspace.h>
const char* ssid = "WiFi_Name";
const char* password = "WiFi_Pass";

// AWS IOT
#define THINGNAME "AWS_IoT_Thing_Name"                         //change this
const char AWS_IOT_ENDPOINT[] = "***.amazonaws.com";       //change this
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
 
 
)KEY";
