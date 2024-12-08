// Reusable function to load content into a placeholder
function loadContent(filePath, placeholderId) {
    const placeholder = document.getElementById(placeholderId);
    if (!placeholder) {
      console.error(`Placeholder with id "${placeholderId}" not found.`);
      return;
    }
  
    fetch(filePath)
      .then((response) => {
        if (!response.ok) throw new Error(`Failed to load ${filePath}: ${response.statusText}`);
        return response.text();
      })
      .then((html) => {
        placeholder.innerHTML = html;
  
        // If loading navigation, highlight the current page
        if (placeholderId === "nav-placeholder") {
          highlightCurrentPage();
        }
      })
      .catch((error) => {
        console.error(`Error loading ${filePath}:`, error);
        placeholder.innerHTML = `<div class="error">Failed to load ${filePath}</div>`;
      });
  }
  
  // Highlight the current page link in the navigation
  function highlightCurrentPage() {
    const currentPage = window.location.pathname.split('/').pop() || "index.html"; // Default to index.html if empty
    const links = document.querySelectorAll('.nav-link');
    links.forEach((link) => {
      if (link.getAttribute('href') === currentPage) {
        link.classList.add('active');
      } else {
        link.classList.remove('active'); // Ensure other links don't remain active
      }
    });
  }
  
  
  // Load navigation and footer dynamically
  document.addEventListener("DOMContentLoaded", function () {
    loadContent("nav.html", "nav-placeholder");
    loadContent("footer.html", "footer-placeholder");
  });
  
      

      async function toggleLight(state, controlId) {
        try {
          const endpoint = state === 'on' ? `/Sonoff${controlId}/1` : `/Sonoff${controlId}/0`;
          
          // Toggle the light status
          await fetch(endpoint);
      
          // Immediately fetch the updated status
          const response = await fetch(`/Sonoff${controlId}/Status`);
          const data = await response.json();
          const status = data.data.switch;
          
          // Update the LED based on the new status
          updateLightStatus(status, controlId);
      
        } catch (error) {
          showError(`Error toggling light ${controlId}: ${error}`);
        }
      }

      function updateHeatingPumpStatus() {
        fetch("/aircon/get_sensor_info")
          .then((response) => response.json())
          .then((json) => {
            hideError();
            const htemp = json.htemp;
            const otemp = json.otemp;
            document.getElementById("tempOut").innerHTML = `Outside Temp: ${otemp}`;
            document.getElementById("tempIn").innerHTML = `Inside Temp: ${htemp}`;
          })
          .catch((error) => {
            showError("Connection failure while getting heating pump status");
            setTimeout(updateHeatingPumpStatus, 5000);
          });
      }
      
      function updateSystemStatus() {
        fetch("/system/status")
          .then((response) => {
            if (!response.ok) {
              throw new Error(`Failed to fetch system status: ${response.statusText}`);
            }
            return response.json();
          })
          .then((json) => {
            // Update CPU temperature
            const temp = json.cpu_temp ? json.cpu_temp.temp : "--";
            document.getElementById("cpuTemp").innerHTML = `CPU Temp: ${temp}`;
      
            // Update system time
            const systemTime = json.system_time || "--";
            document.getElementById("systemTime").innerHTML = `System Time: ${systemTime}`;
          })
          .catch((error) => {
            console.error("Error getting system status:", error);
            showError("Connection failure while fetching system status.");
            setTimeout(updateSystemStatus, 5000); // Retry after 5 seconds
          });
      }
      
      
      async function getLightStatus(controlId) {
        try {
          const response = await fetch(`/Sonoff${controlId}/Status`);
          const data = await response.json();
          if (data && data.data && data.data.switch !== undefined) {
            updateLightStatus(data.data.switch, controlId);
          } else {
            throw new Error("Unexpected response format");
          }
        } catch (error) {
          showError(`Error fetching light ${controlId} status: ${error.message}`);
          console.error(error); // Log error for debugging
        }
      }
      
      function updateLightStatus(status, controlId) {
        const led = document.getElementById(`light-led-${controlId}`);
        const statusText = document.getElementById(`light-status-text-${controlId}`);
        if (status === 'on') {
          led.classList.add('on');
          led.classList.remove('off');
          statusText.textContent = `Light ${controlId} is ON`;
        } else if (status === 'off') {
          led.classList.add('off');
          led.classList.remove('on');
          statusText.textContent = `Light ${controlId} is OFF`;
        } else {
          showError(`Invalid light ${controlId} status: ${status}`);
        }
      }
      
      function showError(message) {
              const errorBox = document.getElementById("error-box");
              errorBox.textContent = message;
              errorBox.style.display = "block";
            }
            
      function hideError() {
              const errorBox = document.getElementById("error-box");
              errorBox.style.display = "none";
            }