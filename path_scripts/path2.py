steps = 100

x0 = 2.4
y0 = 0.33
z0 = 2.1

xx0 = 0.0
yy0 = 0.0
zz0 = 0.0

upx0 = 0.0
upy0 = 1.0
upz0 = 0.0

x1 = 0.696
y1 = 0.0957
z1 = 0.609

xx1 = 0.0
yy1 = 0.5
zz1 = -0.5

upx1 = 0.0
upy1 = 1.0
upz1 = 0.0

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
