![logo-crop-109x106](https://github.com/user-attachments/assets/05beee73-00e3-4ccb-882b-eff56e5f3ac7)

# AeroMap
AeroMap is a drone mapping application for Windows.

Originally, AeroMap was a port of OpenDroneMap. I have always been fascinated by the seeming magic of turning a pile of pictures into accurate 2D and 3D models. The way the ODM pipeline is structured really clarifies the whole process. Unfortunately - for me - ODM is written in Python & I'm a C programmer. So, I decided to port it to my "native" language to learn the code base line by line.

Now that I'm hooked, I am continuing development on AeroMap. Anyone that wants to contribute is welcome - whether to improve AeroMap or simply learn the internal workings of photogrammetry software. Here's what you need to get started:

- Visual Studio Community 2019
- Qt 5.15.2
- OpenCV 4.10.0

Once installed, ensure these environment variables are set:
- OPENCV_PATH
- QTDIR
- Path:
  [OPENCV_PATH]\build\bin
  [OPENCV_PATH]\build\x64\vc16\bin
  [QTDIR]\bin

Once you get it building, you should be able to process RGB and Thermal datasets. Multispectral datasets are still a work in progress.
