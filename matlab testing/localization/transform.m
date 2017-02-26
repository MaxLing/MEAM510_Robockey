function [x,y] = transform(theta,xc,yc)
c = cos(theta);
s = sin(theta);
trans = [c,-s,0,xc;
         s, c,0,yc;
         0, 0,1, 0;
         0, 0,0, 1;];
frame1 = trans * [512, 384, 0, 1];
x = frame1(1);
y = frame1(2);