x = -pi/2 : 0.001 : pi/2;
%x = 0 : 0.001 : pi;

sin_poly1 = polyfit(x,sin(x),1);
disp("sin poly, n=1")
polyout(sin_poly1,'x')
polyval(sin_poly1,0)
polyval(sin_poly1,pi/2)
disp("sin poly, n=3")
sin_poly3 = polyfit(x,sin(x),3);
polyout(sin_poly3,'x')
polyval(sin_poly3,0)
polyval(sin_poly3,pi/2)
disp("sin poly, n=5")
sin_poly5 = polyfit(x,sin(x),5);
polyout(sin_poly5,'x')
polyval(sin_poly5,0)
polyval(sin_poly5,pi/2)

figure(1)
plot(x,sin(x),"k");
hold on;
plot(x,polyval(sin_poly1,x),"r");
hold on;
plot(x,polyval(sin_poly3,x),"g");
hold on;
plot(x,polyval(sin_poly5,x),"b");

figure(2)
plot(x,abs(sin(x)-polyval(sin_poly1,x)),"r");
hold on;
plot(x,abs(sin(x)-polyval(sin_poly3,x)),"g");
hold on;
plot(x,abs(sin(x)-polyval(sin_poly5,x)),"b");


