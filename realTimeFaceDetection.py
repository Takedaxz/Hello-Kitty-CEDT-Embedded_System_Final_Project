import face_recognition
import cv2
import requests
import urllib
import numpy as np
import time

# Blynk Authentication Token and Virtual Pin (V0)
BLYNK_AUTH_TOKEN = "XXX"  # Replace with your Blynk token
BLYNK_URL = f"XXX"

# Video stream link (replace with your stream link)
video_stream_link = "https://b074-49-228-98-217.ngrok-free.app/video"  # Replace with your video stream link

# Load a sample image to recognize (e.g., an image with the person's face you want to recognize)
input_image = face_recognition.load_image_file("Image/Pookan.jpeg")  # Replace with your image path
input_face_encoding = face_recognition.face_encodings(input_image)[0]  # Encode the face

# Create an array to store the known face encodings
known_face_encodings = [input_face_encoding]
known_face_names = ["Pookan"]  # Name associated with the face

# Initialize the stream
img_resp = urllib.request.urlopen(video_stream_link)
byte_data = b''  # Initialize a byte buffer for the stream

last_update_time = time.time()  # Track the last time a Blynk update was sent
update_interval = 5  # Update every 5 seconds

while True:
    byte_data += img_resp.read(1024)  # Read data in chunks
    a = byte_data.find(b'\xff\xd8')  # JPEG start marker
    b = byte_data.find(b'\xff\xd9')  # JPEG end marker

    if a != -1 and b != -1:
        jpg = byte_data[a:b+2]  # Extract the JPEG frame
        byte_data = byte_data[b+2:]  # Keep the remaining data for next frame

        # Convert the byte data to an image
        img_np = np.frombuffer(jpg, dtype=np.uint8)
        frame = cv2.imdecode(img_np, cv2.IMREAD_COLOR)

        # Check if image is captured correctly
        if frame is not None:
            # Convert the frame from BGR (OpenCV) to RGB (Face Recognition)
            rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

            # Find all faces and face encodings in the current frame
            face_locations = face_recognition.face_locations(rgb_frame)
            face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)

            face_detected = False  # Flag to track if any face is detected
            unknown_face_detected = False  # Flag to track if any unknown face is detected

            for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
                # Compare the face encoding with known faces
                matches = face_recognition.compare_faces(known_face_encodings, face_encoding)

                name = "Unknown"  # Default name if no match is found

                if True in matches:
                    first_match_index = matches.index(True)
                    name = known_face_names[first_match_index]
                    face_detected = True  # Face is detected and recognized

                if name == "Unknown":
                    unknown_face_detected = True  # Unknown face detected

                # Draw a rectangle around the face and label it
                cv2.rectangle(frame, (left, top), (right, bottom), (0, 255, 0), 2)
                cv2.putText(frame, name, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (0, 255, 0), 2)

            # Determine the state to send to Blynk
            if (face_detected and not unknown_face_detected) and (time.time() - last_update_time >= update_interval):
                # Send data to Blynk indicating face recognition
                try:
                    response = requests.get(BLYNK_URL + "V0=1")  # Send 1 to V0 to indicate face recognition
                    print("Face recognized! Data sent to Blynk:", response.status_code)
                    last_update_time = time.time()  # Update the last update time
                except Exception as e:
                    print(f"Error sending data to Blynk: {e}")
            elif (not face_detected or unknown_face_detected) and (time.time() - last_update_time >= update_interval):
                # Send data to Blynk indicating no face or unknown face
                try:
                    response = requests.get(BLYNK_URL + "V0=0")  # Send 0 to V0 to indicate no face or unknown face
                    print("No face or unknown face detected! Data sent to Blynk:", response.status_code)
                    last_update_time = time.time()  # Update the last update time
                except Exception as e:
                    print(f"Error sending data to Blynk: {e}")

            # Display the resulting frame
            cv2.imshow('Video', frame)

            # Press 'q' to quit the webcam feed
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        else:
            print("Failed to decode image.")
    
    # To avoid infinite loop or memory issues, ensure stream is not stuck
    if time.time() - last_update_time > 60:  # If no frame is processed for over a minute
        print("No frames detected. Exiting...")
        break

# Release resources
cv2.destroyAllWindows()
