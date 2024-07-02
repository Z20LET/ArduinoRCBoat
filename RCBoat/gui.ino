const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>RC Boat</title>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap-icons@1.3.0/font/bootstrap-icons.css" />
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 0;
      overflow: hidden; /* Disable scrolling */
      transition: background-color 0.5s, color 0.5s;
    }
    .container {
      display: flex;
      flex-direction: column;
      justify-content: space-between;
      align-items: center;
      height: 100vh;
    }
    .slider-container {
      display: flex;
      justify-content: space-between; /* Increase distance between sliders */
      width: 80%;
      max-width: 780px; /* Adjust max-width to accommodate increased spacing */
      margin: 20px;
    }
    .slider-wrapper {
      display: flex;
      flex-direction: column;
      align-items: center;
      width: 100px; /* Adjust width for spacing */
    }
    .slider {
      width: 200px;
      height: 50px;
      -webkit-appearance: none;
      appearance: none;
      transform: rotate(270deg);
      background: transparent;
    }
    .slider::-webkit-slider-runnable-track {
      width: 100%;
      height: 8px;
      cursor: pointer;
      background: #ddd;
      border-radius: 5px;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 20px;
      height: 20px;
      background: #2b6be4; /* Updated color */
      border-radius: 50%;
      cursor: pointer;
      margin-top: -6px; /* Adjust to center the thumb */
    }
    .slider::-moz-range-track {
      width: 100%;
      height: 8px;
      cursor: pointer;
      background: #ddd;
      border-radius: 5px;
    }
    .slider::-moz-range-thumb {
      width: 20px;
      height: 20px;
      background: #2b6be4; /* Updated color */
      border-radius: 50%;
      cursor: pointer;
    }
    .slider::-ms-track {
      width: 100%;
      height: 8px;
      cursor: pointer;
      background: transparent;
      border-color: transparent;
      border-width: 8px 0;
      color: transparent;
    }
    .slider::-ms-fill-lower {
      background: #2b6be4; /* Updated color */
      border-radius: 5px;
    }
    .slider::-ms-fill-upper {
      background: #ddd;
      border-radius: 5px;
    }
    .slider::-ms-thumb {
      width: 20px;
      height: 20px;
      background: #2b6be4; /* Updated color */
      border-radius: 50%;
      cursor: pointer;
    }
    .dark-mode {
      background-color: black;
      color: white;
    }
    .switch {
      position: absolute;
      top: 10px;
      right: 10px;
      cursor: pointer;
    }
    .switch i {
      font-size: 24px;
      margin-left: 10px;
    }
    .slider-wrapper label {
      margin-bottom: 10px;
    }
    .speed-display {
      margin-top: 10px;
      width: 100px; /* Fixed width to prevent movement */
    }
    .control-display {
      display: flex;
      justify-content: space-between; /* Match slider-container spacing */
      width: 80%;
      max-width: 780px; /* Match slider-container max-width */
      margin-bottom: 20px; /* Adjust margin as needed */
    }
    .control-item {
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    .signal-icon {
      position: absolute;
      top: 10px;
      left: 10px;
      font-size: 24px;
    }
  </style>
</head>
<body>
  <div class="switch">
    <i class="bi bi-brightness-high-fill" id="toggleDark"></i>
    <i class="bi bi-arrows-fullscreen" id="toggleFullscreen"></i>
  </div>
  <i class="bi bi-wifi signal-icon" id="signalIcon"></i>
  <div class="container">
    <h2>Arduino RC Boat</h2>
    <div class="slider-container">
      <div class="slider-wrapper">
        <input type="range" id="motor1" min="0" max="255" value="0" class="slider">
      </div>
      <div class="slider-wrapper">
        <input type="range" id="motor2" min="0" max="255" value="0" class="slider">
      </div>
    </div>
    <div class="control-display">
      <div class="control-item">
        <label for="motor1">Motor 1</label>
        <div class="speed-display">Speed: <span id="motor1Speed">0</span></div>
      </div>
      <div class="control-item">
        <label for="motor2">Motor 2</label>
        <div class="speed-display">Speed: <span id="motor2Speed">0</span></div>
      </div>
    </div>
  </div>
 <script>
    document.addEventListener('DOMContentLoaded', function() {
      const toggleDark = document.getElementById('toggleDark');
      const toggleFullscreen = document.getElementById('toggleFullscreen');
      const body = document.querySelector('body');
      const signalIcon = document.getElementById('signalIcon');
      
      toggleDark.addEventListener('click', function() {
        this.classList.toggle('bi-moon');
        if (this.classList.toggle('bi-brightness-high-fill')) {
          body.style.background = 'white';
          body.style.color = 'black';
          body.style.transition = '2s';
        } else {
          body.style.background = 'black';
          body.style.color = 'white';
          body.style.transition = '2s';
        }
      });

      toggleFullscreen.addEventListener('click', function() {
        if (!document.fullscreenElement) {
          document.documentElement.requestFullscreen();
          this.classList.remove('bi-arrows-fullscreen');
          this.classList.add('bi-arrows-angle-contract');
        } else {
          if (document.exitFullscreen) {
            document.exitFullscreen();
            this.classList.remove('bi-arrows-angle-contract');
            this.classList.add('bi-arrows-fullscreen');
          }
        }
      });

      var slider1 = document.getElementById("motor1");
      var slider2 = document.getElementById("motor2");
      var output1 = document.getElementById("motor1Speed");
      var output2 = document.getElementById("motor2Speed");

      function resetSlider(slider, output, motor) {
        slider.value = 0;
        output.innerHTML = 0;
        fetch("/" + motor + "?value=0");
      }

      slider1.oninput = function() {
        output1.innerHTML = this.value;
        fetch("/motor1?value=" + this.value);
      }
      slider1.onmouseup = function() {
        resetSlider(slider1, output1, "motor1");
      }
      slider1.ontouchend = function() {
        resetSlider(slider1, output1, "motor1");
      }

      slider2.oninput = function() {
        output2.innerHTML = this.value;
        fetch("/motor2?value=" + this.value);
      }
      slider2.onmouseup = function() {
        resetSlider(slider2, output2, "motor2");
      }
      slider2.ontouchend = function() {
        resetSlider(slider2, output2, "motor2");
      }

      function updateSignalStrength() {
        fetch("/signal-strength")
          .then(response => response.text())
          .then(data => {
            const strength = parseInt(data);
            if (strength > -50) {
              signalIcon.className = "bi bi-wifi signal-icon";
            } else if (strength > -60) {
              signalIcon.className = "bi bi-wifi-2 signal-icon";
            } else if (strength > -70) {
              signalIcon.className = "bi bi-wifi-1 signal-icon";
            } else {
              signalIcon.className = "bi bi-wifi-off signal-icon";
            }
          });
      }

      setInterval(updateSignalStrength, 1000); // Update signal strength every second
    });
</script>
</body>
</html>
)rawliteral";
