function theta = orientation(xno,yno,xso,yso)
% angle = acos((yno-yso)/sqrt((xno-xso)^2+(yno-yso)^2));
% if xno >= xso
%     theta = angle;
% elseif xno < xso
%     theta = -angle;
% end

theta=atan2((xno-xso),(yno-yso));
end