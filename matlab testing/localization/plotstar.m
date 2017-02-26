function plotstar(A)
data_length = length(A);
x = zeros(1,data_length);
y = zeros(1,data_length);

for i =1:data_length
    [mx,my] = position(A(i,:));
    x(i) = 1024 - mx;
    y(i) = 768 - my;
end

xlim([0 1024]);
ylim([0 768]);
 
hold on;

[x,y] = position(A);
scatter(x,y,'o');
plot([0,0], [0,768]);
plot([0,0], [1028,0]);


hold off;
end
