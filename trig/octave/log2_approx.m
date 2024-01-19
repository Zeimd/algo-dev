x = 0 : 0.0001 : 1

log2_poly1 = polyfit(x,log2(1+x),1);
disp("log2_poly1, n=1")
polyout(log2_poly1,'x')
log2_poly2 = polyfit(x,log2(1+x),2);
disp("log2 poly, n=2")
polyout(log2_poly2,'x')
log2_poly3 = polyfit(x,log2(1+x),3);
disp("log2 poly, n=3")
polyout(log2_poly3,'x')

figure(1)
plot(x,log2(1+x),"k");
hold on;
plot(x,polyval(log2_poly1,x),"r");
hold on;
plot(x,polyval(log2_poly2,x),"g");
hold on;
plot(x,polyval(log2_poly3,x),"b");

figure(2)
plot(x,abs(log2(1+x)-polyval(log2_poly1,x)),"r");
hold on;
plot(x,abs(log2(1+x)-polyval(log2_poly2,x)),"g");
hold on;
plot(x,abs(log2(1+x)-polyval(log2_poly3,x)),"b");
