const buttons = document.querySelectorAll('.item-button');

// Add click event listener to each button
buttons.forEach(button => {
    button.addEventListener('click', () => {
        // Remove 'active' class from all buttons
        buttons.forEach(btn => btn.classList.remove('activate'));

        // Add 'active' class to the clicked button
        button.classList.add('activate');
    });
});



(function() {
  const channelId = '2599999';
  const apiKey = 'SX2MQRH2IHLWECII';
  let phsensor, color, turb, methane, alcohol;

  async function fetchSensorData() {
    const url = `https://blynk.cloud/external/api/getAll?token=J5sf8xbUSsCBJ6Ron8pnFReQvb04G_F8`;

    try {
      const response = await fetch(url);
      const data = await response.json();
      phsensor = data.v1;
      color = data.v3;
      turb = data.v0;
      methane = data.v32;
      alcohol = data.v2;
      updateSensorValues(); // Call a function to update the DOM
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  }

  function updateSensorValues() {
    document.querySelector("#turbidity").innerHTML = `${turb}`;
    document.querySelector("#phsensor").innerHTML = `${phsensor}`;
    document.querySelector("#colorsensor").innerHTML = `${color}`;
    document.querySelector("#methane").innerHTML = `${methane}`;
    document.querySelector("#alcohol").innerHTML = `${alcohol}`;
  }

  // Expose a function to access sensor data from another script
  window.getSensorData = function() {
    return {
      turbidity: turb,
      ph: phsensor,
      color,
      methane,
      alcohol
    };
  };

  setInterval(fetchSensorData, 1000);
  fetchSensorData(); // Initial call
})();



