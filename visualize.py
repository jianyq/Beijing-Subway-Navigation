import os
filepath = os.path.abspath('.')
# print(os.path.abspath('.'))
with open(filepath + r"\\coStation.txt", 'r', encoding="UTF-8") as c:
    c = c.read()
    c = c.split('\n')
    c = [dit.split('，') for dit in c]
stationname = {}
for i, dit in enumerate(c, 0):
    if len(dit) != 0:
        if len(dit) == 1:
            if dit[0][-1] != '*':
                stationname[dit[0]] = c[i + 1]
            else:
                stationname[dit[0][:-1]] = c[i + 1][::-1]

stationxy = {}
with open(filepath + r"\\drawStation.txt", "r", encoding="UTF-8") as d:
    d = d.read()
    d = d.strip().split('\n')
    newd = []
    for line in d:
        line = line.strip().split(',')
        if len(line) == 1:
            newd.append([line[0]])
        else:
            newd.append([eval(line[0]), eval(line[1])])
        # for item in line:
        #     if
        # newd.append([eval(item) for item in line])
    # d = [line.split(',') for line in d]
tmp = []
last = ''
for dit in newd:
    if len(dit) != 0:
        if len(dit) == 1:
            if last != '':
                stationxy[last] = tmp
            tmp = []
            last = dit[0]
        else:
            if len(dit) == 2:
                tmp.append(dit)
stationxy[last] = tmp
stations = {}
with open(filepath + r"\\station_list_id.txt", "r", encoding = "UTF-8") as s:
    s = s.read().split('\n')
    for item in s:
        it = item.split(' ')
        stations[it[0]] = eval(it[1])

for key in stationname:
    for i, per_station in enumerate(stationname[key], 0):
        try:
            a = len(stations[per_station])
            # print()
        except:
            stations[per_station] = (stationxy[key][i][0], stationxy[key][i][1])

with open(filepath + r"\\route.out", "r", encoding="GBK") as r:
    r = r.read().split(" ")

# r = stationname['大兴机场']

# coding: utf-8
import cv2
img = cv2.imread(filepath + r"\\railway.jpg")
for i, st in enumerate(r, 0):
     print(f'{st}:{stations[st]}')
for i, st in enumerate(r[:-1], 0):
    try:
        cv2.circle(img, stations[st], 20, (255, 255, 0), thickness = -1)
        try:
            cv2.circle(img, stations[r[i + 1]], 20, (255, 255, 0), thickness=-1)
            cv2.line(img, stations[st], stations[r[i + 1]], (255, 0, 0), 20)
        except:
            print(f"No station{r[i + 1]}")
    except:
        print(f"No station{st}")

cv2.imwrite(filepath + r"\\picture.jpg", img)
cv2.namedWindow(filepath + r"\\image", cv2.WINDOW_NORMAL)
cv2.imshow(filepath + r"\\image", img)

k = cv2.waitKey(0)
if k == 27:
    cv2.destroyAllWindows()
