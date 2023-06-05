# webcam-rockpaperscissors
2007년 SoC RobotWar 예선 출품작(및 본선 진출)

## Description
This repository contains entries from the '2007 SoC Robot War' competition, which I participated in as a sophomore undergraduate. It was my first attempt at MFC-based Windows programming, and it was a project that introduced me to the field of computer vision.

Since the use of third-party libraries was not allowed, it was implemented using only MFC and VFW. Thanks to this, I acquired a deep understanding of pixels and knowledge about libraries.

![RSP](https://github.com/blastak/webcam-rockpaperscissors/assets/12149098/9b58a244-0a93-4c29-ae70-be001997d693)

## Algorithm
1. Image binarization process with YCbCr color thresholding
2. Iterative wrist area removal process by calculating the difference between the center of gravity (COG) and the center point (CP) of the rectangular bounding box (COG and CP are getting closer)
3. Draw a circle based on COG, and count the number of fingers while exploring the circle at each angle.

## Environments
- MFC
- VFW (Video For Windows)
- Old-fashioned webcam with RGB24 support

