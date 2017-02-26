function [x, y]= filt(x0, y0);
beta=0.9;
alpha=0.9;
x1=x0;x=x0;y1=y0;y=y0;
% for i=2:length(x0)
%     x1(i)=beta*x1(i-1)+beta*(x0(i)-x0(i-1));
%  y1(i)=beta*y1(i-1)+beta*(y0(i)-y0(i-1));
%  
% end
for i=2:length(x1)
x(i)=alpha*x(i-1)+(1-alpha)*(x1(i));
y(i)=alpha*y(i-1)+(1-alpha)*(y1(i));
end
