#pragma once
#include <Arduino.h>

const char index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
            font-family: Helvetica;
            display: inline-block;
            margin: 0px auto;
            text-align: center;
        }
        h1 {
            background-color: #338;
            color: white;
            padding: 20px 0px;
        }
        .button {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 2px 20px;
            text-decoration: none;
            font-size: 24px;
            margin: 2px;
            cursor: pointer;
        }
        .button-label {
            font-size: 32px;
            font-weight: bold;
        }
        .button2 {
            background-color: #555555;
        }
        .state {
            color: #338;
            font-weight: bold;
            font-size: 48px;
        }
        .substate {
            color: #55B;
            font-weight: normal;
            font-size: 24px;
        }
    </style>
</head>

<body>
    <h1>Xmas Pulpit Server</h1>
    <p id="state" class="state">%STATUS%<br /></p>
    <p id="substate" class="substate">%SUBSTATUS%<br /></p>
    <p class="button-label">
        Sound:
        <button id="soundOn" class="button">Turn On</button>
        <button id="soundOff" class="button">Turn Off</button>
    </p>
    <p class="button-label">
        Light:
        <button id="lightOff" class="button">Off</button>
        <button id="lightDim" class="button">Dim</button>
        <button id="lightOn" class="button">Bright</button>
    </p>
</body>
<script>
    function refresh() {
        var xhr = new XMLHttpRequest();
        xhr.onreadystatechange = function () {
            if (xhr.readyState == XMLHttpRequest.DONE) {
                const states = xhr.responseText.split('|');
                document.getElementById("state").innerHTML = states[0];
                document.getElementById("substate").innerHTML = states[1];
            }
        }
        xhr.open("GET", "/status", true);
        xhr.send(null);
    }
    setInterval(() => refresh(), 1000);
    document.getElementById("soundOn").addEventListener("click", sound, false);
    document.getElementById("soundOff").addEventListener("click", sound, false);
    function sound(event) {
        console.log("The sound button " + event.target.id + " has been clicked!", event);
        var xhr = new XMLHttpRequest();
        var param = event.target.id.substring(5).toLowerCase();
        xhr.open("GET", "/update?sound=" + param, true);
        xhr.send();
    }

    document.getElementById("lightOff").addEventListener("click", light, false);
    document.getElementById("lightDim").addEventListener("click", light, false);
    document.getElementById("lightOn").addEventListener("click", light, false);
    function light(event) {
        console.log("The light button " + event.target.id + " has been clicked!");
        var xhr = new XMLHttpRequest();
        var level = "0"
        if (event.target.id === "lightDim") {
            level = "128";
        } else if (event.target.id === "lightOn") {
            level = "255";
        }

        xhr.open("GET", "/update?light=" + level, true);
        xhr.send();
    }
</script>

</html>
)=====";
