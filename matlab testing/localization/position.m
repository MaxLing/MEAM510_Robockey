function [x,y] = position(A)
 N = validpoint(A);
% n = length(N)/2;
% if n == 4 || n == 3
[xn,yn,xs,ys] = findstar(N);
theta = orientation(xn,yn,xs,ys);
xc = (xn+xs)/2-512;
yc = (yn+ys)/2-384;
c = cos(theta);
s = sin(theta);

% matrix = [c, -s; s, c];
% north0 = matrix*[xn-512;yn-384];
% south0 = matrix*[xs-512;ys-384];
% xn2 = north0(1)+512;
% yn2 = north0(2)+384;
% xs2 = south0(1)+512;
% ys2 = south0(2)+384;
% x = (xn2 + xs2)/2;
% y = (yn2 + ys2)/2;

 trans = [c,  -s,0,0;
          s, c,0,  0;
          0, 0,1, 0;
          0, 0,0, 1];
 nframe0 = trans * [xn-512; yn-384; 0; 1];
 sframe0 = trans * [xs-512; ys-384; 0; 1];
 xn3 = nframe0(1)+512;
 yn3 = nframe0(2)+384;
 xs3 = sframe0(1)+512;
 ys3 = sframe0(2)+384;
 
 x = (xn3+xs3)/2;
 y = (yn3+ys3)/2;
%   x = c * xc - s * yc+512;
%   y = s * xc + c * yc+384;

% x=-c*xc-s*yc+512;
% y=s*xc-c*yc+384;
 

end

