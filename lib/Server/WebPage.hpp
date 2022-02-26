#include <Arduino.h>

namespace server
{

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style type="text/css">
.button {
  background-color: #2c0e70;
  border: none;
  color: rgb(65, 24, 104);
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
</style>
<body style="background-color: #ffffff ">
<center>
<div> 
<h1>Moto-Gymkhana counter</h1>
</div> <br> <br>
<style> h2 { color: black;} </style> 
<div><h2>
  Timer time: <span id="timerTime">0</span><br><br>
  State: <span id="state">0</span><br><br>
  Access point name: <span id="softPointName">NA</span><br><br>
  Password: <span id="password">NA</span><br><br>
  IP address: <span id="ipAddress">0.0.0.0</span><br><br>
  TCP host name: <span id="hostName">NA</span>
</h2></div><br><br>
<style> h3 { color: white;} </style> 
<div>
    <button class="button" onclick="reset()"><h3>Reset</h3></button>
    <button class="button" onclick="reboot()"><h3>Reboot</h3></button><BR>
</div>
<script>
function reset() 
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "reset", true);
  xhttp.send();
}
function reboot() 
{
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", "reboot", true);
  xhttp.send();
}
setInterval(function() 
{
   getTimerTime();
   getState();
   getSoftPointName();
   getPassword();
   getIpAddress();
   getHostName();
}, 1500); 
function getTimerTime() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("timerTime").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "timerTime", true);
  xhttp.send();
}
function getSoftPointName() {
  var xhttp = new XMLHttpRequest();  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("softPointName").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "softApName", true);
  xhttp.send();
}
function getPassword() {
  var xhttp = new XMLHttpRequest();  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("password").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "password", true);
  xhttp.send();
}
function getIpAddress() {
  var xhttp = new XMLHttpRequest();  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ipAddress").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "ipAddress", true);
  xhttp.send();
}
function getHostName() {
  var xhttp = new XMLHttpRequest();  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("hostName").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "tcpHostName", true);
  xhttp.send();
}
function getState() {
  var xhttp = new XMLHttpRequest();  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("state").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "state", true);
  xhttp.send();
}
</script>
</center>
</body>
</html>
)=====";

}
