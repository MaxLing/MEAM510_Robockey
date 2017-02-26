function plotrobot(rawStarData)
data_length = length(rawStarData);
x = zeros(1,data_length);
y = zeros(1,data_length);
num=0;
for i =1:data_length
    N = validpoint(rawStarData(i,:));
n = length(N)/2;
if n == 4 %|| n == 3
    [mx,my] = position(rawStarData(i,:));
    x(i-num) = mx;
    y(i-num) = my;
    nx(i-num) = 512 - x(i-num);
    ny(i-num) = 384 - y(i-num);
else num=num+1;
end
end
 [x1, y1]= filt(x,y);
xlim([0 1025]);
ylim([0 769]);
 
hold on;

plot(x1,y1,'-');
quiver (x1,y1,nx,ny);

end
