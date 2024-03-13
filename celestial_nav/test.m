
x = -180 : 0.01 : 180;

% alioth / lat +65
dec = 55.83;
zn = 71.58;
altitude = 56.19;

% canopus / lat -10
dec2 = -52.71;
zn2 = 202.14;
altitude2 = 41.82;

% canopus / lat +30
dec3 = -52.71;
zn3 = 196.35;
altitude3 = 3.85;

% canopus / lat +20
dec3 = -52.71;
zn3 = 196.78;
altitude3 = 13.45;

% canopus / lat +10
dec3 = -52.71;
zn3 = 10; %196.78;
altitude3 = 13.45;

% canopus / lat -30
dec4 = -52.71;
zn4 = 213.73;
altitude4 = 59.62;

% canopus / lat -40
dec4 = -42.71; %-52.71;
zn4 = 226.72;
altitude4 = 67.31;

figure(1);
plot(x,lat_from_azimuth_and_altitude(dec,altitude,zn,x),"k");
hold on;
plot(x,lat_from_azimuth_and_altitude(dec2,altitude2,zn2,x),"b");
hold on;
plot(x,lat_from_azimuth_and_altitude(dec3,altitude3,zn3,x),"r");
hold on;
plot(x,lat_from_azimuth_and_altitude(dec4,altitude4,zn4,x),"g");
