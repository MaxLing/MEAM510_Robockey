data_length = length(rawStarData);
%x = zeros(1,data_length);
%y = zeros(1,data_length);
num=0;
for i =1:data_length
    N = validpoint(rawStarData(i,:));
n = length(N)/2;
%count1(i)  = n;
if n == 4 || n == 3
    [mx,my] = position(rawStarData(i,:));
    x(i-num) = mx;
    y(i-num) = my;
    nx(i-num) = 512 - x(i-num);
    ny(i-num) = 384 - y(i-num);
    v(i-num) = sqrt(nx(i-num).^2+ny(i-num).^2);
    nnx(i-num) = nx(i-num)/v(i-num);
    nny(i-num) = ny(i-num)/v(i-num);
    %count(i) = 0;
else
    num=num+1;
    %count(i) = num;
end
end
 [x1, y1]= filt(x,y);
xlim([0 1025]);
ylim([0 769]);
 
hold on;

plot(x1,y1,'-');
hold on;
 quiver (x1,y1,nnx,nny,10);
