import cv2
import requests

# mobius server url, header
url = 'http://203.253.128.177:7579/Mobius/CapstonDesign/BabyTemp'

headers = {
    'Accept': 'application/json',
    'X-M2M-RI': '12345',
    'X-M2M-Origin': 'cap',
    'Content-Type': 'application/vnd.onem2m-res+json; ty=3'
}

class OpenCvCapture:
    def __init__(self):
        for i in reversed(range(10)):
            print ("Testing for presense of camera #{0}...".format(i))
            cv2_cap = cv2.VideoCapture(i)
            if cv2_cap.isOpened():
                break

        if not cv2_cap.isOpened():
            print ("Camera not found!")
            exit(1)

        self.cv2_cap = cv2_cap

    def show_video(self):
        while True:
            ret, img = self.cv2_cap.read()

            if ret == False:
                print ("Error reading image")
                break

            temp_value = img.mean()
            # celsius = (temp_value - 27315) / 100
            temp = 0.04 * temp_value + 27.4
            
            if temp > 35:
                print(f"Temperature: {temp:.2f} °C")
                requests.post(url, headers=headers, json={"m2m:cin": {"con": round(temp, 2)}})

            if cv2.waitKey(5) == 27:
                break

        cv2.destroyAllWindows()

if __name__ == '__main__':
    OpenCvCapture().show_video()
