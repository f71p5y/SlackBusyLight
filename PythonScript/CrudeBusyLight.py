import requests
import serial
import serial.tools.list_ports
import time
import subprocess
import sys

ser = serial.Serial()
get_info_command = "?"


def run_silently():
    try:
        if sys.platform == "win32":
            # On Windows, use CREATE_NO_WINDOW to suppress the console window
            subprocess.run(["python", "your_script.py"], creationflags=subprocess.CREATE_NO_WINDOW)
        else:
            # On macOS and Linux, use subprocess.PIPE to redirect output to /dev/null
            subprocess.run(["python3", "your_script.py"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    except Exception as e:
        print(f"Error: {e}")

def get_response(command):
    def get_response(command):
        if ser is not None:
            ser.write(command)
            response = ser.read()
            return response
        else:
            return False


def get_user_presence(api_token, user_id):
    api_url = "https://slack.com/api/users.getPresence"
    headers = {
        "Authorization": f"Bearer {api_token}",
        "Content-Type": "application/json",
    }
    params = {
        "user": user_id,
    }
    response = requests.get(api_url, headers=headers, params=params)
    if response.status_code == 200:
        data = response.json()
        user = data.get("user", {})
        profile = user.get("profile", {})
        presence = data.get("presence")
        return True
    else:
        return False


def get_user_status(api_token, user_id):
    api_url = "https://slack.com/api/users.info"
    headers = {
        "Authorization": f"Bearer {api_token}",
        "Content-Type": "application/json",
    }
    params = {
        "user": user_id,
    }
    response = requests.get(api_url, headers=headers, params=params)
    if response.status_code == 200:
        data = response.json()
        user = data.get("user", {})
        profile = user.get("profile", {})
        status_text = profile.get("status_text", "No status text")
        return status_text
    else:
        return f"Error: {response.status_code} - {response.text}"


def find_trinket_port():
    # Get a list of all available COM ports
    available_ports = list(serial.tools.list_ports.comports())

    # Iterate through the list of ports and find the one that matches your Trinket M0
    trinket_port = None
    for port in available_ports:
        print(port)
        print(port.device)
        print(port.description)
        if "USB Serial Device" in port.description:
            global ser
            ser = serial.Serial(port.device, baudrate=115200, timeout=1)
            if "CrudeBusyLight" == get_response(get_info_command):
                break


def send_command(status):
    print("Status that will be set: "+status)
    try:
        ser.write(status.encode())
    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    run_silently()
    # Replace 'YOUR_SLACK_APP_TOKEN' with your actual Slack app-level API token
    slack_app_token = 'YOUR_SLACK_APP_TOKEN'

    # Replace 'YOUR_USER_ID' with your actual Slack user ID
    user_id = 'YOUR_USER_ID'

    # Open the serial connection
    find_trinket_port()

    while True:
        if get_user_presence(slack_app_token, user_id):
            command = get_user_status(slack_app_token, user_id)
            print("Fetched state: "+command)
            if "In a meeting" == command:
                send_command("BUSY")
            if "Lunch" == command:
                send_command("LUNCH")
            if "Out sick" == command:
                send_command("AWAY")
            if "In Flow" == command:
                send_command("FLOW")
            if "" == command:
                send_command("AVAILABLE")
            time.sleep(1)
