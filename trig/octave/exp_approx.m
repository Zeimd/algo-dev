x = 0 : 0.0001 : 1

exp_poly1 = polyfit(x,pow2(1+x),1);
disp("exp poly, n=1")
polyout(exp_poly1,'x')
exp_poly2 = polyfit(x,pow2(1+x),2);
disp("exp poly, n=2")
polyout(exp_poly2,'x')
exp_poly3 = polyfit(x,pow2(1+x),3);
disp("exp poly, n=3")
polyout(exp_poly3,'x')

figure(1)
plot(x,pow2(1+x),"k");
hold on;
plot(x,polyval(exp_poly1,x),"r");
hold on;
plot(x,polyval(exp_poly2,x),"g");
hold on;
plot(x,polyval(exp_poly3,x),"b");

figure(2)
plot(x,abs(pow2(1+x)-polyval(exp_poly1,x)),"r");
hold on;
plot(x,abs(pow2(1+x)-polyval(exp_poly2,x)),"g");
hold on;
plot(x,abs(pow2(1+x)-polyval(exp_poly3,x)),"b");
