//        SerialPort over WiFi
// Serial data over WiFi transreciever
//         By Mohammad Yousefi
//       vahid_you2004@yahoo.com
//            www.dihav.com
//              Mar 2016

#include <ESP8266WiFi.h>
#include <EEPROM.h>

// EEPROM constants
#define EEPROMSIZE   128
#define APSSIDAddr   1
#define APPassAddr   31
#define STSSIDAddr   61
#define STPassAddr   91

String ap_ssid, ap_password, st_ssid, st_password;
String pststr, pstnam, pstval;
String recbuff, remoteDevIP;
bool isopen, lastrstpsw, isownport;
unsigned long rpdmil;
byte RUTSWbf[1024];
unsigned int RUTSWsz;
WiFiServer ser80(80), ser2321(2321);

void setup() {
  DefaultSettings();
  LoadSettings();
  recbuff = "";  
  isopen = false;
  isownport = false;
  RUTSWsz = 0;
  WiFi.mode(WIFI_STA);
  bool b = true; 
  if (st_ssid != "----------") {
    byte n = WiFi.scanNetworks();
    if (n) {
      String s;
      for (byte i = 0; i < n; i++) {       
        s = WiFi.SSID(i);
        if (s == st_ssid) {
          if (st_password != "----------") WiFi.begin(st_ssid.c_str(), st_password.c_str()); else WiFi.begin(st_ssid.c_str()); 
          byte c = 0;  
          while (c < 40) {
            if (WiFi.status() == WL_CONNECTED) {
              c = 45;            
              b = false;
              i = n - 1;
            } 
            delay(500);
            c++;
            wdt_reset();
          }
        }
      }
    }
  }
  if (b) {
    WiFi.mode(WIFI_AP);
    if (ap_password != "----------") WiFi.softAP(ap_ssid.c_str(), ap_password.c_str()); else WiFi.softAP(ap_ssid.c_str());
  } 
  ser80.begin();
  ser2321.begin();
  pinMode(0, INPUT);
  pinMode(2, INPUT);
  lastrstpsw = digitalRead(0);
}

