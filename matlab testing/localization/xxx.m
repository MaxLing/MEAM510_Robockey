data_length = length(rawStarData);


for i =1:data_length
    [mx,my] = position(rawStarData(i,:));
    x(i) = mx;
    y(i) = my;
end