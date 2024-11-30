// LED Toggle Functionality
const toggleSwitches = document.querySelectorAll('.toggle');

toggleSwitches.forEach(toggle => {
  toggle.addEventListener('click', () => {
    toggle.classList.toggle('active'); // Change toggle state
    const status = toggle.classList.contains('active') ? 'ON' : 'OFF';
    console.log(`LED is now: ${status}`); // Log status (can integrate with backend API here)
  });
});
const cards = document.querySelectorAll('.card');
    let draggedCard = null;

    cards.forEach(card => {
      card.addEventListener('dragstart', (e) => {
        draggedCard = e.target;
        setTimeout(() => {
          e.target.style.opacity = '0.5';
        }, 0);
      });

      card.addEventListener('dragend', (e) => {
        setTimeout(() => {
          draggedCard.style.opacity = '1';
          draggedCard = null;
        }, 0);
      });

      card.addEventListener('dragover', (e) => {
        e.preventDefault();
      });

      card.addEventListener('dragenter', (e) => {
        e.preventDefault();
        if (e.target.className === 'card') {
          e.target.style.backgroundColor = '#f1f1f1';
        }
      });

      card.addEventListener('dragleave', (e) => {
        e.target.style.backgroundColor = '';
      });

      card.addEventListener('drop', (e) => {
        e.preventDefault();
        if (e.target.className === 'card') {
          e.target.style.backgroundColor = '';
          if (draggedCard !== e.target) {
            const cardsList = [...document.querySelectorAll('.card')];
            const draggedIndex = cardsList.indexOf(draggedCard);
            const targetIndex = cardsList.indexOf(e.target);

            if (draggedIndex < targetIndex) {
              e.target.after(draggedCard);
            } else {
              e.target.before(draggedCard);
            }
          }
        }
      });
    });

    document.addEventListener("DOMContentLoaded", () => {
      // API URLs
      const faceApiUrl = "https://sgp1.blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V0";
      const LEDApiUrl = "https://blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V4";
      const doorApiUrl = "https://blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V6";
      const motionApiUrl = "https://blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V5";
      const tempApiUrl = "https://blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V2";
      const humApiUrl = "https://blynk.cloud/external/api/get?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V3";
      const doorOnApiUrl = "https://sgp1.blynk.cloud/external/api/update?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V1=1";
      const doorOffApiUrl = "https://sgp1.blynk.cloud/external/api/update?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V1=0";

      
      // Function to fetch Face Detected status
      async function fetchFaceDetected() {
        try {
          const response = await fetch(faceApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const faceDetected = data === 1 ? "Detected" : "Not Detected";
    
          // Update the "Face Detected" card
          const cardSmile = document.getElementById("card-smile");
          const statusElement = cardSmile.querySelector(".status");
          statusElement.textContent = faceDetected;
          statusElement.className = `status ${faceDetected === "Detected" ? "alert" : ""}`;
        } catch (error) {
          console.error("Error fetching face detected status:", error);
        }
      }

      async function fetchTemperature() {
        try {
          const response = await fetch(tempApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const temperature = data +'°C';
    
          // Update the "Face Detected" card
          const cardTemp = document.getElementById("card-temperature");
          const statusElement = cardTemp.querySelector(".status");
          statusElement.textContent = temperature;
        } catch (error) {
          console.error("Error fetching temperature status:", error);
        }
      }

      async function fetchHumidity() {
        try {
          const response = await fetch(humApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const humidity = data +'%';
    
          // Update the "Face Detected" card
          const cardHum = document.getElementById("card-humidity");
          const statusElement = cardHum.querySelector(".status");
          statusElement.textContent = humidity;
        } catch (error) {
          console.error("Error fetching humidity status:", error);
        }
      }
      
      async function fetchLEDDetected() {
        try {
          const response = await fetch(LEDApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const LEDDetected = data === 1 ? "On" : "Off";
    
          const cardLED = document.getElementById("card-light");
          const statusElement = cardLED.querySelector(".status");
          statusElement.textContent = LEDDetected;
          statusElement.className = `status ${LEDDetected === "On" ? "alert" : ""}`;
        } catch (error) {
          console.error("Error fetching LED detected status:", error);
        }
      }

      async function fetchDoorDetected() {
        try {
          const response = await fetch(doorApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const doorDetected = data === 1 ? "Open" : "Close";
    
          const cardDoor = document.getElementById("card-door");
          const statusElement = cardDoor.querySelector(".status");
          statusElement.textContent = doorDetected;
          statusElement.className = `status ${doorDetected === "Open" ? "alert" : ""}`;
        } catch (error) {
          console.error("Error fetching door detected status:", error);
        }
      }

      // Function to fetch Motion Detected status
      async function fetchMotionDetected() {
        try {
          const response = await fetch(motionApiUrl);
          const data = await response.json(); // Assuming the API returns JSON or a number
          const motionDetected = data === 1 ? "Detected" : "Not Detected";
    
          const cardMotion = document.getElementById("card-motion");
          const statusElement = cardMotion.querySelector(".status");
          statusElement.textContent = motionDetected;
          statusElement.className = `status ${motionDetected === "Detected" ? "alert" : ""}`;
        } catch (error) {
          console.error("Error fetching motion detected status:", error);
        }
      }
    
      // Function to toggle Door state
      async function toggleDoor(isOn) {
        try {
          const url = isOn ? doorOnApiUrl : doorOffApiUrl;
          const response = await fetch(url);
          if (response.ok) {
            console.log(`Door is now ${isOn ? "ON" : "OFF"}`);
          } else {
            console.error("Error updating Door status:", response.statusText);
          }
        } catch (error) {
          console.error("Error toggling Door:", error);
        }
      }
    
      const doorToggle = document.querySelector(".toggle");
      let doorState = true; // Assume the Door is ON initially
      doorToggle.addEventListener("click", () => {
        doorState = !doorState; // Toggle the Door state
        doorToggle.classList.toggle("active", doorState);
        toggleDoor(doorState); // Update API
      });
    
      // Initial fetch and periodic updates
      fetchFaceDetected();
      setInterval(fetchFaceDetected, 2000);
      fetchHumidity();
      setInterval(fetchHumidity, 2000);
      fetchTemperature();
      setInterval(fetchTemperature, 2000);
      fetchLEDDetected();
      setInterval(fetchLEDDetected, 2000);
      fetchDoorDetected();
      setInterval(fetchDoorDetected, 2000);
      fetchMotionDetected();
      setInterval(fetchMotionDetected, 2000);
      
    });
    