void loop() {
  // reset passwords button
  if (digitalRead(0) == LOW) {
    if (lastrstpsw) rpdmil = millis();
    if (millis() - rpdmil > 2000) {
      DefaultSettings();
      SaveSettings();
      ESP.reset();
    }
  } 
  lastrstpsw = digitalRead(0);
  // check for serial data
  if (isopen) {
    while (Serial.available()) {
      byte b = Serial.read();  
      if (isownport) {       
        RUTSWbf[RUTSWsz] = b;     
        RUTSWsz++;      
        if (RUTSWsz == 1024) break;     
      } else {
        if (b < 16) recbuff += "0";
        recbuff += String(b, HEX);  
      }  
      delay(1);   
    }
    if (RUTSWsz) {
      WiFiClient c;
      if (c.connect(remoteDevIP.c_str(), 2321)) {
        c.write((const uint8_t *)RUTSWbf, (size_t)RUTSWsz);        
        delay(10);
        c.flush();
        c.stop(); // close the connection
        RUTSWsz = 0;
      }      
    }
  }
  // check for http client 
  WiFiClient cl = ser80.available();
  if (cl) {
    while (!cl.available()) delay(1); // wait for request
    String req = cl.readStringUntil('\r');
    byte pg = 0;
    if (req.indexOf("/set") != -1) pg = 1;
    if (req.indexOf("/con") != -1) pg = 2; 
    if (req.indexOf("/dis") != -1) pg = 3; 
    if (req.indexOf("/rec") != -1) pg = 4; 
    if (req.indexOf("/snd") != -1) pg = 5; 
    while (cl.available()) {
      req = cl.readStringUntil('\r');
      if (req.length() > 3) pststr = req;
      wdt_reset();
    }
    if (pg == 0) {
      HTMLHeader(cl, false);
      HTMLIndex(cl);
      HTMLFooter(cl);   
    } else if (pg == 1) {
      bool rs = false;    
      pstnam = "assid";
      GetPost();
      if ((pstval != "") && (pstval != ap_ssid)) {      
        rs = true;
        ap_ssid = pstval;    
      }
      pstnam = "apswrd";
      if (GetPost()) {
        if (pstval == "") pstval = "----------";
        if ((pstval.length() > 7) && (pstval != ap_password) && (pstval != "NO_CHANGE")) {
          rs = true;
          ap_password = pstval;    
        }
      }
      pstnam = "cssid";
      if (GetPost()) {
        if (pstval == "") pstval = "----------";
        if (pstval != st_ssid) {
          rs = true;
          st_ssid = pstval;    
        }
      }
      pstnam = "cpswrd";
      if (GetPost()) {
        if (pstval == "") pstval = "----------";
        if ((pstval.length() > 7) && (pstval != st_password) && (pstval != "NO_CHANGE")) {
          rs = true;
          st_password = pstval; 
        }   
      }
      if (rs) SaveSettings();
      wdt_reset();
      HTMLHeader(cl, true);
      HTMLSettings(cl);
      HTMLFooter(cl);        
      if (rs) {
        delay(10);
        cl.stop(); // close the connection
        ESP.reset();
      }
    } else if (pg == 2) {
      if (isopen) {
        Serial.end();
        recbuff = "";
        RUTSWsz = 0;
        isopen = false;
        isownport = false;
      }
      int b = 0;
      byte n, p, s;
      pstnam = "bdrt";
      GetPost();
      if (pstval != "") b = pstval.toInt();   
      pstnam = "dbt";
      GetPost();
      if (pstval != "") n = pstval.toInt(); else b = 0;
      pstnam = "sbt";
      GetPost();
      if (pstval != "") s = pstval.toInt(); else b = 0;
      pstnam = "prty";
      GetPost();
      if (pstval != "") p = pstval.toInt(); else b = 0;
      HTTPHeader(cl);
      if (b) {
        switch (n) {
          case 5:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_5N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_5O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_5E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_5N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_5O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_5E2);
            break;
          case 6:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_6N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_6O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_6E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_6N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_6O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_6E2);
            break;
          case 7:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_7N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_7O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_7E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_7N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_7O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_7E2);
            break;
          case 8:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_8N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_8O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_8E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_8N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_8O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_8E2);
            break;    
        }       
        isopen = true;
        delay(50);
        cl.print("OK");
      } else
        cl.print("ER");            
    } else if (pg == 3) {
      if (isopen) {
        Serial.end();
        recbuff = "";
        RUTSWsz = 0;
        isopen = false;
        isownport = false;
      }
      HTTPHeader(cl);
      cl.print("OK");
    } else if (pg == 4) {
      HTTPHeader(cl);
      if (isopen) {
        cl.print("OK" + recbuff);
        recbuff = "";
      } else
        cl.print("ER");    
    } else if (pg == 5) {
      HTTPHeader(cl);
      if (isopen) {   
        byte b = 0;
        char c;      
        for (int k = 1; k < req.length(); k++) {
          c = req.c_str()[k];       
          if (k % 2 == 0) {
            b += HexDigit(c);          
            Serial.write(b);
          } else 
            b = HexDigit(c) * 16;  
        }
        Serial.flush(); // wait for serial trasmission
        cl.print("OK");
      } else
        cl.print("ER");   
    }
    delay(10);
    cl.stop(); // close the connection  
  }
  // check for UW client 
  WiFiClient cl2 = ser2321.available();
  if (cl2) {   
    while (!cl2.available()) delay(1); // wait for request
    byte cmd = cl2.read();
    if (cmd == 1) {
      if (isopen) {
        Serial.end();
        recbuff = "";
        RUTSWsz = 0;
        isopen = false;
        isownport = false;
      }
      int b = 0;
      byte n, p, s;      
      while (cl2.available() < 7) delay(1);
      b = (int)cl2.read() * 0x1000000;
      b += (int)cl2.read() * 0x10000;
      b += (int)cl2.read() * 0x100;
      b += (int)cl2.read();
      n = cl2.read();
      s = cl2.read();
      p = cl2.read();     
      if (b) {
        switch (n) {
          case 5:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_5N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_5O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_5E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_5N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_5O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_5E2);
            break;
          case 6:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_6N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_6O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_6E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_6N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_6O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_6E2);
            break;
          case 7:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_7N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_7O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_7E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_7N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_7O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_7E2);
            break;
          case 8:
            if ((p == 0) && (s == 1)) Serial.begin(b, SERIAL_8N1);
            if ((p == 1) && (s == 1)) Serial.begin(b, SERIAL_8O1);
            if ((p == 2) && (s == 1)) Serial.begin(b, SERIAL_8E1);
            if ((p == 0) && (s == 2)) Serial.begin(b, SERIAL_8N2);
            if ((p == 1) && (s == 2)) Serial.begin(b, SERIAL_8O2);
            if ((p == 2) && (s == 2)) Serial.begin(b, SERIAL_8E2);
            break;    
        }            
        isopen = true;
        isownport = true;    
        delay(50);  
        cl2.flush();
        IPAddress p = cl2.remoteIP();
        remoteDevIP = String(p[0]) + '.' + String(p[1]) + '.' + String(p[2]) + '.' + String(p[3]);
        cl2.write((byte)1);  
      } else
        cl2.write((byte)0);     
    } else if (cmd == 2) {
      if (isopen) {
        Serial.end();
        recbuff = "";
        RUTSWsz = 0;
        isopen = false;
        isownport = false;
        cl2.flush();
      }
      cl2.write((byte)1);
    } else if (cmd == 3) {
      if (isopen) {  
        while (!cl2.available()) delay(1);
        byte c, z;
        z = cl2.read();
        c = 0;
        while (c < z) {
          while (!cl2.available()) delay(1);
          Serial.write(cl2.read());
          c++;
        }
        Serial.flush(); // wait for serial trasmission
        cl2.flush();
        cl2.write((byte)1);
      } else
        cl2.write((byte)0);   
    } else if (cmd == 4) {
      cl2.flush();
      cl2.write((byte)44);
    }
    delay(10);
    cl2.stop(); // close the connection  
  }
}

