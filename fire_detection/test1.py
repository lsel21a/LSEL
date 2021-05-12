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

def split_img(img, rows=4, cols=4, show=False, save=False, save_path='./out', save_name=''):
    sizeX = img.shape[1]
    sizeY = img.shape[0]
    
    tx = int(sizeY/rows)
    ty = int(sizeX/cols)

    splits = []

    for i in range(0, rows):
        for j in range(0, cols):
            img_split = img[i * tx:i * tx + tx, j * ty:j * ty + ty]

            if show:
                cv2.imshow('IMG split '+str(i)+str(j),
                           cv2.cvtColor(img_split, cv2.COLOR_RGB2BGR))

            if save:
                out_path = f"{save_path}/split_" + (f"{save_name}_" if save_name else '') + f"{i}{j}.jpg"
                cv2.imwrite(out_path, cv2.cvtColor(
                    img_split, cv2.COLOR_RGB2BGR))

            splits.append(img_split)

    if show:
        cv2.waitKey()
    
    return splits


fire_cascade = cv2.CascadeClassifier('fire_detection.xml')

if __name__ == "__main__":
    img_path = r'./img'
    imgs = get_dir_files(img_path)

    for f in imgs:
        try:
            # f = './img/fire4.jpg'
            # f = './img/fire6.jpg'
            # f = './img/fire1.jpg'
            # f = './img/fire6.jpg'
            # if 'not_' in f or 'brocoli' in f:
            #     continue
            print(f)
            img = cv2.cvtColor(cv2.imread(f), cv2.COLOR_BGR2RGB)
            img = cv2.GaussianBlur(img, (15, 15), 0)

            hls = cv2.cvtColor(img, cv2.COLOR_RGB2HLS)
            hsv = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)
            lab = cv2.cvtColor(img, cv2.COLOR_RGB2LAB)
            yuv = cv2.cvtColor(img, cv2.COLOR_RGB2YUV)

            # cv2.imshow('Original', cv2.cvtColor(img, cv2.COLOR_RGB2BGR))
            cv2.imshow('HLS', cv2.cvtColor(hls, cv2.COLOR_RGB2BGR))
            # cv2.imshow('HSV', cv2.cvtColor(hsv, cv2.COLOR_RGB2BGR))
            # cv2.imshow('LAB', cv2.cvtColor(lab, cv2.COLOR_RGB2BGR))
            # cv2.imshow('YUV', cv2.cvtColor(yuv, cv2.COLOR_RGB2BGR))

            # HLS
            # hls[:, :, 0] = np.zeros([hls.shape[0], hls.shape[1]])
            lower = np.array([0, 100, 175])
            upper = np.array([40, 190, 255])
            # lower = np.array([0, 100, 150])
            # upper = np.array([50, 200, 255])

            # HSV
            # lower = np.array([0, 100, 200])
            # upper = np.array([50, 255, 255])

            # LAB
            # lower = np.array([150, 100, 150])
            # upper = np.array([255, 200, 200])

            # YUV
            # lower = np.array([100, 0, 100])
            # upper = np.array([255, 100, 200])

            mask = cv2.inRange(hls, lower, upper)
            bit = cv2.bitwise_and(img, img, mask=np.uint8(mask))

            masked = cv2.countNonZero(mask)
            # cv2.imshow(f"OUT - {f}", cv2.cvtColor(bit, cv2.COLOR_RGB2BGR))
            gray = cv2.cvtColor(bit, cv2.COLOR_BGR2GRAY)
            ret, thresh = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY)
            contours, hierarchy = cv2.findContours(
                thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
            # cv2.drawContours(img, contours, 0, (0, 255, 0), 2)

            img_out = img.copy()
            # print(contours)
            for i, c in enumerate(contours):
                M = cv2.moments(c)
                try:
                    # print("center X : '{}'".format(round(M['m10'] / M['m00'])))
                    # print("center Y : '{}'".format(round(M['m01'] / M['m00'])))

                    cv2.drawContours(img_out, contours, i, (0, 255, 0), 2)
                    # cv2.circle(img_out, (round(M['m10'] / M['m00']),
                    #         round(M['m01'] / M['m00'])), 5, (0, 255, 0), -1)
                except Exception as err:
                    print(err)

            avg_color_row = np.average(img, axis=0)
            avg_color1 = np.average(avg_color_row, axis=0)

            avg_r = bit[:, :, 0]
            avg_r = np.average(avg_r[avg_r != 0])

            avg_g = bit[:, :, 1]
            avg_g = np.average(avg_g[avg_g != 0])

            avg_b = bit[:, :, 2]
            avg_b = np.average(avg_b[avg_b != 0])

            avg_color2 = [avg_r, avg_g, avg_b]
            print(avg_color1, avg_color2, "{:02X}{:02X}{:02X}".format(
                ceil(avg_r), ceil(avg_g), ceil(avg_b)), np.mean(avg_color2))


            cv2.imshow("outline contour & centroid",
                       cv2.cvtColor(img_out, cv2.COLOR_RGB2BGR))
            # cv2.imshow("gray", gray)
            # cv2.imshow("gray", thresh)

            # cv2.imshow(f"OUT", cv2.cvtColor(bit, cv2.COLOR_RGB2BGR))

            # frame = cv2.imread(f)
            # blur = cv2.GaussianBlur(frame, (21, 21), 0)
            # hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

            # mask = cv2.inRange(hsv, lower, upper)

            # output = cv2.bitwise_and(frame, hsv, mask=mask)
            # no_red = cv2.countNonZero(mask)
            # cv2.imshow("output", output)
            # #print("output:", frame)
            # if int(no_red) > 20000:
            #     print('Fire detected')

            # mask = np.zeros((img.shape[0], img.shape[1]), dtype=np.uint8)

            # for i in range(img.shape[0]):
            #     for j in range(img.shape[1]):
            #         R = int(img[i][j][0])
            #         G = int(img[i][j][1])
            #         B = int(img[i][j][2])
            #         if G > 0:
            #             cond1 = (R/G) < 0.2
            #         else:
            #             cond1 = True
            #         if cond1:
            #             mask[i][j] = 255
            #         else:
            #             mask[i][j] = 0
            # bit = cv2.bitwise_and(img, img, mask=np.uint8(mask))

            # lower = np.array([11,33,111])
            # upper = np.array([90,255,255])

            # lower = np.array([0, 0, 190])
            # upper = np.array([255, 255, 255])
            # mask = cv2.inRange(img, lower, upper)
            # bit = cv2.bitwise_and(img, img, mask=np.uint8(mask))
            # print(c)

            # cv2.imshow('Original', cv2.cvtColor(img, cv2.COLOR_RGB2BGR))
            # cv2.imshow('HLS', cv2.cvtColor(hls, cv2.COLOR_RGB2BGR))
            # # cv2.imshow('HSV', cv2.cvtColor(hsv, cv2.COLOR_RGB2BGR))
            # cv2.imshow('HLS_GRY', cv2.cvtColor(cv2.cvtColor(
            #     hls, cv2.COLOR_RGB2GRAY), cv2.COLOR_RGB2BGR))

            
            
            # cv2.imshow(f"OUT - {f}", cv2.cvtColor(bit, cv2.COLOR_RGB2BGR))
            # a = fire_cascade.detectMultiScale(img, 1.2, 5)
            # print(a)
            # gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
            # for (x, y, w, h) in a:
            #     cv2.rectangle(img, (x-20, y-20), (x+w+20, y+h+20), (255, 0, 0), 2)
            #     roi_gray = gray[y:y+h, x:x+w]
            #     roi_color = img[y:y+h, x:x+w]
            #     print("fire is detected")
            # cv2.imshow('frame', img)

            # for s in split_img(img, save=True, cols=3, rows=3):
            #     avg_color_row = np.average(s, axis=0)
            #     avg_color = np.average(avg_color_row, axis=0)
            #     hsv = colorsys.rgb_to_hsv(
            #         avg_color[0], avg_color[1], avg_color[2])

            #     print(avg_color, "{:02X}{:02X}{:02X}".format(
            #         ceil(avg_color[0]), ceil(avg_color[1]), ceil(avg_color[2])), hsv)

        except Exception as err:
            print(err)

        cv2.waitKey()
        cv2.destroyAllWindows()
        # break
