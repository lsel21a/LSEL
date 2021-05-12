import os
import cv2
import numpy as np
import colorsys

from math import ceil

def get_dir_files(path):
    files_out = []

    for root, dirs, files in os.walk(path):
        for f in files:
            files_out.append(os.path.join(root, f))

    return files_out


cap = cv2.VideoCapture('video/video5.mp4')
writer = cv2.VideoWriter(
    'video/video5_out.mp4', cv2.VideoWriter_fourcc(*'MP4V'), 59.25, (1920, 1080))

while(cap.isOpened()):

    ret, frame = cap.read()
    if ret:
        frame2 = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        frame2 = cv2.GaussianBlur(frame2, (15, 15), 0)

        hls = cv2.cvtColor(frame2, cv2.COLOR_RGB2HLS)

        lower = np.array([0, 100, 175])
        upper = np.array([40, 255, 255])

        mask = cv2.inRange(hls, lower, upper)
        bit = cv2.bitwise_and(hls, hls, mask=np.uint8(mask))
        masked = cv2.countNonZero(mask)

        gray = cv2.cvtColor(bit, cv2.COLOR_BGR2GRAY)
        ret, thresh = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY)
        contours, hierarchy = cv2.findContours(
            thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        frame_out = frame.copy()
        for i, c in enumerate(contours):
            M = cv2.moments(c)
            try:
                cv2.drawContours(frame_out, contours, i, (0, 255, 0), 2)
            except Exception as err:
                print(err)

        # cv2.imshow("Image", cv2.cvtColor(cv2.resize(hls, (1080, 720), fx=0.5,
        #            fy=0.5), cv2.COLOR_RGB2BGR))
        cv2.imshow("Image", frame_out)
        writer.write(frame_out)
    else:
        print('no video')
        cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
        break

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
writer.release()
cv2.destroyAllWindows()
