const URL = "my_model/";

let model, webcam, labelContainer, maxPredictions;
let isPredicting = false;
let loopRequestId = null;

async function init() {
  const modelURL = URL + "model.json";
  const metadataURL = URL + "metadata.json";
  
  model = await tmImage.load(modelURL, metadataURL);
  maxPredictions = model.getTotalClasses();
  
  const flip = true;
  webcam = new tmImage.Webcam(400, 285, flip);
  await webcam.setup();
  
  labelContainer = document.getElementById("label-container");
  for (let i = 0; i < maxPredictions; i++) {
    labelContainer.appendChild(document.createElement("div"));
  }

  // Append the webcam canvas to the container
  document.getElementById("webcam-container").appendChild(webcam.canvas);
}

async function start() {
  if (!isPredicting) {
    await webcam.play();
    isPredicting = true;
    loop();
  }
}

function stop() {
  if (isPredicting) {
    webcam.stop();
    isPredicting = false;
    if (loopRequestId) {
      window.cancelAnimationFrame(loopRequestId);
      loopRequestId = null;
    }
  }
}

async function loop() {
  webcam.update();
  await predict();
  if (isPredicting) {
    loopRequestId = window.requestAnimationFrame(loop);
  }
}

let lastUpdate = 0;
async function predict() {
  const prediction = await model.predict(webcam.canvas);
  const now = Date.now();

  if (now - lastUpdate > 1000) { // 0.5-second delay
    for (let i = 0; i < maxPredictions; i++) {
      const classPrediction =
        prediction[i].className + ": " + prediction[i].probability.toFixed(2);
      labelContainer.childNodes[i].innerHTML = classPrediction;
    }
    lastUpdate = now;
  }
}

let button = document.getElementById("aibutton");
button.style.background = "linear-gradient(to right, purple, blue)";
let webcamcont = document.getElementById("webcam-container");
let labelcont = document.getElementById("label-container");

document.getElementById("aibutton").addEventListener("click", function() {
  if (isPredicting) {
    stop();
    button.innerHTML = "Start"; 
    button.style.background = "linear-gradient(to right, purple, blue)"; 
    webcamcont.innerHTML = "";
    labelcont.innerHTML = "";
  } else {
    init().then(start);
    button.innerHTML = "Stop";
    button.style.background = "linear-gradient(to right, crimson, purple)";
    button.classList.add("animateBackground");
  }
});

if (navigator.mediaDevices && navigator.mediaDevices.getUserMedia) {
  console.log("WebRTC is supported");
} else {
  console.error("Your browser does not support WebRTC.");
}
