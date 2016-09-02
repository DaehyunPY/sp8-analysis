from Tools import *

# change pixel size 
par0 = {
	"theta": 0.0,
	"dx": 0.927,
	"dy": 0.927,
	"x0": 93.3187219241,
	"y0": 94.7328442349
}
par1 = {
	"theta": 0.0,
	"dx": 0.877104128624,
	"dy": 0.894463238795
}
getNewX0Y0(par0, par1, 0.0, 0.0)
print(par1["x0"])
print(par1["y0"])

# find next parameters 
print(findNextPar(88.2+91.3j, 
                  3.33075e-01+5.83962e-01J,
                  88.2367488456+91.3671075727j, 
                  3.33075e-01+5.83962e-01J
                  ))
