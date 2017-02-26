function [xn1,yn1,xs1,ys1] = assignns(xa,ya,xb,yb,xc,yc)
d1 = distance(xa,ya,xb,yb);
d2 = distance(xb,yb,xc,yc);
ratio = d2/d1;
da=distance(xa,ya,xc,yc);

if d2>da
    xn1=xa;
    yn1=ya;
    xs1=xb;
    ys1=yb;
else
    xn1=xb;
    yn1=yb;
    xs1=xa;
    ys1=ya;
end

% if ratio>12/29 && ratio < 18/29
%     xn1 = xb;
%     yn1 = yb;
%     xs1 = xa;
%     ys1 = ya; 
% elseif ratio>19/29 && ratio < 27/29
%     xn1 = xa;
%     yn1 = ya;
%     xs1 = xb;
%     ys1 = yb; 
% 
% end
end