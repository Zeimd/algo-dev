function retval = lat_from_azimuth_and_altitude (dec,altitude,azimuth,x)

  z = azimuth;

  hr = deg2rad(altitude);

  a = sin(deg2rad(dec));
  b = sin(hr);
  c = cos(hr)*cos(deg2rad(z));

  xr = deg2rad(x)

  retval = rad2deg(a-b*sin(xr)-c*cos(xr));
endfunction
