import time

import GeodisTK
import SimpleITK as sitk
import os
import numpy as np
import matplotlib.pyplot as plt

path = '../data/arota'
img = sitk.GetArrayFromImage(sitk.ReadImage('C:\\Users\\DELL\\Desktop\\growcutdata\\input_original.mhd')).astype(np.float32)
brush = sitk.GetArrayFromImage(sitk.ReadImage('C:\\Users\\DELL\\Desktop\\growcutdata\\brush.mhd'))
fore = (brush==1).astype(np.uint8)
back = (brush==2).astype(np.uint8)

start = time.time()
threeD_dis_fore = GeodisTK.geodesic3d_raster_scan(img, fore, [1, 1, 1], 1, 5)
threeD_dis_back = GeodisTK.geodesic3d_raster_scan(img, back, [1, 1, 1], 1, 5)
end = time.time()
print('cost time: ', end-start)

plt.figure()
plt.subplot(1, 3, 1)
plt.imshow(img[:, 119, :])
plt.title('original image')
plt.subplot(1,3,2)
plt.imshow(brush[:, 119, :])
plt.title('seed point')
plt.subplot(1,3,3)
plt.imshow(threeD_dis_fore[:, 119, :] < threeD_dis_back[:, 119, :])
plt.title('segmentation result')
plt.show()
