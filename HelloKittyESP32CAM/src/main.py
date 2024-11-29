import requests

line_token = "9szxfrDJKWMtGkpO0txNctfrHGmsnKha8rYu6xfRKXj"
line_api = "https://notify-api.line.me/api/notify"

def send_line_notification(message):
    headers = {
        "Authorization": f"Bearer {line_token}"
    }
    data = {
        "message": message
    }
    response = requests.post(line_api, headers=headers, data=data)
    if response.status_code == 200:
        print("Notification sent successfully!")
    else:
        print(f"Failed to send notification: {response.status_code}, {response.text}")

send_line_notification("Hello!")
