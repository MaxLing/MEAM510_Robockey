function B = validpoint(A)
B = [];
n = 1;
for  i = 1:8
    if A(i) ~= 1023;
        B(n) = A(i);
        n = n+1; 
    end
end
end