void DefaultSettings() {
  byte m[6];
  WiFi.softAPmacAddress(m);
  ap_ssid = "";
  if (m[3] < 16) ap_ssid = "0";
  ap_ssid += String(m[3], HEX);
  if (m[4] < 16) ap_ssid += "0";
  ap_ssid += String(m[4], HEX);
  if (m[5] < 16) ap_ssid += "0"; 
  ap_ssid += String(m[5], HEX);
  ap_ssid.toUpperCase();
  ap_ssid = "dihavSerialPort_" + ap_ssid;
  ap_password = "----------";
  st_ssid = "----------";
  st_password = "----------";
}

void SaveSettings() {
  byte i;
  EEPROM.begin(EEPROMSIZE);
  EEPROM.write(0, 92); 
  for (i = 0; i < ap_ssid.length(); i++) EEPROM.write(APSSIDAddr + i, ap_ssid[i]);
  EEPROM.write(APSSIDAddr + ap_ssid.length(), 0);
  for (i = 0; i < ap_password.length(); i++) EEPROM.write(APPassAddr + i, ap_password[i]);
  EEPROM.write(APPassAddr + ap_password.length(), 0);
  for (i = 0; i < st_ssid.length(); i++) EEPROM.write(STSSIDAddr + i, st_ssid[i]);
  EEPROM.write(STSSIDAddr + st_ssid.length(), 0);
  for (i = 0; i < st_password.length(); i++) EEPROM.write(STPassAddr + i, st_password[i]);
  EEPROM.write(STPassAddr + st_password.length(), 0);
  wdt_reset();
  EEPROM.end();
}

void LoadSettings() {
  EEPROM.begin(EEPROMSIZE);
  if (EEPROM.read(0) == 92) {
    word i = APSSIDAddr;
    byte r, k;
    String v; 
    for (byte j = 0; j < 4; j++) {
      v = "";
      if (j == 0) i = APSSIDAddr;
      if (j == 1) i = APPassAddr;
      if (j == 2) i = STSSIDAddr;
      if (j == 3) i = STPassAddr;
      for (k = 0; k < 30; k++) {
        r = EEPROM.read(i + k);
        if (r) 
          v += (char)r;
        else 
          break;
      }
      if (j == 0) ap_ssid = v;
      if (j == 1) ap_password = v;
      if (j == 2) st_ssid = v;
      if (j == 3) st_password = v;
      wdt_reset();
    }
  } 
  EEPROM.end();  
}

byte HexDigit(char dig) {
  byte r = 0;
  if ((dig >= '0') && (dig <= '9')) r = (byte)(dig - '0');
  if ((dig >= 'a') && (dig <= 'f')) r = (byte)(dig - 'a') + 10;
  if ((dig >= 'A') && (dig <= 'F')) r = (byte)(dig - 'A') + 10;
  return (r);
}

