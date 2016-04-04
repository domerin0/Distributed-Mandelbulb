steps = 150

x0 = 0.647583
y0 = 0.262089
z0 = 0.500653

xx0 = 0.000000
yy0 = 1.000000
zz0 = -0.500000

upx0 = 0.000000
upy0 = 0.000000
upz0 = -1.000000

x1 = 0.600094
y1 = 0.297869
z1 = 0.463938

xx1 = 0.000000
yy1 = 0.750000
zz1 = 0.000000

upx1 = 0.000000
upy1 = 0.000000
upz1 = -1.000000

fov = 1.3

def pos(i):
	w = (1.0*(steps - i)/steps)
	
	x = x0*w + x1*(1-w)
	y = y0*w + y1*(1-w)
	z = z0*w + z1*(1-w)
	
	xx = xx0*w + xx1*(1-w)
	yy = yy0*w + yy1*(1-w)
	zz = zz0*w + zz1*(1-w)
	
	upx = upx0*w + upx1*(1-w)
	upy = upy0*w + upy1*(1-w)
	upz = upz0*w + upz1*(1-w)
	
	print("%f %f %f\t%f %f %f\t%f %f %f\t%f" % (x, y, z, xx, yy, zz, upx, upy, upz, fov))

for i in range(0,steps):
	pos(i+1)
