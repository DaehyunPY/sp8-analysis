from accel import *
e, l = 1.0, 1.0
acc = genAcc(e, l)

m, q, p0 = 3.0, 8.0, 1.0
p1, dt = acc(m, q, p0)
print("Momemtum after Acceleration: {}".format(p1))
print("Flight Time: {}".format(dt))

twoAccs = mulAccs(acc, acc)
pfr, pto, bin = -8.0, 8.0, 100
map = AccMap(twoAccs, m, q, pfr, pto, bin)

t = 2.0
p = map.getPAtT(t)
print("Result Momentum: {}".format(p))