bool GetPost() {
  pstval = "";
  int s = pststr.indexOf(pstnam + "=");
  if (s != -1) {
    int e = pststr.indexOf("&", s);
    if (e == -1)
      pstval = pststr.substring(s + pstnam.length() + 1);
    else
      pstval = pststr.substring(s + pstnam.length() + 1, e);
    return (true);
  }  
  return (false);
}

void HTTPHeader(WiFiClient cl) {
  cl.flush(); // clear all unread data 
  cl.print("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n");
}

void HTMLHeader(WiFiClient cl, bool st) {
  HTTPHeader(cl);
  String s = "<html><head><title>dihav SerialPort over WiFi - ";
  s += st?"Settings":"HTML Interface";
  s += "</title></head><body style=\"background-color:#1C2128;text-align:center;line-height:250%\"><br><div style=\"width:";
  s += st?"300":"500";
  s += ";padding:10;margin:auto;box-shadow:0 0 6 2 rgba(0,0,0,0.2);border-radius:10;background-color:#8EA9CC\"><style>input{width:100%} select{width:100%}";
  cl.print(s);
}

void HTMLFooter(WiFiClient cl) {
  cl.print("</div><a href=http://www.dihav.com/ style=\"text-decoration:none;color:#8EA9CC;font-size:0.6em\">www.dihav.com</a></body></html>");
}

void HTMLSettings(WiFiClient cl) {  
  byte i, n, m[6];
  String s;
  s = "</style><b>" + ap_ssid;
  s += " Settings</b><br><form name=frm method=post><table style=\"width:100%\"><tr><td style=\"width:30%\">Dev. Name:</td><td style=\"width:70%\"><input name=assid value=\"";
  s += ap_ssid;
  cl.print(s);
  cl.print("\"></td></tr><tr><td colspan=2 align=right><u><br>Act as Access Point</u></td></tr><tr><td>SSID:</td><td>Same as Device Name</td></tr><tr><td>Password:</td><td><input name=apswrd value=\"NO_CHANGE\"></td></tr><tr><td>MAC:</td><td>");
  WiFi.softAPmacAddress(m);
  s = "";
  for (i = 0; i < 6; i++) {
    if (m[i] < 16) s += "0";
    s += String(m[i], HEX);
    if (i != 5) s += "-";
  }
  cl.print(s + "</td></tr><tr><td colspan=2 align=right><u><br>Connect to Access Point</u></td></tr><tr><td>Networks:</td><td>");
  n = WiFi.scanNetworks();
  s = "";
  if (n)
    for (i = 0; i < n; i++) {
      s += "<a href=\"#\" onclick=\"frm.cssid.value='";
      s += WiFi.SSID(i);
      s += "';frm.cpswrd.value='';frm.cpswrd.focus();return(false);\">";
      s += WiFi.SSID(i);
      s += "</a>";  
      if (WiFi.encryptionType(i) == ENC_TYPE_NONE) s += " <span style=\"font-size:0.75em;color:green\">(OPEN)</span>";
      s += "<br>";
    }
  else
    s = "NO NETWORK AVAILABLE";
  cl.print(s);  
  s = "</td></tr><tr><td>SSID:</td><td><input name=cssid value=\"";
  s += st_ssid;
  cl.print(s + "\"></td></tr><tr><td>Password:</td><td><input name=cpswrd value=\"NO_CHANGE\"></td></tr><tr><td>MAC:</td><td>");
  WiFi.macAddress(m);
  s = "";
  for (i = 0; i < 6; i++) {
    if (m[i] < 16) s += "0";
    s += String(m[i], HEX);
    if (i != 5) s += "-";
  }
  cl.print(s + "</td></tr><tr><td colspan=2 align=center><br><input type=submit value=Save></td></tr>");
  cl.print("<tr><td colspan=2><br>At startup if connection to above access point fails, the device will act as an access point itself.<br>To reset settings hold reset button on the device for about 2 seconds.</td></tr></table></form><a href=hom>HTML Serial Interface</a>");
}

