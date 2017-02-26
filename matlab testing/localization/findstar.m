function [xn,yn,xs,ys] = findstar(A)
if length(A)==8
stardata = num2cell(A);
[x1,x2,x3,x4,y1,y2,y3,y4] = stardata{:};
d12 = distance(x1,y1,x2,y2);
d13 = distance(x1,y1,x3,y3);
d14 = distance(x1,y1,x4,y4);
d23 = distance(x2,y2,x3,y3);
d24 = distance(x2,y2,x4,y4);
d34 = distance(x3,y3,x4,y4);
distances = [d12,d13,d14,d23,d24,d34];
if max(distances) == d12
    [xn,yn,xs,ys] = assignns(x1,y1,x2,y2,x3,y3);
elseif max(distances) == d13
    [xn,yn,xs,ys] = assignns(x1,y1,x3,y3,x4,y4);
elseif max(distances) == d14
    [xn,yn,xs,ys] = assignns(x1,y1,x4,y4,x3,y3);
elseif max(distances) == d23
    [xn,yn,xs,ys] = assignns(x2,y2,x3,y3,x4,y4);
elseif max(distances) == d24
    [xn,yn,xs,ys] = assignns(x2,y2,x4,y4,x3,y3);
elseif max(distances) == d34
    [xn,yn,xs,ys] = assignns(x3,y3,x4,y4,x2,y2);
end
else 
    
   stardata = num2cell(A);
   [x1,x2,x3,y1,y2,y3] = stardata{:};
d12 = distance(x1,y1,x2,y2);
d13 = distance(x1,y1,x3,y3);
d23 = distance(x2,y2,x3,y3);
distances = [d12,d13,d23];
if max(distances) == d12
        
    [xn,yn,xs,ys] = assignns(x1,y1,x2,y2,x3,y3);
elseif max(distances) == d13
    [xn,yn,xs,ys] = assignns(x1,y1,x3,y3,x2,y2);
elseif max(distances) == d23
    [xn,yn,xs,ys] = assignns(x2,y2,x3,y3,x1,y1);
    
end
end
