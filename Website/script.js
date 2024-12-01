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

    function updateDateTime() {
      const dateTimeElement = document.getElementById('dateTime');
      const now = new Date();
    
      // Format date and time
      const options = { 
        weekday: 'long', 
        year: 'numeric', 
        month: 'long', 
        day: 'numeric', 
        hour: '2-digit', 
        minute: '2-digit', 
        second: '2-digit', 
        hour12: true 
      };
      const formattedDateTime = now.toLocaleString('en-US', options);
    
      // Update the element
      dateTimeElement.textContent = formattedDateTime;
    }
    
    // Update the date and time every second
    setInterval(updateDateTime, 1000);
    
    // Initialize the clock
    updateDateTime();
    
    document.addEventListener("DOMContentLoaded", () => {
      // API URLs
      const faceApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V6";
      const LEDApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V2";
      const doorApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V4";
      const motionApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V3";
      const tempApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V0";
      const humApiUrl = "https://blynk.cloud/external/api/get?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V1";
      const doorOnApiUrl = "https://blynk.cloud/external/api/update?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V5=1";
      const doorOffApiUrl = "https://blynk.cloud/external/api/update?token=t5ZCvJ0usmSk7sfhadPg4UUAYvvuVycD&V5=0";

      
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
      
          // Check if the response is OK
          if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
          }
      
          // Handle response depending on type
          const data = await response.text(); // Change to response.json() if API returns JSON
      
          // Interpret the API response
          const doorDetected = data.trim() === "Door is opening" ? "Open" : "Close";
      
          // Update the card status
          const cardDoor = document.getElementById("card-door");
          if (cardDoor) {
            const statusElement = cardDoor.querySelector(".status");
            if (statusElement) {
              statusElement.textContent = doorDetected;
              statusElement.className = `status ${doorDetected === "Open" ? "alert" : ""}`;
            } else {
              console.error("Status element not found inside card-door.");
            }
          } else {
            console.error("Card with ID 'card-door' not found.");
          }
        } catch (error) {
          console.error("Error fetching door detected status:", error);
        }
      }
      
      // Function to fetch Motion Detected status
      async function fetchMotionDetected() {
        try {
          const response = await fetch(motionApiUrl);
      
          // Check if the response is OK
          if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
          }
      
          // Handle response assuming it's a plain string
          const data = await response.text(); // Change to response.json() if API returns JSON
      
          // Interpret the API response
          const motionDetected = data.trim() === "Motion Detected!!" ? "Detected" : "Not Detected";
      
          // Update the card status
          const cardMotion = document.getElementById("card-motion");
          if (cardMotion) {
            const statusElement = cardMotion.querySelector(".status");
            if (statusElement) {
              statusElement.textContent = motionDetected;
              statusElement.className = `status ${motionDetected === "Detected" ? "alert" : ""}`;
            } else {
              console.error("Status element not found inside card-motion.");
            }
          } else {
            console.error("Card with ID 'card-motion' not found.");
          }
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
    

    