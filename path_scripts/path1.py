def pos(i):
	w = ((100. - i)/100.)
	x = 0.0*w + 2.4*(1 - w)
	y = 0.0*w + 0.33*(1 - w)
	z = 2.43*w + 2.1*(1 - w)
	print("%f %f %f\t0 0 0\t0 1 0\t1.3" % (x, y, z))

for i in range(0,100):
	pos(i)
