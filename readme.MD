# Communicating Sequential Elements Demo
This repo contains code to build four silly demos that performs
automatic brightness/contrast balancing in an input image.

**DISCLAIMER:** I am not, by any means, an expert on image processing
nor I do claim to have any knowledge in the subject. I do know OpenCV
can do the same way faster for it has been optimized by the greatest
minds. This is completely irrelevant. The goal is to demonstrate CSE
running on a CPU, so IDC about whatever a GPU cluster can do.

That said, auto-balance should be performed using Manager-Workers
rather than CSE since it is not an iterative process that converges
towards a solution. However, the heat and weave equations are not
as visual. Furthermore, the demo is for electronic engineers that
may use a grid of microcontrollers that process images/video inter-
acting with their neighbors. So please forgive us for forcing the
wrong pattern into a problem.

The demos performs auto-balance with a different approach each:
- The first one (`demo-seq`) processes the image sequentially.
- The second one (`demo-par`) processes the image in parallel with no
communication among threads whatsoever. The output is, thus, broken.
- The third one (`demo-cse`) processes the image in parallel using the
Communicating Sequential Elements parallel software design pattern.
Two cycles suffice to achieve good results.
- The fourth one (`demo-mw`) processes the image in parallel using the
Manager-Workers parallel software design pattern. Much faster and
cleaner approach.

Auto-balance is done by calculating the histogram (mean) of the image,
discarding pxels below the 0.01% and then scaling the values of each
pixel for all three channels (yes, only works for RGB).

The test image used is that one of Lena. The IEEE can stuck its opinion
on its... you know where. We're not publishing a paper. Alternatively
there is an unlicensed image of His Supreme Gorgeousness, the true
Superman, one and only Witcher, and Emperor of Mankind,
Henry we-are-all-yours Cavill.

## Communicating Sequential Elements
You better learn from the expert [here](https://scholar.google.com/scholar?hl=en&as_sdt=0%2C5&q=%22communicating+sequential+elements%22)

## Build and run

Run these commands:
```bash
cd build
cmake ..
make
cd ..
```

Done. The executables have been built in **this** directory because
of... reasons.

To test them just run
```bash
./demo-seq
./demo-par
./demo-cse
./demo-mw
```

Compare the times.
Each demo accepts one parameter: the full path to the image to process.

That's it. Have fun.
