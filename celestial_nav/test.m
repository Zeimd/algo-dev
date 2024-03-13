
x = -90 : 0.001 : 90;

% alioth
dec = 55.83;
zn = 71.58;
altitude = 56.19;

% capella
dec2 = 46.02;
zn2 = 253.02;
altitude2 = 59.64;

figure(1);
plot(x,lat_from_azimuth_and_altitude(dec,altitude,zn,x),"k");
hold on;
plot(x,lat_from_azimuth_and_altitude(dec2,altitude2,zn2,x),"b");
hold on;
plot(x,0,"r");
