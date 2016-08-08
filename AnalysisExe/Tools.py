from numpy import cos, sin 

def rot(x, y, theta): 
	return (x*cos(theta)-y*sin(theta), x*sin(theta)+y*cos(theta))

def point2RawPoint(par, x, y):
	xx, yy = x+par["x0"], y+par["y0"]
	xx, yy = xx/par["dx"], yy/par["dy"]
	xx, yy = rot(xx, yy, -par["theta"])
	return xx, yy

def getNewX0Y0(old_par, new_par, x, y):
	xx, yy = point2RawPoint(old_par, x, y)
	xx, yy = rot(xx, yy, new_par["theta"])
	xx, yy = xx*new_par["dx"], yy*new_par["dy"]
	xx, yy = xx-x, yy-y
	new_par["x0"]=xx 
	new_par["y0"]=yy

def findNextPar(x0, fx0, x1, fx1):
    return (fx1*x0-fx0*x1)/(fx1-fx0)