void HTMLIndex(WiFiClient cl) {
  String s;
  s = " .b2{width:50%} .b4{width:25%} </style><script language=javascript>\r\n";
  s += "var art, rfc = 0;\r\n";
  s += "function con() {\r\n";
  s += "var s='bdrt=' + frm.bdrt.value + '&dbt=' + frm.dbt.value + '&sbt=' + frm.sbt.value + '&prty=' + frm.prty.value;\r\n";
  cl.print(s);
  s = "var req = new XMLHttpRequest();\r\n";
  s += "req.onload = function(e) {\r\n";
  s += " if (req.status == 200) if (req.responseText == 'OK') {\r\n";
  s += " frm.disbtn.disabled = false;\r\n";
  s += " frm.conbtn.disabled = true;\r\n";
  s += " if (frm.ars.value != 0) art = setInterval(rec, frm.ars.value);\r\n";
  s += " }\r\n";
  s += " }\r\n"; 
  s += "req.open('POST', 'con?' + rfc.toString(), true);\r\n";
  s += "rfc++;\r\n";
  s += "req.send(s);\r\n";
  s += "}\r\n";
  cl.print(s);
  s = "function dis() {\r\n";
  s += "var req = new XMLHttpRequest();\r\n";
  s += "req.onload = function(e) {\r\n";
  s += " if (req.status == 200) if (req.responseText == 'OK') {\r\n";
  s += " frm.disbtn.disabled = true;\r\n";
  s += " frm.conbtn.disabled = false;\r\n";
  s += " clearTimeout(art);\r\n";
  s += " }\r\n";
  s += " }\r\n";  
  s += "req.open('GET', 'dis?' + rfc.toString(), true);\r\n";
  s += "rfc++;\r\n";
  s += "req.send(null);\r\n";
  s += "}\r\n";
  cl.print(s);
  s = "function arschng() {\r\n";
  s += "if (frm.disbtn.disabled) return;\r\n";
  s += "clearTimeout(art);\r\n";
  s += "if (frm.ars.value != 0) art = setInterval(rec, frm.ars.value);\r\n";
  s += "}\r\n";
  s += "function rec() {\r\n";
  s += "if (frm.disbtn.disabled) return;\r\n";
  s += "var req = new XMLHttpRequest();\r\n";
  s += "req.onload = function(e) {\r\n";
  s += " if (req.status == 200) if (req.responseText.startsWith('OK')) {\r\n";
  s += " frm.recvd.innerHTML += dt2txt(req.responseText.substr(2), frm.rectp.value);\r\n";
  s += " }\r\n";
  s += " }\r\n";
  s += "req.open('GET', 'rec?' + rfc.toString(), true);\r\n";
  cl.print(s);
  s = "rfc++;\r\n";
  s += "req.send(null);\r\n";
  s += "}\r\n";  
  s += "function send(data) {\r\n";
  s += "if (frm.disbtn.disabled) return;\r\n";
  s += "var req = new XMLHttpRequest();\r\n";
  s += "req.onload = function(e) {\r\n";
  s += " if (req.status == 200) if (req.responseText == 'OK') {\r\n";
  s += " frm.sndhst.innerHTML += dt2txt(data, frm.dttp.value);\r\n";
  s += " }\r\n";
  s += " }\r\n";
  s += "req.open('POST', 'snd?' + rfc.toString(), true);\r\n";
  s += "rfc++;\r\n";
  s += "req.send(data);\r\n";
  s += "}\r\n";
  cl.print(s);
  s = "function snddt() {\r\n";
  s += "var d = '', v = frm.snd.value, t = frm.dttp.value, p = '', i;\r\n";
  s += "if (t != 0) v += ' ';\r\n";
  s += "for (var k = 0; k < v.length; k++)\r\n";
  s += "if (t == 0) {\r\n";
  s += "d += v.charCodeAt(k).toString(16);\r\n";
  s += "} else if (v.charAt(k) == ' ') {\r\n";
  s += "if ((p != '') && (t == 1)) i = parseInt(p, 16);\r\n";
  s += "if ((p != '') && (t == 2)) i = parseInt(p);\r\n";
  s += "if (!isNaN(i)) {\r\n";
  s += "if (i < 0) i = 0;\r\n";
  s += "if (i > 255) i = 255;\r\n";
  s += "if (i < 16) d += '0';\r\n";
  cl.print(s);
  s = "d += i.toString(16);\r\n";
  s += "}\r\n";
  s += "p = '';\r\n";
  s += "} else\r\n";
  s += "p += v.charAt(k);\r\n";
  s += "if (frm.apnd.value == 1) d += '0d';\r\n";
  s += "if (frm.apnd.value == 2) d += '0a';\r\n";
  s += "if (frm.apnd.value == 3) d += '0d0a';\r\n";
  s += "send(d);\r\n";
  s += "}\r\n";
  s += "function dt2txt(data, typ) {\r\n";
  cl.print(s);
  s = "var r = '', i;\r\n";
  s += "for (var k = 0; k < data.length; k += 2) {\r\n";
  s += "i = parseInt(data.substr(k, 2), 16);\r\n";
  s += "if (typ == 0) r += String.fromCharCode(i);\r\n";
  s += "if ((typ == 1) && (i < 16)) r += '0';\r\n";
  s += "if (typ == 1) r += i.toString(16) + ' ';\r\n";
  s += "if (typ == 2) r += i.toString() + ' ';\r\n";
  s += "}\r\n";
  s += "return(r);\r\n";
  s += "}\r\n";
  s += "</script><b>";
  s += ap_ssid;
  cl.print(s);
  cl.print("</b><br><form name=frm><table style=\"width:100%\"><tr><td colspan=2 align=right><u><br>Connection</u></td></tr><tr><td style=\"width:20%\">Baud rate:</td><td style=\"width:80%\"><select size=1 name=bdrt><option>600</option><option>1200</option><option>2400</option>");
  cl.print("<option>4800</option><option>9600</option><option>14400</option><option>19200</option><option>28800</option><option>38400</option><option>56000</option><option>57600</option><option selected>115200</option><option>128000</option><option>256000</option></select></td></tr>");
  cl.print("<tr><td>Data bits:</td><td><select size=1 name=dbt><option>5</option><option>6</option><option>7</option><option selected>8</option></select></td></tr><tr><td>Stop bits:</td><td><select size=1 name=sbt><option selected>");
  cl.print("1</option><option>2</option></select></td></tr><tr><td>Parity:</td><td><select size=1 name=prty><option value=0 selected>None</option><option value=1>Odd</option><option value=2>Even</option></select></td></tr>");
  cl.print("<tr><td></td><td><input type=button class=b2 value=Connect name=conbtn onclick=\"con();\"><input type=button class=b2 value=Disonnect name=disbtn disabled onclick=\"dis();\"></td></tr><tr><td colspan=2 align=right><u><br>Transmitter</u></td></tr>");
  cl.print("<tr><td>Data:</td><td><input name=snd></td></tr><tr><td>Append data:</td><td><select name=apnd size=1><option value=0>None</option><option value=1>CR</option><option value=2>LF</option><option selected value=3>CR+LF</option></select></td></tr>");
  cl.print("<tr><td>Data type:</td><td><select name=dttp size=1><option value=0 selected>ASCII</option><option value=1>HEX (example: 21 f3 1a)</option><option value=2>Decimal (example: 5 241 94 123)</option></select></td></tr>");
  cl.print("<tr><td></td><td><input type=button class=b4 value=Send onclick=\"snddt();\"><input type=button class=b4 value=\"Send CR\" onclick=\"send('0d');\"><input type=button class=b4 value=\"Send LF\" onclick=\"send('0a');\">");
  cl.print("<input type=button class=b4 value=\"Send CR+LF\" onclick=\"send('0d0a');\"></td></tr><tr><td>History:</td><td><textarea name=sndhst style=\"width:100%\" rows=5></textarea></td></tr><tr><td colspan=2 align=right><u><br>Receiver</u></td></tr>");
  cl.print("<tr><td>Display type:</td><td><select name=rectp size=1><option selected value=0>ASCII</option><option value=1>HEX</option><option value=2>Decimal</option></select></td></tr>");
  cl.print("<tr><td>Refresh every:</td><td><select name=ars size=1 onchange=\"arschng();\"><option value=0 selected>None</option><option value=5000>5 second</option><option value=10000>10 seconds</option></select></td></tr>");
  cl.print("<tr><td></td><td><input type=button value=\"Receive Now\" onclick=\"rec();\"></td></tr><tr><td>Received data:</td><td><textarea name=recvd style=\"width:100%\" rows=5></textarea></td></tr></table></form><a href=set>Settings</a>");
}
