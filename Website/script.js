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
      const ledOnApiUrl = "https://sgp1.blynk.cloud/external/api/update?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V1=1";
      const ledOffApiUrl = "https://sgp1.blynk.cloud/external/api/update?token=BkZZ8vHJkh9fu4D7Y5Z8aQp1LR7KX1Ch&V1=0";
    
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
    
      // Function to toggle LED state
      async function toggleLED(isOn) {
        try {
          const url = isOn ? ledOnApiUrl : ledOffApiUrl;
          const response = await fetch(url);
          if (response.ok) {
            console.log(`LED is now ${isOn ? "ON" : "OFF"}`);
          } else {
            console.error("Error updating LED status:", response.statusText);
          }
        } catch (error) {
          console.error("Error toggling LED:", error);
        }
      }
    
      // Set up LED toggle logic
      const ledToggle = document.querySelector(".toggle");
      let ledState = true; // Assume the LED is ON initially
      ledToggle.addEventListener("click", () => {
        ledState = !ledState; // Toggle the LED state
        ledToggle.classList.toggle("active", ledState);
        toggleLED(ledState); // Update API
      });
    
      // Initial fetch and periodic updates
      fetchFaceDetected();
      setInterval(fetchFaceDetected, 2000);
    });
    